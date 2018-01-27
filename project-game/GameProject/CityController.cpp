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
#include "CityRule.h"
#include "CityAttribute.h"

#include "InputKeyboard.h"

#include "BuildingRuleFactory.h"
#include "BuildingManager.h"
#include "CarManager.h"

#include "TrafficBuilding.h"
#include "BuildingController.h"

#include "CarController.h"
#include "CarManager.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define CITY_WIDTH (200.0f)
#define CITY_HEIGHT (200.0f)
#define DIVIDE_COUNT ( 3)
#define CREATE_CAR_COUNT ( 20)

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
	m_BuildingRuleFactory = NULL;
	m_BuildingManager = NULL;
	m_CarManager = NULL;

	m_Width = CITY_WIDTH;
	m_Height = CITY_HEIGHT;
	m_CountCar = CREATE_CAR_COUNT;
	m_IsWireFrame = false;
	
	//Init( CITY_WIDTH, CITY_HEIGHT, DIVIDE_COUNT, CREATE_CAR_COUNT, true);
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void CityController::Init(float cityWidth, float cityHeight, int countDivide, int countCar, bool doConfirmGeometry)
{
	m_Width = cityWidth;
	m_Height = cityHeight;
	m_CountCar = countCar;

	//�Ԃ̏���
	DeleteAllCars();

	//�I������
	Uninit();

	//�����Ǘ��I�u�W�F�N�g����
	m_BuildingManager = new BuildingManager();

	//�����̎��������V�X�e���𐶐�
	m_BuildingRuleFactory = new BuildingRuleFactory();

	//�ԊǗ��I�u�W�F�N�g����
	if( !m_CarManager)
	{
		auto carManagerObject = new GameObject( m_pGameObject);
		m_CarManager = carManagerObject->AddComponent<CarManager>();
	}

	//���̎��������V�X�e���ݒ�̃n�[�t�G�b�W�f�[�^�\���𐶐�
	auto attributeFactory = new CityAttributeFactory( m_pGameObject, m_BuildingRuleFactory, m_BuildingManager, m_CarManager);
	m_Model = new Model( new CityRule(), attributeFactory);

	//�͂��߂̖ʂ𐶐�
	Vector3 sizeHalf( cityWidth * 0.5f, 0.0f, cityHeight * 0.5f);
	m_Model->CreateFirstFace( 
		Vector3( -sizeHalf.x, 0.0f, +sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, +sizeHalf.z),
		Vector3( -sizeHalf.x, 0.0f, -sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, -sizeHalf.z));

	//��敪��
	for( int count = 0; count < countDivide; count++)
	{
		m_Model->DivideAllFaces();
	}

	//�Ԃ𐶐�
	CreateCars( countCar);

	//�����W�I���g�����̍폜
	if( doConfirmGeometry)
	{
		for (auto building : m_BuildingManager->GetAllBuildings())
		{
			building->ConfirmGeometry();
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
	if (m_BuildingRuleFactory)
	{
		delete m_BuildingRuleFactory;
		m_BuildingRuleFactory = NULL;
	}
	if( m_BuildingManager)
	{
		delete m_BuildingManager;
		m_BuildingManager = NULL;
	}
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void CityController::Update()
{
	//��敪��
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_1))
	{
		DeleteAllCars();
		m_Model->DivideAllFaces();
	}

	//�����̍œK��
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_2))
	{
		for (auto building : m_BuildingManager->GetAllBuildings())
		{
			building->ConfirmGeometry();

			//���C���[�t���[���ݒ�
			auto renderers = building->m_pGameObject->GetComponentList<WallRenderer>();
			for( auto renderer : renderers)
			{
				renderer->ChangeWireFrame( m_IsWireFrame);
			}
		}
	}

	//�����̕\��/��\��
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_3))
	{
		for( auto building : m_BuildingManager->GetAllBuildings())
		{
			auto renderers = building->m_pGameObject->GetComponentList<WallRenderer>();
			for( auto renderer : renderers)
			{
				if (renderer->ChangeRenderPolygon())
				{
					auto roofs = building->m_pGameObject->GetComponentListInChildren<Polygon3DRenderer>();
					for (auto roof : roofs)
					{
						roof->SetActive( true);
					}
				}
				else
				{
					auto roofs = building->m_pGameObject->GetComponentListInChildren<Polygon3DRenderer>();
					for (auto roof : roofs)
					{
						roof->SetActive( false);
					}
				}
			}
		}
	}

	//�����̃|���S����1�����\��
	if (Manager::GetInputKeyboard()->GetKeyPress(DIK_4))
	{
		for( auto building : m_BuildingManager->GetAllBuildings())
		{
			auto renderers = building->m_pGameObject->GetComponentList<WallRenderer>();
			for( auto renderer : renderers)
			{
				renderer->AddRenderPolygon();
				renderer->AddRenderPolygon();
				
				if (renderer->AddRenderPolygon())
				{
					auto roofs = building->m_pGameObject->GetComponentListInChildren<Polygon3DRenderer>();
					for (auto roof : roofs)
					{
						roof->SetActive( true);
					}
				}
			}
		}
	}

	//�Ԃ�On/Off
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_5))
	{
		if (m_CarManager->m_pGameObject->GetComponentListInChildren<CarController>().size() == 0)
		{
			CreateCars( m_CountCar);
		}
		else
		{
			DeleteAllCars();
		}
	}

	////�S�f�[�^�����Z�b�g
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_6))
	//{
	//	Init( m_Width,  m_Height, 0, 0, false);
	//	return;
	//}

	//���C���[�t���[���ݒ�
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_7))
	{
		m_IsWireFrame ? m_IsWireFrame = false : m_IsWireFrame = true;

		for( auto building : m_BuildingManager->GetAllBuildings())
		{
			auto renderers = building->m_pGameObject->GetComponentList<WallRenderer>();
			for( auto renderer : renderers)
			{
				renderer->ChangeWireFrame( m_IsWireFrame);
			}
		}
	}
}

/*------------------------------------------------------------------------------
	�Ԃ𐶐�
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

	//��������Ԃ𐶐�
	auto buildings = m_BuildingManager->GetAllBuildings();
	if( buildings.size() == 0)
	{
		return;
	}
	int parCreate = buildings.size() / countCar;
	if (parCreate <= 0)
	{
		parCreate = 1;
	}
	int count = 0;
	for (auto ite = buildings.begin(); ite != buildings.end(); ++ite, ++count)
	{
		if (count % parCreate == 0)
		{
			auto trafficBuilding = (*ite)->GetTrafficBuilding();
			trafficBuilding->CreateCar();
		}
	}
}

/*------------------------------------------------------------------------------
	���ׂĂ̎Ԃ�����
------------------------------------------------------------------------------*/
void CityController::DeleteAllCars(void)
{
	if (m_CarManager)
	{
		auto cars = m_CarManager->m_pGameObject->GetComponentListInChildren<CarController>();
		for (auto car : cars)
		{
			car->m_pGameObject->ReleaseReserve();
		}
	}
}
