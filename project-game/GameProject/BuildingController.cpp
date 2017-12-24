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

#include "BuildingRule.h"
#include "BuildingGeometry.h"
#include "CityAttribute.h"
#include "BuildingManager.h"

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
bool BuildingController::Init( const std::vector<Vector3>& vertices, BuildingRule* rule, std::list<RoadAttribute*> roads, BuildingManager* manager)
{
	m_BuildingManager = manager;
	m_BuildingManager->Register( this);
	
	//�W�I���g���̏�����
	if( !m_Geometry)
	{
		m_Geometry = m_pGameObject->AddComponent<BuildingGeometry>();
	}
	bool hr = false;
	hr = m_Geometry->Init( vertices, rule);

	return hr;
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void BuildingController::Update( void)
{
	
}

