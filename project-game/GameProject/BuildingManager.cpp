/*==============================================================================

    BuildingManager.cpp - �����̎��������[�����I�u�U�[�o�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/24
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BuildingManager.h"
#include "BuildingController.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BuildingManager::BuildingManager()
{
	m_Buildings.clear();
}

/*------------------------------------------------------------------------------
	�����̓o�^
------------------------------------------------------------------------------*/
void BuildingManager::Register(BuildingController* buildingController)
{
	for (auto building : m_Buildings)
	{
		if (buildingController == building)
		{
			return;
		}
	}

	m_Buildings.push_back( buildingController);
}

/*------------------------------------------------------------------------------
	�����̉���
------------------------------------------------------------------------------*/
void BuildingManager::Unregister(BuildingController* buildingController)
{
	for (auto ite = m_Buildings.begin(); ite != m_Buildings.end(); ++ite)
	{
		if (*ite == buildingController)
		{
			m_Buildings.erase( ite);
			return;
		}
	}
}
