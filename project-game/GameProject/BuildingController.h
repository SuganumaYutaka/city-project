/*==============================================================================

    BuildingController.h - 建物の自動生成ー建物コントローラ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/24
==============================================================================*/
#ifndef _BUILDING_CONTROLLER_H_
#define _BUILDING_CONTROLLER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class BuildingGeometry;
class BuildingRule;
class RoadAttribute;
class BuildingManager;
class TrafficBuilding;
class CarManager;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BuildingController : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BuildingController( GameObject* pGameObject);
	void Uninit( void);

	bool Init( const std::vector<Vector3>& vertices, BuildingRule* rule, std::list<RoadAttribute*> roads, BuildingManager* buildingManager, CarManager* carManager);

	BuildingGeometry* GetGeometry( void){ return m_Geometry;}
	TrafficBuilding* GetTrafficBuilding( void){ return m_TrafficBuilding;}

private:
	void Update(void);

	BuildingManager* m_BuildingManager;
	BuildingGeometry* m_Geometry;
	TrafficBuilding* m_TrafficBuilding;
	
};

#endif

