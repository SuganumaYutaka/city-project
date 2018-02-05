///*==============================================================================
//	
//	TrafficBuilding.h - ��ʃV�X�e���[����
//														Author : Yutaka Suganuma
//														Date   : 2017/12/25
//==============================================================================*/
//#ifndef _TRAFFIC_BUILDING_H_
//#define _TRAFFIC_BUILDING_H_
//
///*------------------------------------------------------------------------------
//	�C���N���[�h�t�@�C��
//------------------------------------------------------------------------------*/
//#include "Manager.h"
//#include "Component.h"
//
///*------------------------------------------------------------------------------
//	�O���錾
//------------------------------------------------------------------------------*/
//class GameObject;
//class BuildingAttribute;
//class CarController;
//class TrafficRoad;
//class RoadAttribute;
//class CarManager;
//
///*------------------------------------------------------------------------------
//	�N���X��`
//------------------------------------------------------------------------------*/
//class TrafficBuilding : public Component
//{
//public:
//	static Component* Create( GameObject* gameObject);
//
//	TrafficBuilding( GameObject* pGameObject);
//	void Uninit( void);
//
//	void Init( std::list<RoadAttribute*> roadAttributes, CarManager* carManager);
//
//	CarController* CreateCar( void);
//	void EnterCar( CarController* car);
//	bool ExitCar( CarController* car = NULL);
//	CarController* PopFrontCar( void);
//	CarController* PopBackCar( void);
//	int GetCarCount( void){ return m_Cars.size(); }
//	bool IsCarInBuilding( CarController* car);
//
//private:
//	void Update(void);
//
//	std::list<TrafficRoad*> m_Roads;
//	std::list<CarController*> m_Cars;
//	CarManager* m_CarManager;
//};
//
//
//#endif
//
