/*==============================================================================
	
	CarController.h - ��ʃV�X�e���[��
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "CarController.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "TrafficRoad.h"
#include "TrafficJunction.h"
#include "TrafficLand.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define FRICTION_SPEED_RATE (0.15f)
#define BRAKE_SPEED_RATE (0.5f)
#define ON_ROAD_PERMIT_RATE (0.1f)

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* CarController::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<CarController>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
CarController::CarController( GameObject* pGameObject) : m_Speed( 0.0f)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_CurrentRoad = NULL;
	m_NextJunction = NULL;
	m_TargetLand = NULL;

	//�X�e�[�g�̐ݒ�
	m_States[ eCarStateNeutral] = new CarStateNeutral( this);
	m_States[ eCarStateMoveOnRoad] = new CarStateMoveOnRoad( this);
	m_States[ eCarStateMoveTowardRoad] = new CarStateMoveTowardRoad( this);
	m_States[ eCarStateNearNextJunction] = new CarStateNearNextJunction( this);
	m_CurrentState = m_States[ eCarStateMoveTowardRoad];
	//m_CurrentState = m_States[ eCarStateNeutral];

	//�`��̐ݒ�
	auto renderer = m_pGameObject->AddComponent<XModelRenderer>();
	renderer->LoadXModel( "data/MODEL/car.x");
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void CarController::Init(TrafficRoad* currentRoad, TrafficJunction* nextJunction, TrafficLand* targetLand)
{
	m_CurrentRoad = currentRoad;
	m_NextJunction = nextJunction;
	m_TargetLand = targetLand;
}


/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void CarController::Uninit( void)
{
	//�X�e�[�g�̉��
	for (auto state : m_States)
	{
		delete state.second;
	}
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void CarController::Update( void)
{
	//�X�e�[�g�̍X�V
	m_CurrentState->Update();

	//���ʕ����Ɉړ�
	m_pTransform->Move( m_pTransform->GetForward() * m_Speed);
	
	//���C�ɂ�錸��
	m_Speed -= ( m_Speed * FRICTION_SPEED_RATE);
}

/*------------------------------------------------------------------------------
	���H�̃x�N�g�����擾
------------------------------------------------------------------------------*/
Vector3 CarController::GetCurrentRoadVector(void)
{
	return m_CurrentRoad->GetVector( m_NextJunction);
}

/*------------------------------------------------------------------------------
	�O���̎Ԃ��擾
------------------------------------------------------------------------------*/
CarController* CarController::GetFrontCar(void)
{
	return m_CurrentRoad->GetFrontCar( this);
}

