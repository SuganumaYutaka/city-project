/*==============================================================================
	
	TrafficBuilding.cpp - 交通システムー交差点
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "TrafficBuilding.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "CityAttribute.h"

#include "Edge.h"
#include "Vertex.h"
#include "HalfEdge.h"

#include "TrafficRoad.h"
#include "CarController.h"
#include "CarFactory.h"
#include "CityAttribute.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* TrafficBuilding::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<TrafficBuilding>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
TrafficBuilding::TrafficBuilding( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_CarFactory = NULL;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void TrafficBuilding::Uninit( void)
{

}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void TrafficBuilding::Update( void)
{
	

}

/*------------------------------------------------------------------------------
	属性から道路を設定
------------------------------------------------------------------------------*/
void TrafficBuilding::SetRoads(std::list<RoadAttribute*> roadAttributes)
{
	for (auto roadAttribute : roadAttributes)
	{
		m_Roads.push_back( roadAttribute->GetTrafficRoad());
	}
}

/*------------------------------------------------------------------------------
	CarFactoryを設定
------------------------------------------------------------------------------*/
void TrafficBuilding::SetCarFactory(CarFactory* carFactory)
{
	m_CarFactory = carFactory;
}

/*------------------------------------------------------------------------------
	車を生成
------------------------------------------------------------------------------*/
CarController* TrafficBuilding::CreateCar(void)
{
	if (!m_CarFactory)
	{
		return NULL;
	}

	//TODO:

	return NULL;
}

/*------------------------------------------------------------------------------
	車を入庫させる
------------------------------------------------------------------------------*/
void TrafficBuilding::EnterCar(CarController* car)
{
	//TODO:

}

/*------------------------------------------------------------------------------
	車を出庫させる
------------------------------------------------------------------------------*/
CarController* TrafficBuilding::ExitCar(CarController* car)
{
	//TODO:

	return NULL;
}

