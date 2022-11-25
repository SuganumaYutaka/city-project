/*==============================================================================

    CityAttributeManager.cpp - 町の自動生成ー属性情報管理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "CityAttributeManager.h"
#include "CityAttribute.h"
#include "GameObject.h"
#include "HalfEdgeModel.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
CityAttributeManager::CityAttributeManager(GameObject* parent)
{
	m_GameObject = new GameObject( parent);
}

/*------------------------------------------------------------------------------
	デストラクタ
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
	属性情報のクリア
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
	GameObjectの消去
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
	交差点を生成
------------------------------------------------------------------------------*/
JunctionAttribute* CityAttributeManager::CreateJunctionAttribute(HalfEdgeDataStructure::Model* model, int modelID)
{
	auto junction = new JunctionAttribute( model, modelID, this, m_GameObject);
	return junction;
}

/*------------------------------------------------------------------------------
	交差点を取得
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
	交差点のIDを取得
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

	//発見できない
	return -1;
}

/*------------------------------------------------------------------------------
	登録を解除
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
	道路を生成
------------------------------------------------------------------------------*/
RoadAttribute* CityAttributeManager::CreateRoadAttribute(HalfEdgeDataStructure::Model* model, int modelID)
{
	auto road = new RoadAttribute( model, modelID, this, m_GameObject);
	return road;
}

/*------------------------------------------------------------------------------
	道路を取得
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
	道路のIDを取得
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

	//発見できない
	return -1;
}

/*------------------------------------------------------------------------------
	登録を解除
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
	区画を生成
------------------------------------------------------------------------------*/
BlockAttribute* CityAttributeManager::CreateBlockAttribute(HalfEdgeDataStructure::Model* model, int modelID)
{
	auto block = new BlockAttribute( model, modelID, this, m_GameObject);
	return block;
}

/*------------------------------------------------------------------------------
	区画を取得
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
	区画のIDを取得
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

	//発見できない
	return -1;
}

/*------------------------------------------------------------------------------
	登録を解除
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

