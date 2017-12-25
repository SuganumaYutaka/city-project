/*==============================================================================
	
	TrafficJunction.cpp - 交通システムー交差点
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "TrafficJunction.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "CityAttribute.h"

#include "Edge.h"
#include "Vertex.h"
#include "HalfEdge.h"

#include "TrafficRoad.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* TrafficJunction::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<TrafficJunction>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
TrafficJunction::TrafficJunction( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Attribute = NULL;
	m_IsUpdatedAttribute = false;

	m_Roads.clear();
	m_CanMoveVertical = false;
	m_CanMoveHorizontal = false;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void TrafficJunction::Uninit( void)
{
	m_Attribute->UnregisterView();
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void TrafficJunction::Update( void)
{
	if (m_IsUpdatedAttribute)
	{
		UpdateJunction();

		m_IsUpdatedAttribute = false;
	}

	

}

/*------------------------------------------------------------------------------
	属性情報の設定
------------------------------------------------------------------------------*/
void TrafficJunction::SetAttribute( JunctionAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;
	m_IsUpdatedAttribute = true;
}

/*------------------------------------------------------------------------------
	交差点情報の更新
------------------------------------------------------------------------------*/
void TrafficJunction::UpdateJunction(void)
{
	auto edges = m_Attribute->GetVertex()->GetEdges();
	if (edges.size() <= 0)
	{
		return;
	}
	
	//Ｌ字、Ｔ字、十字の設定
	RoadAttribute* roadAttribute = NULL;

	switch (edges.size())
	{
	case 2:		//Ｌ字
	{
		m_Roads.resize( 2);

		//ベクトルの取得
		auto edgeBegin = edges.begin();
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

		//外積に応じて設定
		edgeBegin = edges.begin();
		if (Vector3::Cross(vec1, vec2).y < 0.0f)
		{
			roadAttribute = (RoadAttribute*)((*edgeBegin)->GetAttribute());
			m_Roads[ eJunctionBranchBack] = roadAttribute->GetTrafficRoad();
			++edgeBegin;
			roadAttribute = (RoadAttribute*)((*edgeBegin)->GetAttribute());
			m_Roads[ eJunctionBranchRight] = roadAttribute->GetTrafficRoad();
		}
		else
		{
			roadAttribute = (RoadAttribute*)((*edgeBegin)->GetAttribute());
			m_Roads[ eJunctionBranchRight] = roadAttribute->GetTrafficRoad();
			++edgeBegin;
			roadAttribute = (RoadAttribute*)((*edgeBegin)->GetAttribute());
			m_Roads[ eJunctionBranchBack] = roadAttribute->GetTrafficRoad();
		}
		
		break;
	}

	case 3:		//Ｔ字
	{
		m_Roads.resize( 3);

		//向かい合っている道路がない道路(Back)を取得
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
		roadAttribute = (RoadAttribute*)(targetEdge->GetAttribute());
		m_Roads[ eJunctionBranchBack] = roadAttribute->GetTrafficRoad();

		//外積から左右の道路を設定
		Vector3 vec1 = targetEdge->GetVector();
		if ( targetEdge->GetEnd() == m_Attribute->GetVertex())
		{
			vec1 *= -1.0f;
		}
		for (auto edge : edges)
		{
			if (edge == targetEdge)
			{
				continue;
			}

			Vector3 vec2 = edge->GetVector();
			if ( edge->GetEnd() == m_Attribute->GetVertex())
			{
				vec2 *= -1.0f;
			}
			
			if (Vector3::Cross(vec1, vec2).y > 0.0f)
			{
				roadAttribute = (RoadAttribute*)(edge->GetAttribute());
				m_Roads[ eJunctionBranchLeft] = roadAttribute->GetTrafficRoad();
			}
			else
			{
				roadAttribute = (RoadAttribute*)(edge->GetAttribute());
				m_Roads[ eJunctionBranchRight] = roadAttribute->GetTrafficRoad();
			}
		}

		break;
	}

	case 4:		//十字
	{
		m_Roads.resize( 4);

		//beginを基準に内積と外積で4方向に設定
		auto edgeBegin = edges.begin();
		roadAttribute = (RoadAttribute*)((*edgeBegin)->GetAttribute());
		m_Roads[ eJunctionBranchBack] = roadAttribute->GetTrafficRoad();
		Vector3 vec1 = (*edgeBegin)->GetVector();
		if ( (*edgeBegin)->GetEnd() == m_Attribute->GetVertex())
		{
			vec1 *= -1.0f;
		}
		for (auto edge : edges)
		{
			if (edge == (*edgeBegin))
			{
				continue;
			}

			Vector3 vec2 = edge->GetVector();
			if ( edge->GetEnd() == m_Attribute->GetVertex())
			{
				vec2 *= -1.0f;
			}
			
			//内積で反対側
			auto dot = Vector3::Dot( vec1.Normalize(), vec2.Normalize());
			if (dot < -0.5f)
			{
				roadAttribute = (RoadAttribute*)(edge->GetAttribute());
				m_Roads[ eJunctionBranchFront] = roadAttribute->GetTrafficRoad();
				continue;
			}

			//外積で左右
			if (Vector3::Cross(vec1, vec2).y > 0.0f)
			{
				roadAttribute = (RoadAttribute*)(edge->GetAttribute());
				m_Roads[ eJunctionBranchLeft] = roadAttribute->GetTrafficRoad();
			}
			else
			{
				roadAttribute = (RoadAttribute*)(edge->GetAttribute());
				m_Roads[ eJunctionBranchRight] = roadAttribute->GetTrafficRoad();
			}
		}

		break;
	}

	default:
		break;
	}
}

/*------------------------------------------------------------------------------
	右折できるか
------------------------------------------------------------------------------*/
bool TrafficJunction::CanTurnRight(TrafficRoad* road)
{
	//TODO:フラグと対向車をチェック

	return true;
}

/*------------------------------------------------------------------------------
	左折できるか
------------------------------------------------------------------------------*/
bool TrafficJunction::CanTurnLeft(TrafficRoad* road)
{
	//TODO:フラグと対向車をチェック

	return true;
}

/*------------------------------------------------------------------------------
	直進できるか
------------------------------------------------------------------------------*/
bool TrafficJunction::CanGoStraight(TrafficRoad* road)
{
	//TODO:フラグと対向車をチェック

	return true;
}
