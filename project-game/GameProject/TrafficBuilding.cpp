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
#include "CityAttribute.h"
#include "CarManager.h"

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

	m_CarManager = NULL;
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void TrafficBuilding::Init(std::list<RoadAttribute*> roadAttributes, CarManager* carManager)
{
	for (auto roadAttribute : roadAttributes)
	{
		m_Roads.push_back( roadAttribute->GetTrafficRoad());
	}
	m_CarManager = carManager;
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
	車を生成
------------------------------------------------------------------------------*/
CarController* TrafficBuilding::CreateCar(void)
{
	if (!m_CarManager)
	{
		return NULL;
	}

	if (m_Roads.size() == 0)
	{
		return NULL;
	}

	//車を生成
	auto road = m_Roads.front();
	auto junction = road->GetJunctions().front();
	auto carController = m_CarManager->CreateCarController( m_pTransform->GetWorldPosition(), m_pTransform->GetWorldRotation()
		,road, junction, this);

	//入庫させる
	//EnterCar( carController);

	//すぐに出庫させる
	road->RegisterCar( carController);

	return carController;
}

/*------------------------------------------------------------------------------
	車を入庫させる
------------------------------------------------------------------------------*/
void TrafficBuilding::EnterCar(CarController* car)
{
	if (!IsCarInBuilding( car))
	{
		//入庫させる
		m_Cars.push_back( car);

		//道路の登録を解除する
		car->GetCurrentRoad()->UnregisterCar( car);
	}
}

/*------------------------------------------------------------------------------
	車を出庫させる
------------------------------------------------------------------------------*/
bool TrafficBuilding::ExitCar(CarController* car)
{
	for (auto ite = m_Cars.begin(); ite != m_Cars.end(); ++ite)
	{
		if (*ite == car)
		{
			//道路に登録する
			car->GetCurrentRoad()->RegisterCar( car);

			//出庫させる
			m_Cars.erase( ite);
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
	先に入れた車を出庫させる
------------------------------------------------------------------------------*/
CarController* TrafficBuilding::PopFrontCar(void)
{
	if (m_Cars.size() == 0)
	{
		return NULL;
	}

	auto car = m_Cars.front();

	//道路に登録する
	car->GetCurrentRoad()->RegisterCar( car);

	//出庫させる
	m_Cars.pop_front();

	return car;
}

/*------------------------------------------------------------------------------
	後に入れた車を出庫させる
------------------------------------------------------------------------------*/
CarController* TrafficBuilding::PopBackCar(void)
{
	if (m_Cars.size() == 0)
	{
		return NULL;
	}
	auto car = m_Cars.back();

	//道路に登録する
	car->GetCurrentRoad()->RegisterCar( car);

	//出庫させる
	m_Cars.pop_back();

	return car;
}

/*------------------------------------------------------------------------------
	特定の車が入庫しているか確認する
------------------------------------------------------------------------------*/
bool TrafficBuilding::IsCarInBuilding(CarController* car)
{
	for (auto carInBuilding : m_Cars)
	{
		if (carInBuilding == car)
		{
			return true;
		}
	}

	return false;
}


