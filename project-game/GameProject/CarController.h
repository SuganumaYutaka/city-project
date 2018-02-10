/*==============================================================================
	
	CarController.h - ��ʃV�X�e���[��
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/
#ifndef _CAR_CONTROLLER_H_
#define _CAR_CONTROLLER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"
#include "CarState.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;
class TrafficJunction;
class TrafficRoad;
class TrafficLand;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class CarController : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	CarController( GameObject* pGameObject);
	void Uninit( void);

	void Init( TrafficRoad* currentRoad, TrafficJunction* nextJunction, TrafficLand* targetLand);

	void SetCurrentRoad( TrafficRoad* road){ m_CurrentRoad = road;}
	TrafficRoad* GetCurrentRoad( void){ return m_CurrentRoad;}
	void SetNextJunction( TrafficJunction* junction){ m_NextJunction = junction;}
	TrafficJunction* GetNextJunction( void){ return m_NextJunction;}
	void SetTargetLand( TrafficLand* building){ m_TargetLand = building;}
	TrafficLand* GetTargetLand( void){ return m_TargetLand;}
	Vector3 GetCurrentRoadVector( void);
	CarController* GetFrontCar(void);
	bool CheckOnRoad( void);
	bool IsCorrectSideRoad( void);
	float CulcLengthFromRoadCenter( void);
	float CulcMoveDistanceOnRoad( void);
	float CulcMoveRateOnRoad( void);
	float CulcRemainDistanceOnRoad( void);
	TrafficRoad* GetRightRoad( void);
	TrafficRoad* GetLeftRoad( void);
	TrafficRoad* GetStraightRoad( void);
	bool CanTurnRight( void);
	bool CanTurnLeft( void);
	bool CanGoStraight( void);
	bool TurnRight( void);
	bool TurnLeft( void);
	bool GoStraight( void);
	void ChangeState( ECarState next);
	void OnDeleteCurrentRoad(void);

	float GetSpeed( void){ return m_Speed;}
	void SetSpeed( float speed){ m_Speed = speed;}
	void AddSpeed( float addSpeed){ m_Speed += addSpeed;}
	void BrakeSpeed( void);

private:
	void Update(void);

	TrafficRoad* m_CurrentRoad;
	TrafficJunction* m_NextJunction;
	TrafficLand* m_TargetLand;
	CarState* m_CurrentState;
	std::unordered_map<ECarState, CarState*> m_States;

	float m_Speed;
};


#endif

