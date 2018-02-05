/*==============================================================================

    CityAttributeManager.cpp - ���̎��������[�������Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "CityAttributeManager.h"
#include "CityAttribute.h"

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
CityAttributeManager::~CityAttributeManager()
{
	for (auto block : m_Blocks)
	{
		delete block;
	}
	m_Blocks.clear();
	m_Blocks.shrink_to_fit();

	for (auto road : m_Roads)
	{
		delete road;
	}
	m_Roads.clear();
	m_Roads.shrink_to_fit();

	for (auto junction : m_Junctions)
	{
		delete junction;
	}
	m_Junctions.clear();
	m_Junctions.shrink_to_fit();
}


/*------------------------------------------------------------------------------
	�����_���擾
------------------------------------------------------------------------------*/
JunctionAttribute* CityAttributeManager::GetJunction(int id)
{
	if (m_Junctions.size() <= id)
	{
		return NULL;
	}

	return m_Junctions[ id];
}

/*------------------------------------------------------------------------------
	�����_��ID���擾
------------------------------------------------------------------------------*/
int CityAttributeManager::GetJunctionID( JunctionAttribute* junction)
{
	int size = m_Junctions.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Junctions[i] == junction)
		{
			return i;
		}
	}

	//�����ł��Ȃ�
	return -1;
}

/*------------------------------------------------------------------------------
	�o�^������
------------------------------------------------------------------------------*/
bool CityAttributeManager::UnregisterJunction( JunctionAttribute* junction)
{
	int size = m_Junctions.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Junctions[i] == junction)
		{
			m_Junctions[i] = NULL;
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
	���H���擾
------------------------------------------------------------------------------*/
RoadAttribute* CityAttributeManager::GetRoad(int id)
{
	if (m_Roads.size() <= id)
	{
		return NULL;
	}

	return m_Roads[ id];
}

/*------------------------------------------------------------------------------
	���H��ID���擾
------------------------------------------------------------------------------*/
int CityAttributeManager::GetRoadID( RoadAttribute* road)
{
	int size = m_Roads.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Roads[i] == road)
		{
			return i;
		}
	}

	//�����ł��Ȃ�
	return -1;
}

/*------------------------------------------------------------------------------
	�o�^������
------------------------------------------------------------------------------*/
bool CityAttributeManager::UnregisterRoad( RoadAttribute* road)
{
	int size = m_Roads.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Roads[i] == road)
		{
			m_Roads[i] = NULL;
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
	�����擾
------------------------------------------------------------------------------*/
BlockAttribute* CityAttributeManager::GetBlock(int id)
{
	if (m_Blocks.size() <= id)
	{
		return NULL;
	}

	return m_Blocks[ id];
}

/*------------------------------------------------------------------------------
	����ID���擾
------------------------------------------------------------------------------*/
int CityAttributeManager::GetBlockID( BlockAttribute* block)
{
	int size = m_Blocks.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Blocks[i] == block)
		{
			return i;
		}
	}

	//�����ł��Ȃ�
	return -1;
}

/*------------------------------------------------------------------------------
	�o�^������
------------------------------------------------------------------------------*/
bool CityAttributeManager::UnregisterBlock( BlockAttribute* block)
{
	int size = m_Blocks.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Blocks[i] == block)
		{
			m_Blocks[i] = NULL;
			return true;
		}
	}

	return false;
}

