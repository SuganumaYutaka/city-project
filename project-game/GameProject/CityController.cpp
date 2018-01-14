/*==============================================================================
	
	CityController.cpp - 町の自動生成ーコントローラ
														Author : Yutaka Suganuma
														Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "CityController.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "HalfEdgeModel.h"
#include "CityRule.h"
#include "CityAttribute.h"

#include "InputKeyboard.h"

#include "BuildingRuleFactory.h"
#include "BuildingManager.h"
#include "CarManager.h"

#include "TrafficBuilding.h"
#include "BuildingController.h"

#include "CarController.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define CITY_WIDTH (200.0f)
#define CITY_HEIGHT (200.0f)
#define DIVIDE_COUNT ( 4)
#define CREATE_CAR_COUNT ( 50)

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* CityController::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<CityController>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
CityController::CityController( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	//建物管理オブジェクト生成
	m_BuildingManager = new BuildingManager();

	//車の管理オブジェクト生成
	auto carManagerObject = new GameObject( m_pGameObject);
	m_CarManager = carManagerObject->AddComponent<CarManager>();

	//建物の自動生成システムを生成
	m_BuildingRuleFactory = new BuildingRuleFactory();

	//町の自動生成システム設定のハーフエッジデータ構造を生成
	auto attributeFactory = new CityAttributeFactory( m_pGameObject, m_BuildingRuleFactory, m_BuildingManager, m_CarManager);
	m_Model = new Model( new CityRule(), attributeFactory);

	//はじめの面を生成
	Vector3 sizeHalf( CITY_WIDTH * 0.5f, 0.0f, CITY_HEIGHT * 0.5f);
	m_Model->CreateFirstFace( 
		Vector3( -sizeHalf.x, 0.0f, +sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, +sizeHalf.z),
		Vector3( -sizeHalf.x, 0.0f, -sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, -sizeHalf.z));

	//区画分割
	for( int count = 0; count < DIVIDE_COUNT; count++)
	{
		m_Model->DivideAllFaces();
	}

	//建物から車を生成
	auto buildings = m_BuildingManager->GetAllBuildings();
	int parCreate = buildings.size() / CREATE_CAR_COUNT;
	int count = 0;
	for (auto ite = buildings.begin(); ite != buildings.end(); ++ite, ++count)
	{
		if (count % parCreate == 0)
		{
			auto trafficBuilding = (*ite)->GetTrafficBuilding();
			trafficBuilding->CreateCar();
		}
	}

	//建物ジオメトリ情報の削除
	for (auto building : m_BuildingManager->GetAllBuildings())
	{
		building->ConfirmGeometry();
	}
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void CityController::Uninit( void)
{
	delete m_Model;
	delete m_BuildingRuleFactory;
	delete m_BuildingManager;
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void CityController::Update()
{
	////区画分割
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE))
	//{
	//	m_Model->DivideAllFaces();
	//}

	////建物から車を生成
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE))
	//{
	//	auto buildings = m_BuildingManager->GetAllBuildings();
	//	int parCreate = buildings.size() / CREATE_CAR_COUNT;
	//	int count = 0;
	//	for (auto ite = buildings.begin(); ite != buildings.end(); ++ite, ++count)
	//	{
	//		if (count % parCreate == 0)
	//		{
	//			auto trafficBuilding = (*ite)->GetTrafficBuilding();
	//			trafficBuilding->CreateCar();
	//		}
	//	}
	//}

	////建物ジオメトリ情報の削除
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE))
	//{
	//	for (auto building : m_BuildingManager->GetAllBuildings())
	//	{
	//		building->ConfirmGeometry();
	//	}
	//}
}
