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
