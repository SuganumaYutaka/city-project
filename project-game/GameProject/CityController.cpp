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
#include "CityAttributeManager.h"
#include "LandManager.h"
#include "BuildingManager.h"
#include "CarManager.h"

#include "InputKeyboard.h"

#include "Face.h"
#include "FaceDivideFunc.h"
#include "CityAttribute.h"
#include "LandSpawner.h"
#include "LandParameter.h"
#include "BuildingSurfacePattern.h"
#include "BuildingParameterSpawner.h"
#include "Land.h"
#include "Builder.h"
#include "Building.h"
#include "Car.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define CITY_WIDTH (200.0f)
#define CITY_HEIGHT (200.0f)
#define DIVIDE_COUNT ( 3)

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
	m_CityAttributeManager = NULL;
	m_LandManager = NULL;
	m_BuildingManager = NULL;
	m_CarManager = NULL;

	m_Width = CITY_WIDTH;
	m_Height = CITY_HEIGHT;

	//表面パターンの設定
	m_BuildingSurfacePatterns.push_back( new BuildingSurfacePattern("data/SCRIPT/BuildingSurfacePattern/test02.txt"));
	m_BuildingSurfacePatterns.push_back( new BuildingSurfacePattern("data/SCRIPT/BuildingSurfacePattern/test02.txt"));
	
	//Init( CITY_WIDTH, CITY_HEIGHT, DIVIDE_COUNT, CREATE_CAR_COUNT, true);
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void CityController::Init(float cityWidth, float cityHeight, int countDivide, int countCar, bool doConfirmGeometry)
{
	m_Width = cityWidth;
	m_Height = cityHeight;
	
	//終了処理
	//Uninit();

	//管理オブジェクトの生成
	m_Model = new Model();
	m_CityAttributeManager = new CityAttributeManager();
	m_LandManager = new LandManager();
	m_BuildingManager = new BuildingManager();
	m_CarManager = new CarManager();

	//町のはじめの面を生成
	CreateFirstFace( cityWidth, cityHeight);

	//区画の分割
	Divide( countDivide);

	//属性情報の付与
	CreateAttribute();

	//土地の追加
	LandSpawner landSpawner;
	auto blocks = m_CityAttributeManager->GetBlocks();
	for (auto block : blocks)
	{
		//土地生成用のパラメーター生成
		auto landParameters = landSpawner( block, m_CityAttributeManager);
		for (auto landParameter : landParameters)
		{
			CreateLand( landParameter, m_CityAttributeManager->GetBlockID( block));
		}
	}

	//土地情報から建物の生成
	auto lands = m_LandManager->GetLands();
	int landCount = lands.size();
	GeometryParameterSpawner geometryParameterSpawner;
	ShapeParameterSpawner shapeParameterSpawner;
	for (int i = 0; i < landCount; i++)
	{
		//建物生成用のパラメーター生成
		auto parameter = geometryParameterSpawner( m_BuildingSurfacePatterns);
		shapeParameterSpawner( lands[i]->GetVertices(), parameter);

		//パラメーターから建物を生成
		CreateBuilding( parameter, i);
	}

	//土地の最適化
	for (auto building : m_BuildingManager->GetBuildings())
	{
		building->ConfirmGeometry();
	}

	//車の生成
	int parCreate = landCount / countCar;
	if (parCreate <= 0)
	{
		parCreate = 1;
	}
	for( int i = 0; i < landCount; i++)
	{
		if ( i % parCreate == 0)
		{
			auto traffic = lands[i]->GetTrafficLand();
			if( traffic)
			{
				CreateCar( i, -1);
			}
		}
	}
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
	if( m_CityAttributeManager)
	{
		delete m_CityAttributeManager;
		m_CityAttributeManager = NULL;
	}
	if( m_LandManager)
	{
		delete m_LandManager;
		m_LandManager = NULL;
	}
	if( m_BuildingManager)
	{
		delete m_BuildingManager;
		m_BuildingManager = NULL;
	}
	if( m_CarManager)
	{
		delete m_CarManager;
		m_CarManager = NULL;
	}

	for (auto surfacePattern : m_BuildingSurfacePatterns)
	{
		delete surfacePattern;
	}
	m_BuildingSurfacePatterns.clear();
	m_BuildingSurfacePatterns.shrink_to_fit();

}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void CityController::Update()
{
	////1キー 区画の分割
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_1))
	//{

	//}

	////2キー 建物の最適化
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_2))
	//{
	//	for (auto building : m_BuildingManager->GetBuildings())
	//	{
	//		building->ConfirmGeometry();
	//	}
	//}
	//
	////3キー 建物の表示On/Off
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_3))
	//{

	//}
	//
	////4キー 建物のタイルを１枚ずつ表示
	//if (Manager::GetInputKeyboard()->GetKeyPress(DIK_4))
	//{

	//}
	//
	////5キー 車のOn/Off
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_5))
	//{

	//}
	//
	////6キー リセット ※メンテナンス中
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_6))
	//{

	//}
	//
	////7キー 建物のワイヤーフレーム表示On/Off
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_7))
	//{

	//}
}

