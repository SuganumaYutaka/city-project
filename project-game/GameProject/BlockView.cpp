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
#include "BlockModel.h"

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
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void BlockView::Uninit( void)
{
	
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
}

/*------------------------------------------------------------------------------
	属性情報の更新
------------------------------------------------------------------------------*/
void BlockView::UpdateAttribute(void)
{
	m_IsUpdatedAttribute = true;
	
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
}

