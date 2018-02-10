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

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define CITY_WIDTH (200.0f)
#define CITY_HEIGHT (200.0f)
#define DIVIDE_COUNT ( 3)

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

	//�\�ʃp�^�[���̐ݒ�
	m_BuildingSurfacePatterns.push_back( new BuildingSurfacePattern("data/SCRIPT/BuildingSurfacePattern/test02.txt"));
	m_BuildingSurfacePatterns.push_back( new BuildingSurfacePattern("data/SCRIPT/BuildingSurfacePattern/test02.txt"));
	
	//Init( CITY_WIDTH, CITY_HEIGHT, DIVIDE_COUNT, CREATE_CAR_COUNT, true);
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void CityController::Init(float cityWidth, float cityHeight, int countDivide, int countCar, bool doConfirmGeometry)
{
	m_Width = cityWidth;
	m_Height = cityHeight;
	
	//�I������
	//Uninit();

	//�Ǘ��I�u�W�F�N�g�̐���
	m_Model = new Model();
	m_CityAttributeManager = new CityAttributeManager();
	m_LandManager = new LandManager();
	m_BuildingManager = new BuildingManager();
	m_CarManager = new CarManager();

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
	for (auto building : m_BuildingManager->GetBuildings())
	{
		building->ConfirmGeometry();
	}

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
	�X�V
------------------------------------------------------------------------------*/
void CityController::Update()
{
	////1�L�[ ���̕���
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_1))
	//{

	//}

	////2�L�[ �����̍œK��
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_2))
	//{
	//	for (auto building : m_BuildingManager->GetBuildings())
	//	{
	//		building->ConfirmGeometry();
	//	}
	//}
	//
	////3�L�[ �����̕\��On/Off
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_3))
	//{

	//}
	//
	////4�L�[ �����̃^�C�����P�����\��
	//if (Manager::GetInputKeyboard()->GetKeyPress(DIK_4))
	//{

	//}
	//
	////5�L�[ �Ԃ�On/Off
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_5))
	//{

	//}
	//
	////6�L�[ ���Z�b�g �������e�i���X��
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_6))
	//{

	//}
	//
	////7�L�[ �����̃��C���[�t���[���\��On/Off
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_7))
	//{

	//}
}

/*------------------------------------------------------------------------------
	�͂��߂̖ʂ𐶐�
------------------------------------------------------------------------------*/
void CityController::CreateFirstFace(float width, float height)
{
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
	//�y�n�̐���
	Land* land = new Land( m_LandManager, m_pGameObject);
	land->Init( parameter->vertices);

	//���ƃ����N
	m_CityAttributeManager->GetBlock(blockID)->LinkLand( land);

	//��ʃV�X�e����ݒ�
	std::list< RoadAttribute*> roadList;
	for (auto id : parameter->roadIDs)
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
	auto building = new Building( m_BuildingManager, m_pGameObject);
	building->InitGeometry( parameter);
	building->LinkLand( m_LandManager, landID);
}

/*------------------------------------------------------------------------------
	�Ԃ̐���
------------------------------------------------------------------------------*/
void CityController::CreateCar(int spawnLandID, int targetLandID)
{
	auto car = new Car( m_CarManager, m_pGameObject);
	//car->Init( m_LandManager->GetLand( spawnLandID)->GetTrafficLand(), m_LandManager->GetLand( targetLandID)->GetTrafficLand());
	car->Init( m_LandManager->GetLand( spawnLandID)->GetTrafficLand(), NULL);
}