/*------------------------------------------------------------------------------
	はじめの面を生成
------------------------------------------------------------------------------*/
void CityController::CreateFirstFace(float width, float height)
{
	Vector3 sizeHalf( width * 0.5f, 0.0f, height * 0.5f);
	m_Model->CreateFirstFace( 
		Vector3( -sizeHalf.x, 0.0f, +sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, +sizeHalf.z),
		Vector3( -sizeHalf.x, 0.0f, -sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, -sizeHalf.z));
}

/*------------------------------------------------------------------------------
	区画の分割
------------------------------------------------------------------------------*/
void CityController::Divide(int count)
{
	FaceDivideFunc funcDivide;
	for (int i = 0; i < count; i++)
	{
		auto faces = m_Model->GetFaces();
		for (auto face : faces)
		{
			funcDivide( face);
		}
	}
}

/*------------------------------------------------------------------------------
	属性情報の付与
------------------------------------------------------------------------------*/
void CityController::CreateAttribute(void)
{
	auto vertices = m_Model->GetVertices();
	int vertexCount = vertices.size();
	for (int i = 0; i < vertexCount; i++)
	{
		auto attribute = new JunctionAttribute( m_Model, i, m_CityAttributeManager, m_pGameObject);
	}
	auto edges = m_Model->GetEdges();
	int edgesCount = edges.size();
	for (int i = 0; i < edgesCount; i++)
	{
		auto attribute = new RoadAttribute( m_Model, i, m_CityAttributeManager, m_pGameObject);
	}
	auto faces = m_Model->GetFaces();
	int facesCount = faces.size();
	for (int i = 0; i < facesCount; i++)
	{
		auto attribute = new BlockAttribute( m_Model, i, m_CityAttributeManager, m_pGameObject);
	}

	//属性情報の初期化
	for (auto junction : m_CityAttributeManager->GetJunctions())
	{
		junction->Init();
	}
	for (auto road : m_CityAttributeManager->GetRoads())
	{
		road->Init();
	}
	for (auto block : m_CityAttributeManager->GetBlocks())
	{
		block->Init();
	}
}

/*------------------------------------------------------------------------------
	土地の生成
------------------------------------------------------------------------------*/
void CityController::CreateLand(LandParameter* parameter, int blockID)
{
	//土地の生成
	Land* land = new Land( m_LandManager, m_pGameObject);
	land->Init( parameter->vertices);

	//区画とリンク
	m_CityAttributeManager->GetBlock(blockID)->LinkLand( land);

	//交通システムを設定
	std::list< RoadAttribute*> roadList;
	for (auto id : parameter->roadIDs)
	{
		roadList.push_back( m_CityAttributeManager->GetRoad(id));
	}
	land->SetTraffic( roadList);
}

/*------------------------------------------------------------------------------
	建物の生成
------------------------------------------------------------------------------*/
void CityController::CreateBuilding(GeometryParameter* parameter, int landID)
{
	auto building = new Building( m_BuildingManager, m_pGameObject);
	building->InitGeometry( parameter);
	building->LinkLand( m_LandManager, landID);
}

/*------------------------------------------------------------------------------
	車の生成
------------------------------------------------------------------------------*/
void CityController::CreateCar(int spawnLandID, int targetLandID)
{
	auto car = new Car( m_CarManager, m_pGameObject);
	//car->Init( m_LandManager->GetLand( spawnLandID)->GetTrafficLand(), m_LandManager->GetLand( targetLandID)->GetTrafficLand());
	car->Init( m_LandManager->GetLand( spawnLandID)->GetTrafficLand(), NULL);
}

