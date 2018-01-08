/*==============================================================================
	
	CarState.cpp - 交通システムー車の状態
														Author : Yutaka Suganuma
														Date   : 2017/12/30
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "CarState.h"
#include "CarController.h"
#include "TrafficRoad.h"
#include "TrafficJunction.h"
#include "TrafficBuilding.h"
#include "ComponentInclude.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define ADD_SPEED_ON_ROAD (0.09f)
#define ADD_SPEED_TOWARD_ROAD (0.07f)
#define ADD_SPEED_NEAR_JUNCTION ( 0.03f)
#define SPEED_SIDEWAY ( 0.03f)
#define MAX_DISTANCE_FRONT_CAR ( 10.0f)
#define MIN_DISTANCE_FRONT_CAR ( 5.0f)
#define SLERP_VALUE (0.2f)

/*------------------------------------------------------------------------------
	State : ニュートラル
------------------------------------------------------------------------------*/
void CarStateNeutral::Update(void)
{

}


/*------------------------------------------------------------------------------
	State : 道路上を走行
------------------------------------------------------------------------------*/
void CarStateMoveOnRoad::Init(void)
{

}

void CarStateMoveOnRoad::Update(void)
{
	//道路の方向と車の方向を合わせる
	auto roadVector = m_CarController->GetCurrentRoadVector();
	m_CarController->m_pTransform->SetWorldRotationLookDirection( roadVector);

	//現在位置が車線の中央から外れている場合修正
	if (!m_CarController->CheckOnRoad())
	{
		float length = m_CarController->CulcLengthFromRoadCenter();
		float widthHalf = m_CarController->GetCurrentRoad()->GetWidth() * 0.5f;
		Vector3 sidewayVector = Vector3::Cross( roadVector, Vector3(0.0f, 1.0f, 0.0f));
		if (length < widthHalf * 0.5f)
		{
			m_CarController->m_pTransform->Move( sidewayVector.Normalize() * SPEED_SIDEWAY);
		}
		else
		{
			m_CarController->m_pTransform->Move( sidewayVector.Normalize() * SPEED_SIDEWAY * -1.0f);
		}
	}

	//前の車との車間距離に応じて加速・減速
	auto frontCar = m_CarController->GetFrontCar();
	if (frontCar == NULL)
	{
		m_CarController->AddSpeed( ADD_SPEED_ON_ROAD);
	}
	else
	{
		float distance = Vector3::Distance( m_CarController->m_pTransform->GetWorldPosition(), frontCar->m_pTransform->GetWorldPosition());
		if (distance > MAX_DISTANCE_FRONT_CAR)
		{
			m_CarController->AddSpeed( ADD_SPEED_ON_ROAD);
		}
		else if (distance < MIN_DISTANCE_FRONT_CAR)
		{
			m_CarController->BrakeSpeed();
		}
	}

	//交差点に近い場合は状態遷移
	float lengthToNextJunction = m_CarController->CulcRemainDistanceOnRoad();
	if (lengthToNextJunction < m_CarController->GetCurrentRoad()->GetWidth() * 0.75f)
	{
		m_CarController->ChangeState( eCarStateNearNextJunction);
	}
}

/*------------------------------------------------------------------------------
	State : 道路に合流するために走行
------------------------------------------------------------------------------*/
void CarStateMoveTowardRoad::Init(void)
{

}

void CarStateMoveTowardRoad::Update(void)
{
	//加速
	m_CarController->AddSpeed( ADD_SPEED_TOWARD_ROAD);

	//車線に接近してから車線方向にSlerp
	auto roadVector = m_CarController->GetCurrentRoadVector();
	float length = m_CarController->CulcLengthFromRoadCenter();
	float widthHalf = m_CarController->GetCurrentRoad()->GetWidth() * 0.5f;
	if (!m_CarController->IsCorrectSideRoad())
	{
		//車線に接近
		Vector3 sidewayVector = Vector3::Cross( roadVector, Vector3(0.0f, 1.0f, 0.0f));
		m_CarController->m_pTransform->RotateLookDirection( sidewayVector, SLERP_VALUE);
	}
	else if (length > widthHalf)
	{
		//車線に接近
		Vector3 sidewayVector = Vector3::Cross( roadVector, Vector3(0.0f, 1.0f, 0.0f)) * -1.0f;
		m_CarController->m_pTransform->RotateLookDirection( sidewayVector, SLERP_VALUE);
	}
	else if (Vector3::Dot( roadVector.Normalize(), m_CarController->m_pTransform->GetForward()) > 0.95f)
	{
		//曲がり切って合流が終わったら状態遷移
		m_CarController->ChangeState( eCarStateMoveOnRoad);
	}
	else
	{
		//車線方向にSlerp
		m_CarController->m_pTransform->RotateLookDirection( roadVector, SLERP_VALUE);
	}
}

/*------------------------------------------------------------------------------
	State : 次の交差点に接近した状態
------------------------------------------------------------------------------*/
CarStateNearNextJunction::CarStateNearNextJunction( CarController* carController) : m_JunctionSelect(eCarJunctionGoStraight)
{ 
	m_CarController = carController;
	m_Random = new Random();
	m_Random->SetRangeInt(0, 3);
}

CarStateNearNextJunction::~CarStateNearNextJunction()
{
	delete m_Random;
}

void CarStateNearNextJunction::Init(void)
{
	//交差点を進む方向の決定
	for(;;)
	{
		m_JunctionSelect = (ECarJunctionSelect)m_Random->GetInt();

		switch (m_JunctionSelect)
		{
		case eCarJunctionGoStraight:
			if (m_CarController->GetNextJunction()->GetStraightRoad(m_CarController->GetCurrentRoad()))
			{
				return;
			}
			break;

		case eCarJunctionTurnRight:
			if (m_CarController->GetNextJunction()->GetRightRoad(m_CarController->GetCurrentRoad()))
			{
				return;
			}
			break;

		case eCarJunctionTurnLeft:
			if (m_CarController->GetNextJunction()->GetLeftRoad(m_CarController->GetCurrentRoad()))
			{
				return;
			}
			break;

		default:
			break;
		}
	}
}

void CarStateNearNextJunction::Update(void)
{
	//ブレーキ
	m_CarController->BrakeSpeed();

	//交差点を曲がるor直進
	switch (m_JunctionSelect)
	{
	case eCarJunctionGoStraight:
		if (m_CarController->CanGoStraight())
		{
			if (m_CarController->GoStraight())
			{
				m_CarController->ChangeState( eCarStateMoveTowardRoad);
			}
		}
		break;

	case eCarJunctionTurnRight:
		if (m_CarController->CanTurnRight())
		{
			if (m_CarController->TurnRight())
			{
				m_CarController->ChangeState( eCarStateMoveTowardRoad);
			}
		}
		break;

	case eCarJunctionTurnLeft:
		if (m_CarController->CanTurnLeft())
		{
			if (m_CarController->TurnLeft())
			{
				m_CarController->ChangeState( eCarStateMoveTowardRoad);
			}
		}
		break;

	default:
		break;
	}
}


