/*==============================================================================

    RoadView.cpp - 町の自動生成ー道路ビュー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "RoadView.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "Polygon3DRenderer.h"
#include "CityAttribute.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* RoadView::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<RoadView>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
RoadView::RoadView( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Attribute = NULL;
	m_IsUpdatedAttribute = false;

	//レンダラーの設定
	m_Renderer = m_pGameObject->AddComponent<Polygon3DRenderer>();
	m_Renderer->LoadTexture( "data/TEXTURE/field.jpg");
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void RoadView::Uninit( void)
{
	m_Attribute->UnregisterView();
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void RoadView::Update( void)
{
	if (!m_IsUpdatedAttribute)
	{
		return;
	}

	m_IsUpdatedAttribute = false;

	//View情報の更新
	m_Renderer->m_pTransform->SetLocalPosition( m_Attribute->GetCenterPosition());
	m_Renderer->m_pTransform->SetWorldRotationLookDirection( m_Attribute->GetVector());
	m_Renderer->m_pTransform->SetLocalScale( m_Attribute->GetWidth(), 1.0f, m_Attribute->GetLength());
}

/*------------------------------------------------------------------------------
	属性情報の設定
------------------------------------------------------------------------------*/
void RoadView::SetAttribute( RoadAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;

	//View情報の更新
	m_Renderer->m_pTransform->SetLocalPosition( m_Attribute->GetCenterPosition());
	m_Renderer->m_pTransform->SetWorldRotationLookDirection( m_Attribute->GetVector());
	m_Renderer->m_pTransform->SetLocalScale( m_Attribute->GetWidth(), 1.0f, m_Attribute->GetLength());
}
