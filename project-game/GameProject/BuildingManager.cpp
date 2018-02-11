/*==============================================================================

    BuildingManager.cpp - �����̎��������[�����I�u�U�[�o�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/24
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BuildingManager.h"
#include "Building.h"
#include "GameObject.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BuildingManager::BuildingManager(GameObject* parent)
{
	m_GameObject = new GameObject( parent);
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
BuildingManager::~BuildingManager()
{
	for (auto building : m_Buildings)
	{
		if( building)
		{
			delete building;
		}
	}
}

/*------------------------------------------------------------------------------
	GameObject�̏���
------------------------------------------------------------------------------*/
void BuildingManager::DeleteGameObject(void)
{
	if (m_GameObject)
	{
		m_GameObject->ReleaseReserve();
		m_GameObject = NULL;
	}
}

/*------------------------------------------------------------------------------
	�����𐶐�
------------------------------------------------------------------------------*/
Building* BuildingManager::CreateBuilding(GeometryParameter* parameter, BuildingSurfacePattern* surfacePattern)
{
	auto building = new Building( this, m_GameObject);
	building->InitGeometry( parameter, surfacePattern);

	return building;
}

/*------------------------------------------------------------------------------
	�������擾
------------------------------------------------------------------------------*/
Building* BuildingManager::GetBuilding(int id)
{
	if (m_Buildings.size() <= id)
	{
		return NULL;
	}

	return m_Buildings[ id];
}

/*------------------------------------------------------------------------------
	������ID���擾
------------------------------------------------------------------------------*/
int BuildingManager::GetBuildingID( Building* building)
{
	int size = m_Buildings.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Buildings[i] == building)
		{
			return i;
		}
	}

	//�����ł��Ȃ�
	return -1;
}

/*------------------------------------------------------------------------------
	�����̉���
------------------------------------------------------------------------------*/
bool BuildingManager::Unregister(Building* building)
{
	int size = m_Buildings.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Buildings[i] == building)
		{
			m_Buildings[i] = NULL;
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
	�����̐����擾
------------------------------------------------------------------------------*/
int BuildingManager::GetBuildingCount(void)
{
	int size = m_Buildings.size();
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (m_Buildings[i])
		{
			count++;
		}
	}

	return count;
}
