/*==============================================================================
	
	TrafficLand.h - 交通システムー土地
														Author : Yutaka Suganuma
														Date   : 2018/2/3
==============================================================================*/
#ifndef _TRAFFIC_LAND_H_
#define _TRAFFIC_LAND_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class LandAttribute;
class CarController;
class TrafficRoad;
class RoadAttribute;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class TrafficLand : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	TrafficLand( GameObject* pGameObject);
	void Uninit( void);

	void Init( std::list<RoadAttribute*> roadAttributes);

	void EnterCar( CarController* car);
	bool ExitCar( CarController* car = NULL);
	CarController* PopFrontCar( void);
	CarController* PopBackCar( void);
	int GetCarCount( void){ return m_Cars.size(); }
	bool IsCarInLand( CarController* car);
	const std::list<TrafficRoad*>& GetTrafficRoad( void){ return m_Roads;}

private:
	void Update(void);

	std::list<TrafficRoad*> m_Roads;
	std::list<CarController*> m_Cars;
};


#endif

