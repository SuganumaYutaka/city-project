/*==============================================================================

    BuildingController.cpp - �����̎��������[�����R���g���[��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/24
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BuildingController.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "Land.h"
#include "Shape.h"
#include "Wall.h"

#include "BuildingRule.h"
#include "BuildingGeometry.h"
#include "CityAttribute.h"
#include "BuildingManager.h"
#include "TrafficBuilding.h"

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* BuildingController::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BuildingController>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BuildingController::BuildingController( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Geometry = NULL;
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void BuildingController::Uninit( void)
{
	if (m_BuildingManager)
	{
		m_BuildingManager->Unregister( this);
	}
}

/*------------------------------------------------------------------------------
	����������
------------------------------------------------------------------------------*/
bool BuildingController::Init( Land* land, BuildingRule* rule, std::list<RoadAttribute*> roads, BuildingManager* buildingManager, CarManager* carManager)
{
	m_BuildingManager = buildingManager;
	m_BuildingManager->Register( this);
	
	//�W�I���g���̏�����
	if( !m_Geometry)
	{
		m_Geometry = m_pGameObject->AddComponent<BuildingGeometry>();
	}
	m_Geometry->Init( land, rule);

	////��ʃv���O�����̌����̏�����
	//if (!m_TrafficBuilding)
	//{
	//	m_TrafficBuilding = m_pGameObject->AddComponent<TrafficBuilding>();
	//}
	//m_TrafficBuilding->Init( roads, carManager);

	return true;
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void BuildingController::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	�W�I���g�����m�肳����
------------------------------------------------------------------------------*/
void BuildingController::ConfirmGeometry(void)
{
	if (!m_Geometry)
	{
		return;
	}
	m_Geometry->ConfirmGeometry();
	
	//�W�I���g�����̍폜
	m_Geometry->ReleaseReserve();
	m_Geometry =NULL;
}

/*------------------------------------------------------------------------------
	�Ǘ��N���X���폜�����ۂ̏���
------------------------------------------------------------------------------*/
void BuildingController::OnBuildingManagerDelete()
{
	m_BuildingManager = NULL;
	m_pGameObject->ReleaseReserve();
}

