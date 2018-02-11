/*==============================================================================
	
	CityController.cpp - ���̎��������[�R���g���[��
														Author : Yutaka Suganuma
														Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
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
	�}�N����`
------------------------------------------------------------------------------*/
#define CITY_WIDTH (200.0f)
#define CITY_HEIGHT (200.0f)
#define DIVIDE_COUNT ( 4)
#define CAR_COUNT ( 40)

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* CityController::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<CityController>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
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

	//�\�ʃp�^�[���̐ݒ�
	m_BuildingSurfacePatterns.push_back( new BuildingSurfacePattern("data/SCRIPT/BuildingSurfacePattern/test02.txt"));
	m_BuildingSurfacePatterns.push_back( new BuildingSurfacePattern("data/SCRIPT/BuildingSurfacePattern/test02.txt"));
	
	//�葱���̕ۑ��t�H�[�}�b�g�̏�����
	m_ProcedualSaveData.ClearTextStrage();
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void CityController::Init(float cityWidth, float cityHeight, int countDivide, int countCar, bool doConfirmGeometry)
{
	//�葱���̕ۑ��t�H�[�}�b�g�̏�����
	m_ProcedualSaveData.ClearTextStrage();

	m_Width = cityWidth;
	m_Height = cityHeight;
	
	//�Ǘ��I�u�W�F�N�g�̍Đ���
	ResetManagers();

	//���̂͂��߂̖ʂ𐶐�
	CreateFirstFace( cityWidth, cityHeight);

	//���̕���
	Divide( countDivide);

	//�������̕t�^
	CreateAttribute();

	//�y�n�̒ǉ�
	LandSpawner landSpawner;
	auto blocks = m_CityAttributeManager->GetBlocks();
	for (auto block : blocks)
	{
		//�y�n�����p�̃p�����[�^�[����
		auto landParameters = landSpawner( block, m_CityAttributeManager);
		for (auto landParameter : landParameters)
		{
			CreateLand( landParameter, m_CityAttributeManager->GetBlockID( block));
		}
	}

	//�y�n��񂩂猚���̐���
	auto lands = m_LandManager->GetLands();
	int landCount = lands.size();
	GeometryParameterSpawner geometryParameterSpawner;
	ShapeParameterSpawner shapeParameterSpawner;
	for (int i = 0; i < landCount; i++)
	{
		//���������p�̃p�����[�^�[����
		auto parameter = geometryParameterSpawner( m_BuildingSurfacePatterns);
		shapeParameterSpawner( lands[i]->GetVertices(), parameter);

		//�p�����[�^�[���猚���𐶐�
		CreateBuilding( parameter, i);
	}

	//�y�n�̍œK��
	ConfirmBuilding();

	//�Ԃ̐���
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

	//�葱���̕ۑ�
	m_ProcedualSaveData.Save( "data/SCRIPT/Procedual/test.txt");
}

/*------------------------------------------------------------------------------
	�I������
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
	�}�l�[�W���[�̃��Z�b�g
------------------------------------------------------------------------------*/
void CityController::ResetManagers(void)
{
	//�I������
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

	//�Ǘ��I�u�W�F�N�g�̐���
	m_Model = new Model();
	m_CityAttributeManager = new CityAttributeManager( m_pGameObject);
	m_LandManager = new LandManager( m_pGameObject);
	m_BuildingManager = new BuildingManager( m_pGameObject);
	m_CarManager = new CarManager( m_pGameObject);

	//�葱���̕ۑ���������
	m_ProcedualSaveData.ClearTextStrage();
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void CityController::Update()
{
	//�f������

	//1�L�[ ���̕���
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_1))
	{
		Divide( 1);

		//�������̕t�^
		CreateAttribute();
	}

	//2�L�[ �����̍œK��
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_2))
	{
		ConfirmBuilding();
	}
	
	//3�L�[ �����̕\��On/Off
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_3))
	{
		if (m_BuildingManager->GetBuildingCount() <= 0)
		{
			//�y�n�ƌ����̐���
			//�y�n�̒ǉ�
			LandSpawner landSpawner;
			auto blocks = m_CityAttributeManager->GetBlocks();
			for (auto block : blocks)
			{
				//�y�n�����p�̃p�����[�^�[����
				auto landParameters = landSpawner( block, m_CityAttributeManager);
				for (auto landParameter : landParameters)
				{
					CreateLand( landParameter, m_CityAttributeManager->GetBlockID( block));
				}
			}

			//�y�n��񂩂猚���̐���
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

				//���������p�̃p�����[�^�[����
				auto parameter = geometryParameterSpawner( m_BuildingSurfacePatterns);
				shapeParameterSpawner( lands[i]->GetVertices(), parameter);

				//�p�����[�^�[���猚���𐶐�
				CreateBuilding( parameter, i);
			}
		}

		else
		{
			//�y�n�̕\���؂�ւ�
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
	
	//4�L�[ �����̃^�C�����P�����\��
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
	
	//5�L�[ �Ԃ�On/Off
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_5))
	{
		if( m_CarManager->GetCarCount() > 0)
		{
			m_CarManager->Clear();
		}
		else if( m_LandManager->GetLandCount() > 0)
		{
			//�Ԃ̐���
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
	
	//6�L�[ ���Z�b�g
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_6))
	{
		//�S�}�l�[�W���[�����Z�b�g
		ResetManagers();

		//�͂��߂̖ʂ𐶐�
		CreateFirstFace( m_Width, m_Height);

		//�������̕t�^
		CreateAttribute();
	}
	
	//7�L�[ �����̃��C���[�t���[���\��On/Off
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

	//8�L�[ �葱���̕ۑ�
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_8))
	{
		m_ProcedualSaveData.Save( "data/SCRIPT/Procedual/test.txt");
	}

	//9�L�[ �葱���̓ǂݍ���
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_9))
	{
		Load("data/SCRIPT/Procedual/test.txt");
	}
}

