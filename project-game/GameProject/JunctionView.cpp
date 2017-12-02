/*==============================================================================

    JunctionView.h - 町の自動生成ー交差点ビュー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "JunctionView.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "Polygon3DRenderer.h"
#include "CityAttribute.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* JunctionView::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<JunctionView>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
JunctionView::JunctionView( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Attribute = NULL;
	m_IsUpdatedAttribute = false;

	//レンダラーの設定
	m_Renderer = m_pGameObject->AddComponent<Polygon3DRenderer>();
	
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void JunctionView::Uninit( void)
{
	m_Attribute->UnregisterView();
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void JunctionView::Update( void)
{
	if (!m_IsUpdatedAttribute)
	{
		return;
	}

	m_IsUpdatedAttribute = false;

	//View情報の更新
	m_pTransform->SetLocalPosition( m_Attribute->GetPosition());
	m_pTransform->SetLocalPositionY( 0.1f);
}

/*------------------------------------------------------------------------------
	属性情報の設定
------------------------------------------------------------------------------*/
void JunctionView::SetAttribute( JunctionAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;

	//View情報の更新
	m_pTransform->SetLocalPosition( m_Attribute->GetPosition());
	m_pTransform->SetLocalPositionY( 0.1f);
}

