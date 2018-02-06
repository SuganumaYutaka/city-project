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

#include "BuildingController.h"

#include "CarController.h"
#include "WallRenderer.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define CITY_WIDTH (200.0f)
#define CITY_HEIGHT (200.0f)
#define DIVIDE_COUNT ( 3)
#define CREATE_CAR_COUNT ( 20)

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

	m_Model = NULL;
	/*m_BuildingRuleFactory = NULL;
	m_BuildingManager = NULL;
	m_CarManager = NULL;*/

	m_Width = CITY_WIDTH;
	m_Height = CITY_HEIGHT;
	m_CountCar = CREATE_CAR_COUNT;
	m_IsWireFrame = false;
	
	//Init( CITY_WIDTH, CITY_HEIGHT, DIVIDE_COUNT, CREATE_CAR_COUNT, true);
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void CityController::Init(float cityWidth, float cityHeight, int countDivide, int countCar, bool doConfirmGeometry)
{
	m_Width = cityWidth;
	m_Height = cityHeight;
	m_CountCar = countCar;

	//車の消去
	DeleteAllCars();

	//終了処理
	Uninit();

	////建物管理オブジェクト生成
	//m_BuildingManager = new BuildingManager();

	////建物の自動生成システムを生成
	//m_BuildingRuleFactory = new BuildingRuleFactory();

	////車管理オブジェクト生成
	//if( !m_CarManager)
	//{
	//	auto carManagerObject = new GameObject( m_pGameObject);
	//	m_CarManager = carManagerObject->AddComponent<CarManager>();
	//}

	////町の自動生成システム設定のハーフエッジデータ構造を生成
	//m_Model = new Model( new CityRule());

	////はじめの面を生成
	//Vector3 sizeHalf( cityWidth * 0.5f, 0.0f, cityHeight * 0.5f);
	//m_Model->CreateFirstFace( 
	//	Vector3( -sizeHalf.x, 0.0f, +sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, +sizeHalf.z),
	//	Vector3( -sizeHalf.x, 0.0f, -sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, -sizeHalf.z));

	////区画分割
	//for( int count = 0; count < countDivide; count++)
	//{
	//	m_Model->DivideAllFaces();
	//}

	////車を生成
	//CreateCars( countCar);

	////建物ジオメトリ情報の削除
	//if( doConfirmGeometry)
	//{
	//	for (auto building : m_BuildingManager->GetAllBuildings())
	//	{
	//		building->ConfirmGeometry();
	//	}
	//}
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void CityController::Uninit( void)
{
	if( m_Model)
	{
		delete m_Model;
		m_Model = NULL;
	}
	/*if (m_BuildingRuleFactory)
	{
		delete m_BuildingRuleFactory;
		m_BuildingRuleFactory = NULL;
	}
	if( m_BuildingManager)
	{
		delete m_BuildingManager;
		m_BuildingManager = NULL;
	}*/
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void CityController::Update()
{
	
}

/*------------------------------------------------------------------------------
	車を生成
------------------------------------------------------------------------------*/
void CityController::CreateCars(int countCar)
{
	if (countCar <= 0)
	{
		return;
	}

	if (!m_BuildingManager)
	{
		return;
	}

	////建物から車を生成
	//auto buildings = m_BuildingManager->GetAllBuildings();
	//if( buildings.size() == 0)
	//{
	//	return;
	//}
	//int parCreate = buildings.size() / countCar;
	//if (parCreate <= 0)
	//{
	//	parCreate = 1;
	//}
	//int count = 0;
	//for (auto ite = buildings.begin(); ite != buildings.end(); ++ite, ++count)
	//{
	//	if (count % parCreate == 0)
	//	{
	//		auto trafficBuilding = (*ite)->GetTrafficBuilding();
	//		trafficBuilding->CreateCar();
	//	}
	//}
}

/*------------------------------------------------------------------------------
	すべての車を消去
------------------------------------------------------------------------------*/
void CityController::DeleteAllCars(void)
{
	/*if (m_CarManager)
	{
		auto cars = m_CarManager->m_pGameObject->GetComponentListInChildren<CarController>();
		for (auto car : cars)
		{
			car->m_pGameObject->ReleaseReserve();
		}
	}*/
}