/*------------------------------------------------------------------------------
	�͂��߂̖ʂ𐶐�
------------------------------------------------------------------------------*/
void CityController::CreateFirstFace(float width, float height)
{
	//�葱���̕ۑ�
	m_ProcedualSaveData += " CreateFirstFace " + std::to_string( width) + ' ' + std::to_string( height) + "\n";

	Vector3 sizeHalf( width * 0.5f, 0.0f, height * 0.5f);
	m_Model->CreateFirstFace( 
		Vector3( -sizeHalf.x, 0.0f, +sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, +sizeHalf.z),
		Vector3( -sizeHalf.x, 0.0f, -sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, -sizeHalf.z));
}

/*------------------------------------------------------------------------------
	���̕���
------------------------------------------------------------------------------*/
void CityController::Divide(int count)
{
	//�葱���̕ۑ�
	m_ProcedualSaveData += " Divide " + std::to_string( count) + "\n";

	//�������̔j��
	if (m_CityAttributeManager->GetBlockCount() > 0)
	{
		//�Ԃ̔j��
		m_CarManager->Clear();

		//�������̔j��
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
	�������̕t�^
------------------------------------------------------------------------------*/
void CityController::CreateAttribute(void)
{
	//�葱���̕ۑ�
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

	//�������̏�����
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
	�y�n�̐���
------------------------------------------------------------------------------*/
void CityController::CreateLand(LandParameter* parameter, int blockID)
{
	//�葱���̕ۑ�
	m_ProcedualSaveData += " CreateLand " + parameter->Save() + ' ' + std::to_string( blockID) + ' ' + "\n";

	//�y�n�̐���
	auto land = m_LandManager->CreateLand( parameter->m_Vertices);
	
	//���ƃ����N
	if( blockID >= 0)
	{
		land->LinkAttribute( m_CityAttributeManager, blockID);
	}

	//��ʃV�X�e����ݒ�
	std::list< RoadAttribute*> roadList;
	for (auto id : parameter->m_RoadIDs)
	{
		roadList.push_back( m_CityAttributeManager->GetRoad(id));
	}
	land->SetTraffic( roadList);
}

/*------------------------------------------------------------------------------
	�����̐���
------------------------------------------------------------------------------*/
void CityController::CreateBuilding(GeometryParameter* parameter, int landID)
{
	//�葱���̕ۑ�
	m_ProcedualSaveData += " CreateBuilding " + parameter->Save() + ' ' + std::to_string( landID) + ' ' + "\n";

	//�����̐���
	auto surfacePattern = m_BuildingSurfacePatterns[ parameter->m_SurfacePatternID];
	auto building = m_BuildingManager->CreateBuilding( parameter, surfacePattern);
	
	//�y�n�ƃ����N
	if( landID >= 0)
	{
		building->LinkLand( m_LandManager, landID);
	}
}

/*------------------------------------------------------------------------------
	�����̍œK��
------------------------------------------------------------------------------*/
void CityController::ConfirmBuilding(void)
{
	//�葱���̕ۑ�
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
	�Ԃ̐���
------------------------------------------------------------------------------*/
void CityController::CreateCar(int spawnLandID, int targetLandID)
{
	//�葱���̕ۑ�
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
	���[�h
------------------------------------------------------------------------------*/
void CityController::Load(std::string filename)
{
	Text saveData;
	saveData.Load( filename);

	if (!saveData.Load(filename))
	{
		MessageBox( NULL, "CityController�̃��[�h�Ɏ��s", "error", MB_OK);
		return;
	}

	//�Ǘ��I�u�W�F�N�g�̍Đ���
	ResetManagers();

	for (;;)
	{
		auto CurrentWord = saveData.GetWord();

		//�͂��߂̖ʂ𐶐�
		if (saveData.GetWord() == "CreateFirstFace")
		{
			saveData.ForwardPositionToNextWord();
			m_Width = std::stof( saveData.GetWord());
			saveData.ForwardPositionToNextWord();
			m_Height = std::stof( saveData.GetWord());
			CreateFirstFace( m_Width, m_Height);
		}

		//���̕���
		else if (saveData.GetWord() == "Divide")
		{
			saveData.ForwardPositionToNextWord();
			int count = std::stoi( saveData.GetWord());
			
			Divide( count);
		}

		//�������̕t�^
		else if (saveData.GetWord() == "CreateAttribute")
		{
			CreateAttribute();
		}

		//�y�n�̐���
		else if (saveData.GetWord() == "CreateLand")
		{
			saveData.ForwardPositionToNextWord();
			LandParameter parameter;
			parameter.Load( saveData);
			saveData.ForwardPositionToNextWord();
			int blockID = std::stoi( saveData.GetWord());

			CreateLand( &parameter, blockID);
		}

		//�����̐���
		else if (saveData.GetWord() == "CreateBuilding")
		{
			saveData.ForwardPositionToNextWord();
			GeometryParameter parameter;
			parameter.Load( saveData);
			saveData.ForwardPositionToNextWord();
			int landID = std::stoi( saveData.GetWord());

			CreateBuilding( &parameter, landID);
		}

		//�����̍œK��
		else if (saveData.GetWord() == "ConfirmBuilding")
		{
			ConfirmBuilding();
		}
		
		//�Ԃ̐���
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