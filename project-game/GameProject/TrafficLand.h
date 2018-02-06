/*==============================================================================
	
	TrafficLand.h - ��ʃV�X�e���[�y�n
														Author : Yutaka Suganuma
														Date   : 2018/2/3
==============================================================================*/
#ifndef _TRAFFIC_LAND_H_
#define _TRAFFIC_LAND_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;
class LandAttribute;
class CarController;
class TrafficRoad;
class RoadAttribute;

/*------------------------------------------------------------------------------
	�N���X��`
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

