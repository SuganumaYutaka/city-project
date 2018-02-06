/*==============================================================================
	
	TrafficLand.cpp - 交通システムー土地
														Author : Yutaka Suganuma
														Date   : 2018/2/3
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "TrafficLand.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "CityAttribute.h"

#include "Edge.h"
#include "Vertex.h"
#include "HalfEdge.h"

#include "TrafficRoad.h"
#include "CarController.h"
#include "CityAttribute.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* TrafficLand::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<TrafficLand>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
TrafficLand::TrafficLand( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void TrafficLand::Init(std::list<RoadAttribute*> roadAttributes)
{
	for (auto roadAttribute : roadAttributes)
	{
		m_Roads.push_back( roadAttribute->GetTrafficRoad());
	}
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void TrafficLand::Uninit( void)
{

}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void TrafficLand::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	車を入庫させる
------------------------------------------------------------------------------*/
void TrafficLand::EnterCar(CarController* car)
{
	if (!IsCarInLand( car))
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
bool TrafficLand::ExitCar(CarController* car)
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
CarController* TrafficLand::PopFrontCar(void)
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
CarController* TrafficLand::PopBackCar(void)
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
bool TrafficLand::IsCarInLand(CarController* car)
{
	for (auto carInLand : m_Cars)
	{
		if (carInLand == car)
		{
			return true;
		}
	}

	return false;
}


