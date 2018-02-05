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

/*------------------------------------------------------------------------------
	デストラクタ
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

