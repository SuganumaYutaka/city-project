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

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define CITY_WIDTH (200.0f)
#define CITY_HEIGHT (200.0f)
#define DIVIDE_COUNT ( 4)
#define CREATE_CAR_COUNT ( 50)

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

	//�����Ǘ��I�u�W�F�N�g����
	m_BuildingManager = new BuildingManager();

	//�Ԃ̊Ǘ��I�u�W�F�N�g����
	auto carManagerObject = new GameObject( m_pGameObject);
	m_CarManager = carManagerObject->AddComponent<CarManager>();

	//�����̎��������V�X�e���𐶐�
	m_BuildingRuleFactory = new BuildingRuleFactory();

	//���̎��������V�X�e���ݒ�̃n�[�t�G�b�W�f�[�^�\���𐶐�
	auto attributeFactory = new CityAttributeFactory( m_pGameObject, m_BuildingRuleFactory, m_BuildingManager, m_CarManager);
	m_Model = new Model( new CityRule(), attributeFactory);

	//�͂��߂̖ʂ𐶐�
	Vector3 sizeHalf( CITY_WIDTH * 0.5f, 0.0f, CITY_HEIGHT * 0.5f);
	m_Model->CreateFirstFace( 
		Vector3( -sizeHalf.x, 0.0f, +sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, +sizeHalf.z),
		Vector3( -sizeHalf.x, 0.0f, -sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, -sizeHalf.z));

	//��敪��
	for( int count = 0; count < DIVIDE_COUNT; count++)
	{
		m_Model->DivideAllFaces();
	}

	//��������Ԃ𐶐�
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

	//�����W�I���g�����̍폜
	for (auto building : m_BuildingManager->GetAllBuildings())
	{
		building->ConfirmGeometry();
	}
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void CityController::Uninit( void)
{
	delete m_Model;
	delete m_BuildingRuleFactory;
	delete m_BuildingManager;
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void CityController::Update()
{
	////��敪��
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE))
	//{
	//	m_Model->DivideAllFaces();
	//}

	////��������Ԃ𐶐�
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

	////�����W�I���g�����̍폜
	//if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE))
	//{
	//	for (auto building : m_BuildingManager->GetAllBuildings())
	//	{
	//		building->ConfirmGeometry();
	//	}
	//}
}