/*------------------------------------------------------------------------------
	�Ԃ����H��𑖍s���Ă��邩
------------------------------------------------------------------------------*/
bool CarController::CheckOnRoad(void)
{
	//�������Ԑ����ɂ��邩
	if (!IsCorrectSideRoad())
	{
		return false;
	}

	//�Ԑ��̒����𑖍s���Ă��邩
	float length = CulcLengthFromRoadCenter();
	float roadWidthHalf = m_CurrentRoad->GetWidth() * 0.5f;
	float permitWidth = roadWidthHalf * ON_ROAD_PERMIT_RATE;
	if (length > roadWidthHalf + permitWidth || length < roadWidthHalf - permitWidth)
	{
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------
	�Ԃ��������Ԑ����ɂ��邩
------------------------------------------------------------------------------*/
bool CarController::IsCorrectSideRoad( void)
{
	//�O�ς�p���Ĕ���
	auto otherJunctionPosition = m_CurrentRoad->GetOtherJunction( m_NextJunction)->GetPosition();
	auto roadVector = GetCurrentRoadVector();
	Vector3 vector = m_pTransform->GetWorldPosition() - otherJunctionPosition;
	if (Vector3::Cross(vector, roadVector).y >= 0.0f)
	{
		return true;
	}
	
	return false;
}

/*------------------------------------------------------------------------------
	���H�̒���������̋������Z�o
------------------------------------------------------------------------------*/
float CarController::CulcLengthFromRoadCenter(void)
{
	//�_�Ɛ����̋��������߂�
	auto otherJunctionPosition = m_CurrentRoad->GetOtherJunction( m_NextJunction)->GetPosition();
	auto roadVector = GetCurrentRoadVector();
	Vector3 vector = m_pTransform->GetWorldPosition() - otherJunctionPosition;

	auto heightVector = roadVector * Vector3::Dot( roadVector.Normalize(), vector) / roadVector.Length() - vector;

	return heightVector.Length();
}

/*------------------------------------------------------------------------------
	���H�̏���Ԃ��i�񂾋������Z�o����
------------------------------------------------------------------------------*/
float CarController::CulcMoveDistanceOnRoad(void)
{
	//�u���K�������x�N�g��N�ƔC�Ӄx�N�g��A�Ƃ̓��ς́AA��N�ɓ��e�������̒����i�����t���j�ƂȂ�v�����𗘗p����
	auto otherJunctionPosition = m_CurrentRoad->GetOtherJunction( m_NextJunction)->GetPosition();
	auto roadVector = GetCurrentRoadVector();
	Vector3 vector = m_pTransform->GetWorldPosition() - otherJunctionPosition;
	float length = Vector3::Dot( roadVector.Normalize(), vector);

	return length;
}

/*------------------------------------------------------------------------------
	���H�̏���Ԃ��i�񂾋����̊������Z�o����
------------------------------------------------------------------------------*/
float CarController::CulcMoveRateOnRoad(void)
{
	//�u���K�������x�N�g��N�ƔC�Ӄx�N�g��A�Ƃ̓��ς́AA��N�ɓ��e�������̒����i�����t���j�ƂȂ�v�����𗘗p����
	auto otherJunctionPosition = m_CurrentRoad->GetOtherJunction( m_NextJunction)->GetPosition();
	auto roadVector = GetCurrentRoadVector();
	Vector3 vector = m_pTransform->GetWorldPosition() - otherJunctionPosition;
	float length = Vector3::Dot( roadVector.Normalize(), vector);

	//�����ɂ��邽�߂ɒ����Ŋ���
	float rate = length / roadVector.Length();

	return rate;
}

/*------------------------------------------------------------------------------
	�Ԃ����̌����_�܂łɎc���Ă��鋗�����Z�o����
------------------------------------------------------------------------------*/
float CarController::CulcRemainDistanceOnRoad(void)
{
	//�u���K�������x�N�g��N�ƔC�Ӄx�N�g��A�Ƃ̓��ς́AA��N�ɓ��e�������̒����i�����t���j�ƂȂ�v�����𗘗p����
	auto otherJunctionPosition = m_CurrentRoad->GetOtherJunction( m_NextJunction)->GetPosition();
	auto roadVector = GetCurrentRoadVector();
	Vector3 vector = m_pTransform->GetWorldPosition() - otherJunctionPosition;
	float length = Vector3::Dot( roadVector.Normalize(), vector);

	return roadVector.Length() - length;
}

/*------------------------------------------------------------------------------
	�E�ɂ��铹�H���擾
------------------------------------------------------------------------------*/
TrafficRoad* CarController::GetRightRoad(void)
{
	return m_NextJunction->GetRightRoad( m_CurrentRoad);
}

/*------------------------------------------------------------------------------
	���ɂ��铹�H���擾
------------------------------------------------------------------------------*/
TrafficRoad* CarController::GetLeftRoad(void)
{
	return m_NextJunction->GetLeftRoad( m_CurrentRoad);
}

/*------------------------------------------------------------------------------
	���i���铹�H���擾
------------------------------------------------------------------------------*/
TrafficRoad* CarController::GetStraightRoad(void)
{
	return m_NextJunction->GetStraightRoad( m_CurrentRoad);
}

/*------------------------------------------------------------------------------
	�E�܂ł��邩
------------------------------------------------------------------------------*/
bool CarController::CanTurnRight(void)
{
	return m_NextJunction->CanTurnRight( m_CurrentRoad);
}

/*------------------------------------------------------------------------------
	���܂ł��邩
------------------------------------------------------------------------------*/
bool CarController::CanTurnLeft(void)
{
	return m_NextJunction->CanTurnLeft( m_CurrentRoad);
}

/*------------------------------------------------------------------------------
	���i�ł��邩
------------------------------------------------------------------------------*/
bool CarController::CanGoStraight(void)
{
	return m_NextJunction->CanGoStraight( m_CurrentRoad);
}

/*------------------------------------------------------------------------------
	�E��
------------------------------------------------------------------------------*/
bool CarController::TurnRight(void)
{
	//�ύX��̓��H���擾
	auto nextRoad = m_NextJunction->GetRightRoad( m_CurrentRoad);
	if (!nextRoad)
	{
		return false;
	}

	//�����_��ύX
	m_NextJunction = nextRoad->GetOtherJunction( m_NextJunction);

	//���H��ύX
	m_CurrentRoad->UnregisterCar( this);
	m_CurrentRoad = nextRoad;
	nextRoad->RegisterCar( this);

	return true;
}

/*------------------------------------------------------------------------------
	����
------------------------------------------------------------------------------*/
bool CarController::TurnLeft(void)
{
	//�ύX��̓��H���擾
	auto nextRoad = m_NextJunction->GetLeftRoad( m_CurrentRoad);
	if (!nextRoad)
	{
		return false;
	}

	//�����_��ύX
	m_NextJunction = nextRoad->GetOtherJunction( m_NextJunction);

	//���H��ύX
	m_CurrentRoad->UnregisterCar( this);
	m_CurrentRoad = nextRoad;
	nextRoad->RegisterCar( this);

	return true;
}

/*------------------------------------------------------------------------------
	���i
------------------------------------------------------------------------------*/
bool CarController::GoStraight(void)
{
	//�ύX��̓��H���擾
	auto nextRoad = m_NextJunction->GetStraightRoad( m_CurrentRoad);
	if (!nextRoad)
	{
		return false;
	}

	//�����_��ύX
	m_NextJunction = nextRoad->GetOtherJunction( m_NextJunction);

	//���H��ύX
	m_CurrentRoad->UnregisterCar( this);
	m_CurrentRoad = nextRoad;
	nextRoad->RegisterCar( this);

	return true;
}

/*------------------------------------------------------------------------------
	��ԑJ��
------------------------------------------------------------------------------*/
void CarController::ChangeState(ECarState next)
{
	m_CurrentState = m_States[ next];
	m_CurrentState->Init();
}

/*------------------------------------------------------------------------------
	�u���[�L�𓥂�
------------------------------------------------------------------------------*/
void CarController::BrakeSpeed(void)
{
	m_Speed -= ( m_Speed * BRAKE_SPEED_RATE);
}

/*------------------------------------------------------------------------------
	���s���̓��H�������̏���
------------------------------------------------------------------------------*/
void CarController::OnDeleteCurrentRoad(void)
{
	m_CurrentRoad = NULL;
	SetActive( false);
}

/*------------------------------------------------------------------------------
	���H����O���
------------------------------------------------------------------------------*/
void CarController::OutRoad(void)
{
	if( m_CurrentRoad)
	{
		m_CurrentRoad->UnregisterCar( this);
		m_CurrentRoad = NULL;
	}
	SetActive( false);
}
