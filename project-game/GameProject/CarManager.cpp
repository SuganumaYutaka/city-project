/*==============================================================================
	
	CarManager.cpp - 交通システムー車マネージャー
														Author : Yutaka Suganuma
														Date   : 2017/12/28
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "CarManager.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "CarController.h"
#include "TrafficRoad.h"
#include "TrafficJunction.h"
#include "TrafficBuilding.h"

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* CarManager::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<CarManager>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
CarManager::CarManager( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();


}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void CarManager::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void CarManager::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	すべてのCarControllerを取得
------------------------------------------------------------------------------*/
std::list<CarController*> CarManager::GetAllCarControllers(void)
{
	return m_pGameObject->GetComponentListInChildren<CarController>();
}

/*------------------------------------------------------------------------------
	CarController生成
------------------------------------------------------------------------------*/
CarController* CarManager::CreateCarController(const Vector3& position, const D3DXQUATERNION& rotation
	, TrafficRoad* currentRoad, TrafficJunction* nextJunction, TrafficBuilding* targetBuilding)
{
	auto carObject = new GameObject( m_pGameObject);
	carObject->m_pTransform->SetWorldPosition( position);
	carObject->m_pTransform->SetWorldRotation( rotation);
	auto carController = carObject->AddComponent<CarController>();
	carController->Init( currentRoad, nextJunction, targetBuilding);

	return carController;
}
