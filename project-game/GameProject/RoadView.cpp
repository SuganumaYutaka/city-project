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
	m_GrayRenderer = m_pGameObject->AddComponent<MeshPolygonRenderer>();
	m_GrayRenderer->LoadTexture( "data/TEXTURE/gray.jpg");

	m_WhiteLineRenderer = m_pGameObject->AddComponent<MeshPolygonRenderer>();
	m_WhiteLineRenderer->LoadTexture( "data/TEXTURE/whiteline.png");
	m_WhiteLineRenderer->SetPass( 1);
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void RoadView::Uninit( void)
{
	
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

	//描画情報の更新
	UpdateRenderer();
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

	//描画情報の更新
	UpdateRenderer();
}

/*------------------------------------------------------------------------------
	描画情報の更新
------------------------------------------------------------------------------*/
void RoadView::UpdateRenderer(void)
{
	//道路の頂点を取得
	auto& vertices = m_Attribute->GetVertices();
	
	//交差点分短くする
	float width = m_Attribute->GetWidth();
	Vector3 vector = m_Attribute->GetVector().Normalize();
	vertices[0] += vector * width * ( 0.5f - 0.03f);
	vertices[1] -= vector * width * ( 0.5f - 0.03f);
	vertices[2] -= vector * width * ( 0.5f - 0.03f);
	vertices[3] += vector * width * ( 0.5f - 0.03f);

	//アスファルト
	m_GrayRenderer->SetVertices( vertices);

	//白線
	for (auto& vertex : vertices)
	{
		vertex.y += 0.01f;
	}
	m_WhiteLineRenderer->SetVertices( vertices);
}

