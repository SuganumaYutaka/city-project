/*==============================================================================
	
	GameObject.cpp - オブジェクトノード
														Author : Yutaka Suganuma
														Date   : 2017/4/29
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "GameObject.h"
#include "Component.h"
#include "ComponentFactory.h"

/*------------------------------------------------------------------------------
	静的メンバ変数宣言
------------------------------------------------------------------------------*/
std::list<GameObject*> GameObject::m_listReleaseObject;

/*------------------------------------------------------------------------------
	デフォルトコンストラクタ
	シリアライズ用
------------------------------------------------------------------------------*/
GameObject::GameObject( )
{
	m_bRelease = false;
	IsCreatedByOtherComponent = false;
	IsSelected = false;
}

/*------------------------------------------------------------------------------
	コンストラクタ
	引数
		GameObject* pParent		親ノード
------------------------------------------------------------------------------*/
GameObject::GameObject( GameObject* pParent)
{
	InitGameObject( pParent);
}

/*------------------------------------------------------------------------------
	初期化
	引数
		GameObject* pParent		親ノード
------------------------------------------------------------------------------*/
void GameObject::InitGameObject( GameObject* pParent)
{
	m_pParent = pParent;
	m_Tag = "UnTag";
	
	if (m_pParent != NULL)
	{
		m_pParent->AddChild( this);
		m_pRoot = m_pParent->m_pRoot;
	}
	else
	{
		m_pRoot = this;
	}

	m_listChild.clear();
	m_listComponent.clear();
	m_pTransform = AddComponent<Transform>();
	m_bRelease = false;
	IsCreatedByOtherComponent = false;
	IsSelected = false;
}

/*------------------------------------------------------------------------------
	子ノード追加
	引数
		GameObject* pChild
------------------------------------------------------------------------------*/
void GameObject::AddChild( GameObject* pChild)
{
	m_listChild.push_back( pChild);
	this;
}

/*------------------------------------------------------------------------------
	全オブジェクト更新
------------------------------------------------------------------------------*/
void GameObject::UpdateAll()
{
	Update();

	//子を再帰的に更新
	std::for_each( m_listChild.begin(), m_listChild.end(), 
		[]( GameObject* pChild) { pChild->UpdateAll();});
}

/*------------------------------------------------------------------------------
	全オブジェクト描画
------------------------------------------------------------------------------*/
void GameObject::DrawAll()
{
	Draw();

	//子を再帰的に描画
	std::for_each(m_listChild.begin(), m_listChild.end(), 
		[]( GameObject* pChild) { pChild->DrawAll();});
}

/*------------------------------------------------------------------------------
	オブジェクト消去予約
------------------------------------------------------------------------------*/
void GameObject::ReleaseReserve( void )
{
	if (m_bRelease == true)
	{
		return;
	}

	//リストに追加
	m_listReleaseObject.push_back( this);

	m_bRelease = true;

	//コンポーネントを実行不能に
	for (Component* pComp : m_listComponent)
	{
		pComp->SetActive( false);
	}

	//子を再帰的に消去予約
	std::list< GameObject*>::iterator ite;
	for( ite = m_listChild.begin(); ite != m_listChild.end(); ite++)
	{
		( *ite)->ReleaseReserveChildren();
	}
}

/*------------------------------------------------------------------------------
	子のオブジェクト消去予約
------------------------------------------------------------------------------*/
void GameObject::ReleaseReserveChildren( void )
{
	if( m_bRelease == true)
	{
		//リストから消去
		std::list< GameObject*>::iterator ite;
		for( ite = m_listReleaseObject.begin(); ite != m_listReleaseObject.end(); ite++)
		{
			if( *ite == this)
			{
				ite = m_listReleaseObject.erase( ite);
				break;
			}
		}
	}

	m_bRelease = true;

	//コンポーネントを実行不能に
	for (Component* pComp : m_listComponent)
	{
		pComp->SetActive( false);
	}

	//子を再帰的に消去予約
	std::list< GameObject*>::iterator ite;
	for( ite = m_listChild.begin(); ite != m_listChild.end(); ite++)
	{
		( *ite)->ReleaseReserveChildren();
	}
}

/*------------------------------------------------------------------------------
	リストの順にオブジェクト消去
------------------------------------------------------------------------------*/
void GameObject::ReleaseList(void)
{
	for(GameObject* Obj : m_listReleaseObject)
	{
		Obj->ReleaseObject();
	}

	//リストをリセット
	m_listReleaseObject.clear();
}

/*------------------------------------------------------------------------------
	オブジェクト消去実行
------------------------------------------------------------------------------*/
void GameObject::ReleaseObject()
{
	//親のリストから消去
	std::list< GameObject*>::iterator ite;
	if( m_pParent != NULL && m_pParent->m_Tag != "Root")
	{
		for( ite = m_pParent->m_listChild.begin(); ite != m_pParent->m_listChild.end(); ite++)
		{
			//リストから消去
			if( *ite == this)
			{
				ite = m_pParent->m_listChild.erase( ite);
				break;
			}
		}
	}

	ReleaseRecursive();
}

