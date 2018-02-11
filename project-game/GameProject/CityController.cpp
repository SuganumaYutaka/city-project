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
#include "WallRenderer.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define CITY_WIDTH (200.0f)
#define CITY_HEIGHT (200.0f)
#define DIVIDE_COUNT ( 4)
#define CAR_COUNT ( 40)

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
	m_IsWireFrame = false;

	//表面パターンの設定
	m_BuildingSurfacePatterns.push_back( new BuildingSurfacePattern("data/SCRIPT/BuildingSurfacePattern/test02.txt"));
	m_BuildingSurfacePatterns.push_back( new BuildingSurfacePattern("data/SCRIPT/BuildingSurfacePattern/test02.txt"));
	
	//手続きの保存フォーマットの初期化
	m_ProcedualSaveData.ClearTextStrage();
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void CityController::Init(float cityWidth, float cityHeight, int countDivide, int countCar, bool doConfirmGeometry)
{
	//手続きの保存フォーマットの初期化
	m_ProcedualSaveData.ClearTextStrage();

	m_Width = cityWidth;
	m_Height = cityHeight;
	
	//管理オブジェクトの再生成
	ResetManagers();

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
	ConfirmBuilding();

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

	//手続きの保存
	m_ProcedualSaveData.Save( "data/SCRIPT/Procedual/test.txt");
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
	マネージャーのリセット
------------------------------------------------------------------------------*/
void CityController::ResetManagers(void)
{
	//終了処理
	if( m_Model)
	{
		delete m_Model;
		m_Model = NULL;
	}
	if( m_CityAttributeManager)
	{
		m_CityAttributeManager->DeleteGameObject();
		delete m_CityAttributeManager;
		m_CityAttributeManager = NULL;
	}
	if( m_LandManager)
	{
		m_LandManager->DeleteGameObject();
		delete m_LandManager;
		m_LandManager = NULL;
	}
	if( m_BuildingManager)
	{
		m_BuildingManager->DeleteGameObject();
		delete m_BuildingManager;
		m_BuildingManager = NULL;
	}
	if( m_CarManager)
	{
		m_CarManager->DeleteGameObject();
		delete m_CarManager;
		m_CarManager = NULL;
	}

	//管理オブジェクトの生成
	m_Model = new Model();
	m_CityAttributeManager = new CityAttributeManager( m_pGameObject);
	m_LandManager = new LandManager( m_pGameObject);
	m_BuildingManager = new BuildingManager( m_pGameObject);
	m_CarManager = new CarManager( m_pGameObject);

	//手続きの保存を初期化
	m_ProcedualSaveData.ClearTextStrage();
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void CityController::Update()
{
	//デモ操作

	//1キー 区画の分割
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_1))
	{
		Divide( 1);

		//属性情報の付与
		CreateAttribute();
	}

	//2キー 建物の最適化
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_2))
	{
		ConfirmBuilding();
	}
	
	//3キー 建物の表示On/Off
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_3))
	{
		if (m_BuildingManager->GetBuildingCount() <= 0)
		{
			//土地と建物の生成
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
				if (!lands[i])
				{
					continue;
				}

				//建物生成用のパラメーター生成
				auto parameter = geometryParameterSpawner( m_BuildingSurfacePatterns);
				shapeParameterSpawner( lands[i]->GetVertices(), parameter);

				//パラメーターから建物を生成
				CreateBuilding( parameter, i);
			}
		}

		else
		{
			//土地の表示切り替え
			for (auto building : m_BuildingManager->GetBuildings())
			{
				if (!building)
				{
					continue;
				}
				bool isAppear = true;
				auto walls = building->GetGameObject()->GetComponentList<WallRenderer>();
				for (auto wall : walls)
				{
					isAppear = wall->ChangeRenderPolygon();
				}

				auto roofs = building->GetGameObject()->GetComponentListInChildren<Polygon3DRenderer>();
				for( auto roof : roofs)
				{
					roof->SetActive( isAppear);
				}
			}
		}
	}
	
	//4キー 建物のタイルを１枚ずつ表示
	if (Manager::GetInputKeyboard()->GetKeyPress(DIK_4))
	{
		for (auto building : m_BuildingManager->GetBuildings())
		{
			if (!building)
			{
				continue;
			}
			bool isAppear = true;
			auto walls = building->GetGameObject()->GetComponentList<WallRenderer>();
			for( auto wall : walls)
			{
				wall->AddRenderPolygon();
				wall->AddRenderPolygon();
				if (!wall->AddRenderPolygon())
				{
					isAppear = false;
				}
			}
			if ( isAppear)
			{
				auto roofs = building->GetGameObject()->GetComponentListInChildren<Polygon3DRenderer>();
				for( auto roof : roofs)
				{
					roof->SetActive( true);
				}
			}
		}
	}
	
	//5キー 車のOn/Off
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_5))
	{
		if( m_CarManager->GetCarCount() > 0)
		{
			m_CarManager->Clear();
		}
		else if( m_LandManager->GetLandCount() > 0)
		{
			//車の生成
			auto lands = m_LandManager->GetLands();
			int landCount = m_LandManager->GetLandCount();
			int parCreate = landCount / CAR_COUNT;
			landCount = lands.size();
			if (parCreate <= 0)
			{
				parCreate = 1;
			}
			for( int i = 0; i < landCount; i++)
			{
				if (!lands[i])
				{
					continue;
				}

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
	}
	
	//6キー リセット
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_6))
	{
		//全マネージャーをリセット
		ResetManagers();

		//はじめの面を生成
		CreateFirstFace( m_Width, m_Height);

		//属性情報の付与
		CreateAttribute();
	}
	
	//7キー 建物のワイヤーフレーム表示On/Off
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_7))
	{
		m_IsWireFrame ? m_IsWireFrame = false : m_IsWireFrame = true;

		auto buildings = m_BuildingManager->GetBuildings();
		for (auto building : buildings)
		{
			if (!building)
			{
				continue;
			}

			auto wall = building->GetGameObject()->GetComponent<WallRenderer>();
			wall->ChangeWireFrame( m_IsWireFrame);
		}
	}

	//8キー 手続きの保存
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_8))
	{
		m_ProcedualSaveData.Save( "data/SCRIPT/Procedual/test.txt");
	}

	//9キー 手続きの読み込み
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_9))
	{
		Load("data/SCRIPT/Procedual/test.txt");
	}
}

