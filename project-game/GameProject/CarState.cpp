/*==============================================================================
	
	CarState.cpp - ��ʃV�X�e���[�Ԃ̏��
														Author : Yutaka Suganuma
														Date   : 2017/12/30
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "CarState.h"
#include "CarController.h"
#include "TrafficRoad.h"
#include "TrafficJunction.h"
#include "TrafficBuilding.h"
#include "ComponentInclude.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define ADD_SPEED_ON_ROAD (0.09f)
#define ADD_SPEED_TOWARD_ROAD (0.07f)
#define ADD_SPEED_NEAR_JUNCTION ( 0.03f)
#define SPEED_SIDEWAY ( 0.03f)
#define MAX_DISTANCE_FRONT_CAR ( 10.0f)
#define MIN_DISTANCE_FRONT_CAR ( 5.0f)
#define SLERP_VALUE (0.2f)

/*------------------------------------------------------------------------------
	State : �j���[�g����
------------------------------------------------------------------------------*/
void CarStateNeutral::Update(void)
{

}


/*------------------------------------------------------------------------------
	State : ���H��𑖍s
------------------------------------------------------------------------------*/
void CarStateMoveOnRoad::Init(void)
{

}

void CarStateMoveOnRoad::Update(void)
{
	//���H�̕����ƎԂ̕��������킹��
	auto roadVector = m_CarController->GetCurrentRoadVector();
	m_CarController->m_pTransform->SetWorldRotationLookDirection( roadVector);

	//���݈ʒu���Ԑ��̒�������O��Ă���ꍇ�C��
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

	//�O�̎ԂƂ̎Ԋԋ����ɉ����ĉ����E����
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

	//�����_�ɋ߂��ꍇ�͏�ԑJ��
	float lengthToNextJunction = m_CarController->CulcRemainDistanceOnRoad();
	if (lengthToNextJunction < m_CarController->GetCurrentRoad()->GetWidth() * 0.75f)
	{
		m_CarController->ChangeState( eCarStateNearNextJunction);
	}
}

/*------------------------------------------------------------------------------
	State : ���H�ɍ������邽�߂ɑ��s
------------------------------------------------------------------------------*/
void CarStateMoveTowardRoad::Init(void)
{

}

void CarStateMoveTowardRoad::Update(void)
{
	//����
	m_CarController->AddSpeed( ADD_SPEED_TOWARD_ROAD);

	//�Ԑ��ɐڋ߂��Ă���Ԑ�������Slerp
	auto roadVector = m_CarController->GetCurrentRoadVector();
	float length = m_CarController->CulcLengthFromRoadCenter();
	float widthHalf = m_CarController->GetCurrentRoad()->GetWidth() * 0.5f;
	if (!m_CarController->IsCorrectSideRoad())
	{
		//�Ԑ��ɐڋ�
		Vector3 sidewayVector = Vector3::Cross( roadVector, Vector3(0.0f, 1.0f, 0.0f));
		m_CarController->m_pTransform->RotateLookDirection( sidewayVector, SLERP_VALUE);
	}
	else if (length > widthHalf)
	{
		//�Ԑ��ɐڋ�
		Vector3 sidewayVector = Vector3::Cross( roadVector, Vector3(0.0f, 1.0f, 0.0f)) * -1.0f;
		m_CarController->m_pTransform->RotateLookDirection( sidewayVector, SLERP_VALUE);
	}
	else if (Vector3::Dot( roadVector.Normalize(), m_CarController->m_pTransform->GetForward()) > 0.95f)
	{
		//�Ȃ���؂��č������I��������ԑJ��
		m_CarController->ChangeState( eCarStateMoveOnRoad);
	}
	else
	{
		//�Ԑ�������Slerp
		m_CarController->m_pTransform->RotateLookDirection( roadVector, SLERP_VALUE);
	}
}

/*------------------------------------------------------------------------------
	State : ���̌����_�ɐڋ߂������
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
	//�����_��i�ޕ����̌���
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
	//�u���[�L
	m_CarController->BrakeSpeed();

	//�����_���Ȃ���or���i
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


