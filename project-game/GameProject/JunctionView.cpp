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

#include "Edge.h"
#include "Vertex.h"
#include "HalfEdge.h"

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
	m_Renderer->LoadTexture( "data/TEXTURE/junction_+.png");
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void JunctionView::Uninit( void)
{
	
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
	UpdateRenderer();
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
	UpdateRenderer();
}

/*------------------------------------------------------------------------------
	描画情報の更新
------------------------------------------------------------------------------*/
void JunctionView::UpdateRenderer(void)
{
	m_pTransform->SetLocalPosition( m_Attribute->GetPosition());
	m_pTransform->SetLocalPositionY( 0.02f);
	
	auto edges = m_Attribute->GetVertex()->GetEdges();
	if (edges.size() == 0)
	{
		return;
	}

	auto edgeBegin = edges.begin();
	auto roadAttribute = (RoadAttribute*)((*edgeBegin)->GetAttribute());
	float width = roadAttribute->GetWidth();
	m_pTransform->SetLocalScale( width, 1.0f, width);

	//Ｌ字、Ｔ字、十字の設定
	switch (edges.size())
	{
	case 2:		//Ｌ字
	{
		m_Renderer->LoadTexture( "data/TEXTURE/junction_L.png");

		//ベクトルの取得
		Vector3 vec1 = (*edgeBegin)->GetVector();
		if (!(*edgeBegin)->GetRight())
		{
			vec1 *= -1.0f;
		}
		++edgeBegin;
		Vector3 vec2 = (*edgeBegin)->GetVector();
		if (!(*edgeBegin)->GetRight())
		{
			vec2 *= -1.0f;
		}

		//外積に応じて回転
		if (Vector3::Cross(vec1, vec2).y < 0.0f)
		{
			m_pTransform->SetWorldRotationLookDirection( vec1);
		}
		else
		{
			m_pTransform->SetWorldRotationLookDirection( vec2);
		}

		break;
	}

	case 3:		//Ｔ字
	{
		m_Renderer->LoadTexture( "data/TEXTURE/junction_T.png");
		
		//向かい合っている道路がない道路を取得
		Edge* targetEdge = NULL;
		for (auto edge1 : edges)
		{
			bool isTarget = true;

			//他辺と向かい合っているか
			for(auto edge2 : edges)
			{
				if (edge1 != edge2)
				{
					auto dot = Vector3::Dot(edge1->GetVector().Normalize(), edge2->GetVector().Normalize());
					if ( dot < -0.5f || dot > 0.5f)
					{
						isTarget = false;
						break;
					}
				}
			}

			if (isTarget)
			{
				targetEdge = edge1;
				break;
			}
		}

		//Ｔ字に回転
		auto vec = targetEdge->GetVector();
		if (targetEdge->GetStart() == m_Attribute->GetVertex())
		{
			vec *= -1.0f;
		}
		m_pTransform->SetWorldRotationLookDirection( vec);

		break;
	}

	case 4:		//十字
	{
		m_Renderer->LoadTexture( "data/TEXTURE/junction_+.png");


		break;
	}

	default:
		break;
	}
}
