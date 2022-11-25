/*==============================================================================
	
	TrafficRoad.h - 交通システムー道路
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/
#ifndef _TRAFFIC_ROAD_H_
#define _TRAFFIC_ROAD_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class RoadAttribute;
class CarController;
class TrafficJunction;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class TrafficRoad : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	TrafficRoad( GameObject* pGameObject);
	void Uninit( void);

	RoadAttribute* GetAttribute( void) { return m_Attribute;}
	void SetAttribute( RoadAttribute* attribute);
	void UpdateAttribute( void) { m_IsUpdatedAttribute = true;}
	
	void RegisterCar( CarController* car);
	void UnregisterCar( CarController* car);

	const std::list<CarController*>& GetCars( TrafficJunction* nextJunction);
	CarController* GetFrontCar( CarController* carController);

	std::list<TrafficJunction*> GetJunctions( void);
	TrafficJunction* GetOtherJunction( TrafficJunction* junction);

	Vector3 GetVector( TrafficJunction* nextJunction);
	float GetWidth( void);

private:
	void Update(void);

	RoadAttribute* m_Attribute;
	bool m_IsUpdatedAttribute;

	std::list<CarController*> m_LeftSideCars;
	std::list<CarController*> m_RightSideCars;
	
};


#endif

