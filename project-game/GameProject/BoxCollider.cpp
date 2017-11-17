/*==============================================================================

    BoxCollider.cpp - ボックスコライダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BoxCollider.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshBoxRenderer.h"

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* BoxCollider::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BoxCollider>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
BoxCollider::BoxCollider( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = pGameObject->GetComponent<Transform>();
	m_ColType = eColBox;
	m_Size = Vector3(1.0f, 1.0f, 1.0f);

	//デバッグ表示
#ifdef _DEBUG
	auto obj = new GameObject( m_pGameObject->GetRoot());
	obj->IsCreatedByOtherComponent = true;
	m_pRenderer = obj->AddComponent<MeshBoxRenderer>();
	m_pRenderer->SetWireFrame();
	obj->IsCreatedByOtherComponent = true;
#endif // _DEBUG

}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void BoxCollider::Uninit( void)
{
	Manager::GetCollisionManager()->ReleaseCollider( this);

#ifdef _DEBUG
	m_pRenderer->m_pGameObject->ReleaseReserve();
#endif // _DEBUG
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void BoxCollider::Update()
{
#ifdef _DEBUG
	m_pRenderer->m_pTransform->SetWorldPosition( m_pTransform->GetWorldPosition());

	//中心位置をずらす
	D3DXVECTOR3 center = m_Center.ConvertToDX();
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationQuaternion( &mtxRot, &m_pTransform->GetWorldRotation());
	D3DXVec3TransformCoord( &center, &center, &mtxRot);
	m_pRenderer->m_pTransform->Move( Vector3::ConvertFromDX(center));

#endif // _DEBUG
}

/*------------------------------------------------------------------------------
	衝突
	引数
		Collider *pCollider			衝突したコライダー
------------------------------------------------------------------------------*/
void BoxCollider::OnCollision( Collider *pCollider)
{
	
}

/*------------------------------------------------------------------------------
	中心位置設定
------------------------------------------------------------------------------*/
void BoxCollider::SetCenter(const Vector3& Center)
{
	m_Center = Center;

	//デバッグ表示
#ifdef _DEBUG
	m_pRenderer->SetCenter( Center);

#endif // _DEBUG
}

/*------------------------------------------------------------------------------
	大きさ設定
------------------------------------------------------------------------------*/
void BoxCollider::SetSize(const Vector3& Size)
{
	m_Size = Size;

	//デバッグ表示
#ifdef _DEBUG
	m_pRenderer->SetSize( Size);

#endif // _DEBUG
}

/*------------------------------------------------------------------------------
	アクティブ設定
------------------------------------------------------------------------------*/
void BoxCollider::SetActive(bool bEnable)
{
	if (m_bEnable == true && bEnable == false)
	{
		//マネージャーから削除
		Manager::GetCollisionManager()->ReleaseCollider( this);
	}
	else if (m_bEnable == false && bEnable == true)
	{
		//マネージャーに登録
		Manager::GetCollisionManager()->AddCollider( this);
	}

	m_bEnable = bEnable;

#ifdef _DEBUG
	m_pRenderer->SetActive( bEnable);

#endif // _DEBUG

}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void BoxCollider::Load(Text& text)
{
	//textを読み進める
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "Center")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_Center.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}
		if (text.GetWord() == "Size")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_Size.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}
		if (text.GetWord() == "IsTrigger")
		{
			text.ForwardPositionToNextWord();
			m_IsTrigger = (bool)std::stoi(text.GetWord());
		}

		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	セーブ
------------------------------------------------------------------------------*/
void BoxCollider::Save(Text& text)
{
	StartSave(text);

	text += "Center " + m_Center.ConvertToString() + '\n';
	text += "Size " + m_Size.ConvertToString() + '\n';
	text += "IsTrigger" + std::to_string((int)m_IsTrigger) ; '\n';

	EndSave( text);
}