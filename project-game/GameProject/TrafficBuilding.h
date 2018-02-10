///*==============================================================================
//	
//	TrafficBuilding.h - 交通システムー建物
//														Author : Yutaka Suganuma
//														Date   : 2017/12/25
//==============================================================================*/
//#ifndef _TRAFFIC_BUILDING_H_
//#define _TRAFFIC_BUILDING_H_
//
///*------------------------------------------------------------------------------
//	インクルードファイル
//------------------------------------------------------------------------------*/
//#include "Manager.h"
//#include "Component.h"
//
///*------------------------------------------------------------------------------
//	前方宣言
//------------------------------------------------------------------------------*/
//class GameObject;
//class BuildingAttribute;
//class CarController;
//class TrafficRoad;
//class RoadAttribute;
//class CarManager;
//
///*------------------------------------------------------------------------------
//	クラス定義
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
