/*==============================================================================

    BlockView.cpp - 町の自動生成ー区画ビュー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BlockView.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "Polygon3DRenderer.h"
#include "CityAttribute.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* BlockView::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BlockView>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
BlockView::BlockView( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	
	m_Attribute = NULL;
	m_IsUpdatedAttribute = false;

	//レンダラーの設定
	
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void BlockView::Uninit( void)
{
	m_Attribute->UnregisterView();
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void BlockView::Update( void)
{
	if (!m_IsUpdatedAttribute)
	{
		return;
	}

	m_IsUpdatedAttribute = false;

	//建物を再生成
	m_Attribute->CreateBuilding();
}

/*------------------------------------------------------------------------------
	属性情報の設定
------------------------------------------------------------------------------*/
void BlockView::SetAttribute( BlockAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;

	//建物を生成
	m_Attribute->CreateBuilding();
}