/*------------------------------------------------------------------------------
	はじめの面を生成
------------------------------------------------------------------------------*/
void CityController::CreateFirstFace(float width, float height)
{
	//手続きの保存
	m_ProcedualSaveData += " CreateFirstFace " + std::to_string( width) + ' ' + std::to_string( height) + "\n";

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
	//手続きの保存
	m_ProcedualSaveData += " Divide " + std::to_string( count) + "\n";

	//属性情報の破棄
	if (m_CityAttributeManager->GetBlockCount() > 0)
	{
		//車の破棄
		m_CarManager->Clear();

		//属性情報の破棄
		m_CityAttributeManager->Clear();
	}

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
	//手続きの保存
	m_ProcedualSaveData += " CreateAttribute\n";

	auto vertices = m_Model->GetVertices();
	int vertexCount = vertices.size();
	for (int i = 0; i < vertexCount; i++)
	{
		m_CityAttributeManager->CreateJunctionAttribute( m_Model, i);
	}
	auto edges = m_Model->GetEdges();
	int edgesCount = edges.size();
	for (int i = 0; i < edgesCount; i++)
	{
		m_CityAttributeManager->CreateRoadAttribute( m_Model, i);
	}
	auto faces = m_Model->GetFaces();
	int facesCount = faces.size();
	for (int i = 0; i < facesCount; i++)
	{
		m_CityAttributeManager->CreateBlockAttribute( m_Model, i);
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
	//手続きの保存
	m_ProcedualSaveData += " CreateLand " + parameter->Save() + ' ' + std::to_string( blockID) + ' ' + "\n";

	//土地の生成
	auto land = m_LandManager->CreateLand( parameter->m_Vertices);
	
	//区画とリンク
	if( blockID >= 0)
	{
		land->LinkAttribute( m_CityAttributeManager, blockID);
	}

	//交通システムを設定
	std::list< RoadAttribute*> roadList;
	for (auto id : parameter->m_RoadIDs)
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
	//手続きの保存
	m_ProcedualSaveData += " CreateBuilding " + parameter->Save() + ' ' + std::to_string( landID) + ' ' + "\n";

	//建物の生成
	auto surfacePattern = m_BuildingSurfacePatterns[ parameter->m_SurfacePatternID];
	auto building = m_BuildingManager->CreateBuilding( parameter, surfacePattern);
	
	//土地とリンク
	if( landID >= 0)
	{
		building->LinkLand( m_LandManager, landID);
	}
}

/*------------------------------------------------------------------------------
	建物の最適化
------------------------------------------------------------------------------*/
void CityController::ConfirmBuilding(void)
{
	//手続きの保存
	m_ProcedualSaveData += " ConfirmBuilding\n";

	for (auto building : m_BuildingManager->GetBuildings())
	{
		if( building)
		{
			building->ConfirmGeometry();
		}
	}
}

/*------------------------------------------------------------------------------
	車の生成
------------------------------------------------------------------------------*/
void CityController::CreateCar(int spawnLandID, int targetLandID)
{
	//手続きの保存
	m_ProcedualSaveData += " CreateCar " + std::to_string( spawnLandID) + ' ' + std::to_string( targetLandID) + ' ' + "\n";

	auto car = m_CarManager->CreateCar();
	if( targetLandID >= 0)
	{
		car->Init( m_LandManager->GetLand( spawnLandID)->GetTrafficLand(), m_LandManager->GetLand( targetLandID)->GetTrafficLand());
	}
	else
	{
		car->Init( m_LandManager->GetLand( spawnLandID)->GetTrafficLand(), NULL);
	}
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void CityController::Load(std::string filename)
{
	Text saveData;
	saveData.Load( filename);

	if (!saveData.Load(filename))
	{
		MessageBox( NULL, "CityControllerのロードに失敗", "error", MB_OK);
		return;
	}

	//管理オブジェクトの再生成
	ResetManagers();

	for (;;)
	{
		auto CurrentWord = saveData.GetWord();

		//はじめの面を生成
		if (saveData.GetWord() == "CreateFirstFace")
		{
			saveData.ForwardPositionToNextWord();
			m_Width = std::stof( saveData.GetWord());
			saveData.ForwardPositionToNextWord();
			m_Height = std::stof( saveData.GetWord());
			CreateFirstFace( m_Width, m_Height);
		}

		//区画の分割
		else if (saveData.GetWord() == "Divide")
		{
			saveData.ForwardPositionToNextWord();
			int count = std::stoi( saveData.GetWord());
			
			Divide( count);
		}

		//属性情報の付与
		else if (saveData.GetWord() == "CreateAttribute")
		{
			CreateAttribute();
		}

		//土地の生成
		else if (saveData.GetWord() == "CreateLand")
		{
			saveData.ForwardPositionToNextWord();
			LandParameter parameter;
			parameter.Load( saveData);
			saveData.ForwardPositionToNextWord();
			int blockID = std::stoi( saveData.GetWord());

			CreateLand( &parameter, blockID);
		}

		//建物の生成
		else if (saveData.GetWord() == "CreateBuilding")
		{
			saveData.ForwardPositionToNextWord();
			GeometryParameter parameter;
			parameter.Load( saveData);
			saveData.ForwardPositionToNextWord();
			int landID = std::stoi( saveData.GetWord());

			CreateBuilding( &parameter, landID);
		}

		//建物の最適化
		else if (saveData.GetWord() == "ConfirmBuilding")
		{
			ConfirmBuilding();
		}
		
		//車の生成
		else if (saveData.GetWord() == "CreateCar")
		{
			saveData.ForwardPositionToNextWord();
			int spawnLandID = std::stoi( saveData.GetWord());
			saveData.ForwardPositionToNextWord();
			int targetLandID = std::stoi( saveData.GetWord());

			CreateCar( spawnLandID, targetLandID);
		}

		if (saveData.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}
}