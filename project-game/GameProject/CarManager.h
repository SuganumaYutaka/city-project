/*==============================================================================
	
	CarManager.h - 交通システムー車マネージャー
														Author : Yutaka Suganuma
														Date   : 2017/12/28
==============================================================================*/
#ifndef _CAR_MANAGER_H_
#define _CAR_MANAGER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class CarController;
class TrafficJunction;
class TrafficRoad;
class TrafficBuilding;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class CarManager : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	CarManager( GameObject* pGameObject);
	void Uninit( void);

	CarController* CreateCarController( const Vector3& position, const D3DXQUATERNION& rotation
		, TrafficRoad* currentRoad, TrafficJunction* nextJunction, TrafficBuilding* targetBuilding);

private:
	void Update(void);

	std::list<CarController*> GetAllCarControllers( void);
};


#endif

