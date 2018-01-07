/*==============================================================================

    BuildingManager.h - 建物の自動生成ー建物オブザーバー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/24
==============================================================================*/
#ifndef _BUILDING_MANAGER_H_
#define _BUILDING_MANAGER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class BuildingController;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BuildingManager
{
public:
	BuildingManager();

	void Register( BuildingController* buildingController);
	void Unregister( BuildingController* buildingController);

	const std::list<BuildingController*>& GetAllBuildings( void){ return m_Buildings;}

private:
	std::list<BuildingController*> m_Buildings;

};

#endif

