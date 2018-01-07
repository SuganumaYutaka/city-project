/*==============================================================================
	
	TrafficRoad.h - ��ʃV�X�e���[���H
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/
#ifndef _TRAFFIC_ROAD_H_
#define _TRAFFIC_ROAD_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;
class RoadAttribute;
class CarController;
class TrafficJunction;

/*------------------------------------------------------------------------------
	�N���X��`
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

