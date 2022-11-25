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
#include "CarController.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define ONCOMING_CAR_MAX_NEAR_DISTANCE ( 5.0f)		//対向車と交差点の最接近距離（これより近いと対向車ありとみなす）
#define TRAFFIC_CHANGE_COUNT (120)
#define TRAFFIC_WAIT_COUNT (30)

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
	m_CanMoveVertical = true;
	m_CanMoveHorizontal = true;
	m_TrafficCount = 0;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void TrafficJunction::Uninit( void)
{
	
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

	//信号機の更新
	if (m_Roads.size() <= 2)
	{
		m_CanMoveVertical = true;
		m_CanMoveHorizontal = true;
	}
	else
	{
		m_TrafficCount = (m_TrafficCount + 1) % (TRAFFIC_CHANGE_COUNT * 2 + TRAFFIC_WAIT_COUNT * 2);
		if (m_TrafficCount < TRAFFIC_WAIT_COUNT)
		{
			m_CanMoveHorizontal = false;
			m_CanMoveVertical = false;
		}
		else if (m_TrafficCount < TRAFFIC_CHANGE_COUNT + TRAFFIC_WAIT_COUNT)
		{
			m_CanMoveHorizontal = false;
			m_CanMoveVertical = true;
		}
		else if (m_TrafficCount < TRAFFIC_CHANGE_COUNT + TRAFFIC_WAIT_COUNT * 2)
		{
			m_CanMoveHorizontal = false;
			m_CanMoveVertical = false;
		}
		else
		{
			m_CanMoveHorizontal = true;
			m_CanMoveVertical = false;
		}
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
	UpdateJunction();
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
	右にある道路を取得
------------------------------------------------------------------------------*/
TrafficRoad* TrafficJunction::GetRightRoad(TrafficRoad* currentRoad)
{
	auto currentBranch = CheckBranch( currentRoad);
	int size = m_Roads.size();
	switch (currentBranch)
	{
	case eJunctionBranchBack:
		return m_Roads[ eJunctionBranchRight];
		break;

	case eJunctionBranchRight:
		//L字、T字は道路がない
		if (size != 4)
		{
			return NULL;
		}
		return m_Roads[ eJunctionBranchFront];
		break;

	case eJunctionBranchLeft:
		return m_Roads[ eJunctionBranchBack];
		break;

	case eJunctionBranchFront:
		return m_Roads[ eJunctionBranchLeft];
		break;

	default:
		break;
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	左にある道路を取得
------------------------------------------------------------------------------*/
TrafficRoad* TrafficJunction::GetLeftRoad(TrafficRoad* currentRoad)
{
	auto currentBranch = CheckBranch( currentRoad);
	int size = m_Roads.size();
	switch (currentBranch)
	{
	case eJunctionBranchBack:
		//L字は道路がない
		if (size == 2)
		{
			return NULL;
		}
		return m_Roads[ eJunctionBranchLeft];
		break;

	case eJunctionBranchRight:
		return m_Roads[ eJunctionBranchBack];
		break;

	case eJunctionBranchLeft:
		//L字、T字は道路がない
		if (size != 4)
		{
			return NULL;
		}
		return m_Roads[ eJunctionBranchFront];
		break;

	case eJunctionBranchFront:
		return m_Roads[ eJunctionBranchRight];
		break;

	default:
		break;
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	直進する道路を取得
------------------------------------------------------------------------------*/
TrafficRoad* TrafficJunction::GetStraightRoad(TrafficRoad* currentRoad)
{
	auto currentBranch = CheckBranch( currentRoad);
	int size = m_Roads.size();
	switch (currentBranch)
	{
	case eJunctionBranchBack:
		//L字、T字は道路がない
		if (size != 4)
		{
			return NULL;
		}
		return m_Roads[ eJunctionBranchFront];
		break;

	case eJunctionBranchRight:
		//L字は道路がない
		if (size == 2)
		{
			return NULL;
		}
		return m_Roads[ eJunctionBranchLeft];
		break;

	case eJunctionBranchLeft:
		return m_Roads[ eJunctionBranchRight];
		break;

	case eJunctionBranchFront:
		return m_Roads[ eJunctionBranchBack];
		break;

	default:
		break;
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	右折できるか
------------------------------------------------------------------------------*/
bool TrafficJunction::CanTurnRight(TrafficRoad* currentRoad)
{
	//現在のブランチをチェック
	auto currentBranch = CheckBranch( currentRoad);

	//信号を確認
	if (!CanMoveTraffic(currentBranch))
	{
		return false;
	}

	//曲がる先に道があるか確認
	if (!GetRightRoad(currentRoad))
	{
		return false;
	}

	//対向車がないか確認
	int size = m_Roads.size();
	TrafficRoad* oncomingCar = NULL;
	switch (currentBranch)
	{
	case eJunctionBranchBack:
		//対向車を設定
		if (size == 4)
		{
			oncomingCar = m_Roads[ eJunctionBranchFront];
		}
		break;

	case eJunctionBranchRight:
		//対向車を設定
		oncomingCar = m_Roads[ eJunctionBranchLeft];
		break;

	case eJunctionBranchLeft:
		//対向車を設定
		oncomingCar = m_Roads[ eJunctionBranchRight];
		break;

	case eJunctionBranchFront:
		//対向車を設定
		oncomingCar = m_Roads[ eJunctionBranchBack];
		break;

	default:
		break;
	}

	//対向車が交差点近くにいないか確認
	if (CheckOncomingCar(oncomingCar))
	{
		return false;
	}

	//通行可能
	return true;
}

/*------------------------------------------------------------------------------
	左折できるか
------------------------------------------------------------------------------*/
bool TrafficJunction::CanTurnLeft(TrafficRoad* currentRoad)
{
	//現在のブランチをチェック
	auto currentBranch = CheckBranch( currentRoad);

	//信号を確認
	if (!CanMoveTraffic(currentBranch))
	{
		return false;
	}

	//曲がる先に道があるか確認
	if (!GetLeftRoad(currentRoad))
	{
		return false;
	}

	//通行可能
	return true;
}

/*------------------------------------------------------------------------------
	直進できるか
------------------------------------------------------------------------------*/
bool TrafficJunction::CanGoStraight(TrafficRoad* currentRoad)
{
	//現在のブランチをチェック
	auto currentBranch = CheckBranch( currentRoad);

	//信号を確認
	if (!CanMoveTraffic(currentBranch))
	{
		return false;
	}

	//直進先に道があるか確認
	if (!GetStraightRoad(currentRoad))
	{
		return false;
	}

	//通行可能
	return true;
}

/*------------------------------------------------------------------------------
	位置を取得
------------------------------------------------------------------------------*/
const Vector3& TrafficJunction::GetPosition(void)
{
	return m_Attribute->GetPosition();
}

/*------------------------------------------------------------------------------
	道路がどのブランチに当たるかチェック
------------------------------------------------------------------------------*/
ETrafficJunctionBranch TrafficJunction::CheckBranch(TrafficRoad* road)
{
	if (m_Roads[eJunctionBranchBack] == road)
	{
		return eJunctionBranchBack;
	}
	if (m_Roads[eJunctionBranchRight] == road)
	{
		return eJunctionBranchRight;
	}
	if (m_Roads[eJunctionBranchLeft] == road)
	{
		return eJunctionBranchLeft;
	}
	
	return eJunctionBranchFront;
}

/*------------------------------------------------------------------------------
	信号を確認
------------------------------------------------------------------------------*/
bool TrafficJunction::CanMoveTraffic(ETrafficJunctionBranch currentBranch)
{
	if (currentBranch == eJunctionBranchBack || currentBranch == eJunctionBranchFront)
	{
		if (!m_CanMoveVertical)
		{
			return false;
		}
	}
	else if (currentBranch == eJunctionBranchRight || currentBranch == eJunctionBranchLeft)
	{
		if (!m_CanMoveHorizontal)
		{
			return false;
		}
	}

	return true;
}

/*------------------------------------------------------------------------------
	対向車がいるか確認
------------------------------------------------------------------------------*/
bool TrafficJunction::CheckOncomingCar(TrafficRoad* oncomingRoad)
{
	if (!oncomingRoad)
	{
		return false;
	}

	//対向車線の先頭車の取得
	auto oncomingCars = oncomingRoad->GetCars( this);
	if (oncomingCars.size() == 0)
	{
		return false;
	}
	auto car = oncomingCars.back();

	//対向車が交差点に近いか確認
	float distance = car->CulcRemainDistanceOnRoad();
	if ( distance > ONCOMING_CAR_MAX_NEAR_DISTANCE)
	{
		return false;
	}

	//対向車あり
	return true;
}
