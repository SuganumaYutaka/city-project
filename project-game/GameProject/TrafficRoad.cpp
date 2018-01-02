/*==============================================================================
	
	TrafficRoad.cpp - 交通システムー交差点
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "TrafficRoad.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "CityAttribute.h"

#include "Edge.h"
#include "Vertex.h"
#include "HalfEdge.h"

#include "TrafficJunction.h"
#include "CarController.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* TrafficRoad::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<TrafficRoad>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
TrafficRoad::TrafficRoad( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Attribute = NULL;
	m_IsUpdatedAttribute = false;

	m_LeftSideCars.clear();
	m_RightSideCars.clear();
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void TrafficRoad::Uninit( void)
{
	m_Attribute->UnregisterView();
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void TrafficRoad::Update( void)
{
	if (m_IsUpdatedAttribute)
	{

		m_IsUpdatedAttribute = false;
	}

	

}

/*------------------------------------------------------------------------------
	属性情報の設定
------------------------------------------------------------------------------*/
void TrafficRoad::SetAttribute( RoadAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;
	m_IsUpdatedAttribute = true;
}

/*------------------------------------------------------------------------------
	車の登録
------------------------------------------------------------------------------*/
void TrafficRoad::RegisterCar(CarController* car)
{
	UnregisterCar( car);

	//次に到達する交差点の情報から左右のどちらを通行するか決定
	if (car->GetNextJunction()->GetAttribute()->GetVertex() == m_Attribute->GetEdge()->GetEnd())
	{
		//始点から見て左車線に挿入
		if (m_LeftSideCars.size() == 0)
		{
			m_LeftSideCars.push_back( car);
			return;
		}

		//挿入地点の始点からの距離（進んだ距離）に応じて途中に挿入
		float insertRate = car->CulcMoveRateOnRoad();
		for (auto ite = m_LeftSideCars.begin(); ite != m_LeftSideCars.end(); ++ite)
		{
			float rate = (*ite)->CulcMoveRateOnRoad();
			if (insertRate <= rate)
			{
				m_LeftSideCars.insert( ite, car);
				return;
			}
		}

		//最後尾に挿入
		m_LeftSideCars.push_back( car);
		return;
	}
	else
	{
		//始点から見て右車線に挿入
		if (m_RightSideCars.size() == 0)
		{
			m_RightSideCars.push_back( car);
			return;
		}

		//挿入地点の始点からの距離（進んだ距離）に応じて途中に挿入
		float insertRate = car->CulcMoveRateOnRoad();
		for (auto ite = m_RightSideCars.begin(); ite != m_RightSideCars.end(); ++ite)
		{
			float rate = (*ite)->CulcMoveRateOnRoad();
			if (insertRate <= rate)
			{
				m_RightSideCars.insert( ite, car);
				return;
			}
		}

		//最後尾に挿入
		m_RightSideCars.push_back( car);
		return;
	}
}
	
/*------------------------------------------------------------------------------
	車の登録を解除
------------------------------------------------------------------------------*/
void TrafficRoad::UnregisterCar(CarController* car)
{
	for (auto ite = m_LeftSideCars.begin(); ite != m_LeftSideCars.end(); ++ite)
	{
		if (*ite == car)
		{
			m_LeftSideCars.erase( ite);
			return;
		}
	}
	for (auto ite = m_RightSideCars.begin(); ite != m_RightSideCars.end(); ++ite)
	{
		if (*ite == car)
		{
			m_RightSideCars.erase( ite);
			return;
		}
	}
}

/*------------------------------------------------------------------------------
	車の取得
------------------------------------------------------------------------------*/
const std::list<CarController*>& TrafficRoad::GetCars(TrafficJunction* nextJunction)
{
	//次に到達する交差点の情報から左右のどちらを通行するか決定
	if (nextJunction->GetAttribute()->GetVertex() == m_Attribute->GetEdge()->GetEnd())
	{
		return m_LeftSideCars;
	}
	return m_RightSideCars;
}

/*------------------------------------------------------------------------------
	交差点の取得
------------------------------------------------------------------------------*/
std::list<TrafficJunction*> TrafficRoad::GetJunctions(void)
{
	std::list<TrafficJunction*> junctions;
	JunctionAttribute* junctionAttribute = (JunctionAttribute*)m_Attribute->GetEdge()->GetStart()->GetAttribute();
	junctions.push_back( junctionAttribute->GetTrafficJunction());
	junctionAttribute = (JunctionAttribute*)m_Attribute->GetEdge()->GetEnd()->GetAttribute();
	junctions.push_back( junctionAttribute->GetTrafficJunction());

	return junctions;
}

/*------------------------------------------------------------------------------
	反対側の交差点の取得
------------------------------------------------------------------------------*/
TrafficJunction* TrafficRoad::GetOtherJunction(TrafficJunction* junction)
{
	JunctionAttribute* junctionAttribute = (JunctionAttribute*)m_Attribute->GetEdge()->GetStart()->GetAttribute();
	auto startJunction = junctionAttribute->GetTrafficJunction();
	junctionAttribute = (JunctionAttribute*)m_Attribute->GetEdge()->GetEnd()->GetAttribute();
	auto endJunction = junctionAttribute->GetTrafficJunction();

	if (startJunction == junction)
	{
		return endJunction;
	}
	else if (endJunction == junction)
	{
		return startJunction;
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	ベクトルの取得
------------------------------------------------------------------------------*/
Vector3 TrafficRoad::GetVector(TrafficJunction* nextJunction)
{
	JunctionAttribute* junctionAttribute = (JunctionAttribute*)m_Attribute->GetEdge()->GetStart()->GetAttribute();
	auto startJunction = junctionAttribute->GetTrafficJunction();

	if (nextJunction == startJunction)
	{
		return m_Attribute->GetVector() * -1.0f;
	}
	
	return m_Attribute->GetVector();
}

