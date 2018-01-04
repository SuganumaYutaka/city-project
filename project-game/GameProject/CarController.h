/*==============================================================================
	
	CarController.h - 交通システムー車
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/
#ifndef _CAR_CONTROLLER_H_
#define _CAR_CONTROLLER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"
#include "CarState.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class TrafficJunction;
class TrafficRoad;
class TrafficBuilding;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class CarController : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	CarController( GameObject* pGameObject);
	void Uninit( void);

	void Init( TrafficRoad* currentRoad, TrafficJunction* nextJunction, TrafficBuilding* targetBuilding);

	void SetCurrentRoad( TrafficRoad* road){ m_CurrentRoad = road;}
	TrafficRoad* GetCurrentRoad( void){ return m_CurrentRoad;}
	void SetNextJunction( TrafficJunction* junction){ m_NextJunction = junction;}
	TrafficJunction* GetNextJunction( void){ return m_NextJunction;}
	void SetTargetBuilding( TrafficBuilding* building){ m_TargetBuilding = building;}
	TrafficBuilding* GetTargetBuilding( void){ return m_TargetBuilding;}
	float CulcMoveDistanceOnRoad( void);
	float CulcMoveRateOnRoad( void);
	float CulcRemainDistanceOnRoad( void);

	bool TurnRight( void);
	bool TurnLeft( void);
	bool GoStraight( void);

private:
	void Update(void);

	TrafficRoad* m_CurrentRoad;
	TrafficJunction* m_NextJunction;
	TrafficBuilding* m_TargetBuilding;
	CarState* m_CurrentState;
	std::unordered_map<ECarState, CarState*> m_States;
};


#endif

