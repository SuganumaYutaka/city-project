/*==============================================================================

    Car.cpp - 町の自動生成ー交通システムー車
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Car.h"
#include "GameObject.h"
#include "CarManager.h"
#include "CarController.h"
#include "TrafficLand.h"
#include "TrafficRoad.h"
#include "TrafficJunction.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Car::Car(CarManager* manager, GameObject* parent)
{
	m_GameObject = new GameObject( parent);
	m_CarController = NULL;
	
	m_Manager = manager;
	m_Manager->RegisterCar( this);
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Car::~Car()
{
	
}

/*------------------------------------------------------------------------------
	消去
------------------------------------------------------------------------------*/
void Car::Delete(void)
{
	m_GameObject->ReleaseReserve();
	m_Manager->UnregisterCar( this);
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void Car::Init(TrafficLand* spawnLand, TrafficLand* targetLand)
{
	if (!m_CarController)
	{
		m_CarController = m_GameObject->AddComponent<CarController>();
	}

	//出現位置から道路と交差点を取得
	auto road = spawnLand->GetTrafficRoad().front();
	auto junction = road->GetJunctions().front();

	//車の初期化
	m_CarController->Init( road, junction, targetLand);
	m_GameObject->m_pTransform->SetWorldPosition( spawnLand->m_pTransform->GetWorldPosition());

}