/*------------------------------------------------------------------------------
	再帰的にオブジェクト消去
------------------------------------------------------------------------------*/
void GameObject::ReleaseRecursive()
{
	//子を再帰的に消去
	std::list< GameObject*>::iterator ite;
	for( ite = m_listChild.begin(); ite != m_listChild.end(); ite++)
	{
		( *ite)->ReleaseRecursive();
	}

	//コンポーネント終了処理
	for( auto pComp : m_listComponent)
	{
		pComp->Uninit();
	}

	//コンポーネントの消去
	std::list< Component *>::iterator iteComp;
	for( iteComp = m_listComponent.begin(); iteComp != m_listComponent.end(); iteComp++)
	{
		delete *iteComp;
	}

	//自身の消去（必ず最後に）
	delete this;
}

/*------------------------------------------------------------------------------
	コンポーネントを消去
------------------------------------------------------------------------------*/
void GameObject::ReleaseComponent(Component* pComp)
{
	//コンポーネントの消去
	for (auto comp : m_listComponent)
	{
		if( comp == pComp)
		{
			pComp->Uninit();
			delete pComp;
			break;
		}
	}

	//リストから消去
	std::list< Component *>::iterator iteComp;
	for( iteComp = m_listComponent.begin(); iteComp != m_listComponent.end(); iteComp++)
	{
		if( *iteComp == pComp)
		{
			iteComp = m_listComponent.erase( iteComp);
			break;
		}
	}
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void GameObject::Update()
{
	//コンポーネントの更新
	for (Component *pComponent : m_listComponent)
	{
		if ( pComponent->GetActive() == true)
		{
			pComponent->Update();
		}
	}

	if( IsSelected)
	{
		SetImGuiView();
	}
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void GameObject::Draw()
{
	////コンポーネントの描画（RenderManagerで管理する）
	//std::for_each(m_listComponent.begin(), m_listComponent.end(), 
	//	[]( Component* pComponent) { pComponent->Draw();});
}

/*------------------------------------------------------------------------------
	衝突時の処理
------------------------------------------------------------------------------*/
void GameObject::OnCollision(Collider *pCollider)
{
	//コンポーネントにコール
	for (Component* pComp : m_listComponent)
	{
		pComp->OnCollision( pCollider);
	}
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void GameObject::Load(Text& text)
{
	//textを読み進める
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndGameObject")
	{
		// GameObject
		if (text.GetWord() == "Tag")
		{
			text.ForwardPositionToNextWord();
			m_Tag = text.GetWord();
		}
		else if (text.GetWord() == "IsSelected")
		{
			text.ForwardPositionToNextWord();
			IsSelected = (bool)std::stoi(text.GetWord());
		}

		// Component
		else if (text.GetWord() == "Component")
		{
			text.ForwardPositionToNextWord();
			text.ForwardPositionToNextWord();

			// transformだけ別の処理
			if (text.GetWord() == "Transform")
			{
				m_pTransform->Load(text);
			}
			else
			{
				auto pComponent = Manager::GetComponentFactory()->Create( text.GetWord(), this);
				pComponent->Load( text);
			}
		}

		// 子のゲームオブジェクト
		else if (text.GetWord() == "GameObject")
		{
			auto pGameObject = new GameObject(this);
			pGameObject->Load(text);
		}

		// textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}
}

/*------------------------------------------------------------------------------
	セーブ
------------------------------------------------------------------------------*/
void GameObject::Save(Text& text)
{
	// 他のコンポーネントから生成された場合保存しない
	if (IsCreatedByOtherComponent == true)
	{
		return;
	}

	// GameObject
	text += "GameObject\n";
	text += "Tag " + m_Tag + "\n";
	text += "IsSelected " + std::to_string((int)IsSelected) + '\n';

	// Component
	for (auto *pComponent : m_listComponent)
	{
		if (pComponent->IsCreatedByOtherComponent == false)
		{
			pComponent->Save(text);
		}
	}

	// 子のゲームオブジェクト
	for (auto *pGameObject : m_listChild)
	{
		if (pGameObject->IsCreatedByOtherComponent == false)
		{
			pGameObject->Save(text);
		}
	}

	text += "EndGameObject\n";
}

/*------------------------------------------------------------------------------
	ImGuiの表示情報設定
------------------------------------------------------------------------------*/
void GameObject::SetImGuiView()
{
	ImGui::Begin(m_Tag.c_str(), &IsSelected);
	//コンポーネントの更新
	for (Component *pComponent : m_listComponent)
	{
		pComponent->SetImGuiView();
	}
	ImGui::End();
}
