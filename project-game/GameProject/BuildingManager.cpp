/*==============================================================================

    BuildingManager.cpp - 建物の自動生成ー建物オブザーバー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/24
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BuildingManager.h"
#include "BuildingController.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
BuildingManager::BuildingManager()
{
	m_Buildings.clear();
}

/*------------------------------------------------------------------------------
	建物の登録
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
	建物の解除
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
