/*==============================================================================

    BuildingManager.cpp - 建物の自動生成ー建物オブザーバー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/24
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BuildingManager.h"
#include "Building.h"

/*------------------------------------------------------------------------------
	デストラクタ
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
	建物を取得
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
	建物のIDを取得
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

	//発見できない
	return -1;
}

/*------------------------------------------------------------------------------
	建物の解除
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
