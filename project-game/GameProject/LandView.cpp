/*==============================================================================

    LandView.cpp - 町の自動生成ー区画ビュー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "LandView.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "Polygon3DRenderer.h"
#include "CityAttribute.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* LandView::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<LandView>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
LandView::LandView( GameObject* pGameObject)
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
void LandView::Uninit( void)
{
	m_Attribute->UnregisterView();
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void LandView::Update( void)
{
	if (!m_IsUpdatedAttribute)
	{
		return;
	}

	m_IsUpdatedAttribute = false;

	//View情報の更新

}

/*------------------------------------------------------------------------------
	属性情報の設定
------------------------------------------------------------------------------*/
void LandView::SetAttribute( LandAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;

	//View情報の更新

}

