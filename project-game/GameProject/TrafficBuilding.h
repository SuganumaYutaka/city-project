/*==============================================================================
	
	TrafficBuilding.h - 交通システムー建物
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/
#ifndef _TRAFFIC_BUILDING_H_
#define _TRAFFIC_BUILDING_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class BuildingAttribute;
class CarController;
class TrafficRoad;
class CarFactory;
class RoadAttribute;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class TrafficBuilding : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	TrafficBuilding( GameObject* pGameObject);
	void Uninit( void);

	void SetRoads( std::list<RoadAttribute*> roadAttributes);
	void SetCarFactory( CarFactory* carFactory);

	CarController* CreateCar( void);
	void EnterCar( CarController* car);
	CarController* ExitCar( CarController* car = NULL);

private:
	void Update(void);

	std::list<TrafficRoad*> m_Roads;
	std::list<CarController*> m_Cars;
	CarFactory* m_CarFactory;
};


#endif

