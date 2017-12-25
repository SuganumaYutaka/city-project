/*==============================================================================
	
	TrafficBuilding.h - ��ʃV�X�e���[����
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/
#ifndef _TRAFFIC_BUILDING_H_
#define _TRAFFIC_BUILDING_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;
class BuildingAttribute;
class CarController;
class TrafficRoad;
class CarFactory;
class RoadAttribute;

/*------------------------------------------------------------------------------
	�N���X��`
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

