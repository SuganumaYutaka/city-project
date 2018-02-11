/*==============================================================================

    Building.h - ���̎��������[����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Building.h"
#include "GameObject.h"
#include "BuildingGeometry.h"
#include "Land.h"
#include "LandManager.h"
#include "BuildingManager.h"
#include "BuildingParameter.h"
#include "WallRenderer.h"
#include "Transform.h"
#include "BuildingSurfacePattern.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Building::Building(BuildingManager* manager, GameObject* parent)
{
	m_GameObject = new GameObject( parent);
	m_Geometry = NULL;
	m_LandManager = NULL;
	m_LandID = -1;

	m_Manager = manager;
	m_Manager->Register( this);
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Building::~Building()
{
	
}

/*------------------------------------------------------------------------------
	����
------------------------------------------------------------------------------*/
void Building::Delete(void)
{
	m_GameObject->ReleaseReserve();

	//�����N������
	if (m_LandManager)
	{
		m_LandManager->GetLand( m_LandID)->UnlinkBuilding( this);
	}
	m_Manager->Unregister( this);

	delete this;
}

/*------------------------------------------------------------------------------
	�W�I���g���̏�����
------------------------------------------------------------------------------*/
void Building::InitGeometry(GeometryParameter* parameter, BuildingSurfacePattern* surfacePattern)
{
	//���łɂ���W�I���g���ƕ`������폜
	if (m_Geometry)
	{
		m_Geometry->ReleaseReserve();
		m_Geometry = NULL;
		auto walls = m_GameObject->GetComponentList<WallRenderer>();
		for (auto wall : walls)
		{
			wall->ReleaseReserve();
		}
	}

	m_Geometry = m_GameObject->AddComponent<BuildingGeometry>();
	m_Geometry->Init( parameter, surfacePattern);
}

/*------------------------------------------------------------------------------
	�y�n�������̏���
------------------------------------------------------------------------------*/
void Building::OnDeleteLand(void)
{
	//�������Ƀ����N�̉��������Ȃ�
	m_LandManager = NULL;
	m_LandID = -1;

	//��������
	Delete();
}

/*------------------------------------------------------------------------------
	�y�n�ƃ����N����
------------------------------------------------------------------------------*/
void Building::LinkLand( LandManager* landManager, int landID)
{
	m_LandManager = landManager;
	m_LandID = landID;

	m_LandManager->GetLand( m_LandID)->LinkBuilding( this);

	//GameObject���ړ�
	m_GameObject->m_pTransform->SetWorldPosition( m_LandManager->GetLand( m_LandID)->GetGameObject()->m_pTransform->GetWorldPosition());
}

/*------------------------------------------------------------------------------
	�W�I���g�����m�肳����
------------------------------------------------------------------------------*/
void Building::ConfirmGeometry(void)
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
