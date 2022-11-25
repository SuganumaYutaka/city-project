/*==============================================================================

    Component.cpp - コンポーネント（基底クラス）
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/27
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Component.h"
#include "GameObject.h"

/*------------------------------------------------------------------------------
	静的メンバ変数宣言
------------------------------------------------------------------------------*/
std::list<Component*> Component::m_listReleaseComponent;

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Component::Component()
{
	m_pGameObject = NULL;
	m_pTransform = NULL;
	m_nLayer = eLayerDefault;
	m_bEnable = true;
	m_bRelease = false;
	IsCreatedByOtherComponent = false;
}

/*------------------------------------------------------------------------------
	コンストラクタ
	引数
	CScene *pScene
------------------------------------------------------------------------------*/
Component::Component( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_bEnable = true;
	m_bRelease = false;
	IsCreatedByOtherComponent = false;
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Component::~Component()
{

}

/*------------------------------------------------------------------------------
	消去予約
------------------------------------------------------------------------------*/
void Component::ReleaseReserve(void)
{
	if (m_bRelease == true)
	{
		return;
	}

	m_bRelease = true;
	m_bEnable = false;

	m_listReleaseComponent.push_back(this);

}

/*------------------------------------------------------------------------------
	リストの順にコンポーネント消去
------------------------------------------------------------------------------*/
void Component::ReleaseList(void)
{
	for (Component* pComp : m_listReleaseComponent)
	{
		pComp->m_pGameObject->ReleaseComponent(pComp);
	}

	//リストをリセット
	m_listReleaseComponent.clear();
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void Component::Load(Text& text)
{
	do
	{
		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}while ( text.GetWord() != "EndComponent");
}

/*------------------------------------------------------------------------------
	セーブ
------------------------------------------------------------------------------*/
void Component::Save(Text& text)
{
	StartSave(text);
	EndSave(text);
}

/*------------------------------------------------------------------------------
	セーブ開始
------------------------------------------------------------------------------*/
void Component::StartSave(Text& text)
{
	// 他のコンポーネントから生成された場合保存しない
	if (IsCreatedByOtherComponent == true)
	{
		return;
	}

	// クラス名
	text += "Component ";
	text += typeid(*this).name();
	text += "\n";
}

/*------------------------------------------------------------------------------
	セーブ終了
------------------------------------------------------------------------------*/
void Component::EndSave(Text& text)
{
	// 他のコンポーネントから生成された場合保存しない
	if (IsCreatedByOtherComponent == true)
	{
		return;
	}

	text += "EndComponent\n";
}
