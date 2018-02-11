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
#include "GameObject.h"
#include "HalfEdgeModel.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
CityAttributeManager::CityAttributeManager(GameObject* parent)
{
	m_GameObject = new GameObject( parent);
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
CityAttributeManager::~CityAttributeManager()
{
	for (auto block : m_Blocks)
	{
		if( block)
		{
			delete block;
		}
	}
	m_Blocks.clear();
	m_Blocks.shrink_to_fit();

	for (auto road : m_Roads)
	{
		if( road)
		{
			delete road;
		}
	}
	m_Roads.clear();
	m_Roads.shrink_to_fit();

	for (auto junction : m_Junctions)
	{
		if( junction)
		{
			delete junction;
		}
	}
	m_Junctions.clear();
	m_Junctions.shrink_to_fit();
}

/*------------------------------------------------------------------------------
	�������̃N���A
------------------------------------------------------------------------------*/
void CityAttributeManager::Clear(void)
{
	for (auto block : m_Blocks)
	{
		if( block)
		{
			block->Delete();
		}
	}
	m_Blocks.clear();
	m_Blocks.shrink_to_fit();

	for (auto road : m_Roads)
	{
		if( road)
		{
			road->Delete();
		}
	}
	m_Roads.clear();
	m_Roads.shrink_to_fit();

	for (auto junction : m_Junctions)
	{
		if( junction)
		{
			junction->Delete();
		}
	}
	m_Junctions.clear();
	m_Junctions.shrink_to_fit();
}

/*------------------------------------------------------------------------------
	GameObject�̏���
------------------------------------------------------------------------------*/
void CityAttributeManager::DeleteGameObject(void)
{
	if (m_GameObject)
	{
		m_GameObject->ReleaseReserve();
		m_GameObject = NULL;
	}
}

/*------------------------------------------------------------------------------
	�����_�𐶐�
------------------------------------------------------------------------------*/
JunctionAttribute* CityAttributeManager::CreateJunctionAttribute(HalfEdgeDataStructure::Model* model, int modelID)
{
	auto junction = new JunctionAttribute( model, modelID, this, m_GameObject);
	return junction;
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
	���H�𐶐�
------------------------------------------------------------------------------*/
RoadAttribute* CityAttributeManager::CreateRoadAttribute(HalfEdgeDataStructure::Model* model, int modelID)
{
	auto road = new RoadAttribute( model, modelID, this, m_GameObject);
	return road;
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
	���𐶐�
------------------------------------------------------------------------------*/
BlockAttribute* CityAttributeManager::CreateBlockAttribute(HalfEdgeDataStructure::Model* model, int modelID)
{
	auto block = new BlockAttribute( model, modelID, this, m_GameObject);
	return block;
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

