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
#include "TrafficBuilding.h"

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
CarController::CarController( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_CurrentRoad = NULL;
	m_NextJunction = NULL;
	m_TargetBuilding = NULL;

	//�X�e�[�g�̐ݒ�
	m_States[ eCarStateNeutral] = new CarStateNeutral();
	
	m_CurrentState = m_States[ eCarStateNeutral];
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void CarController::Init(TrafficRoad* currentRoad, TrafficJunction* nextJunction, TrafficBuilding* targetBuilding)
{
	m_CurrentRoad = currentRoad;
	m_NextJunction = nextJunction;
	m_TargetBuilding = targetBuilding;

	//TODO:���݈ʒu�ݒ�

	//TODO:�`��̐ݒ�

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
	
}

/*------------------------------------------------------------------------------
	���H�̏���Ԃ��i�񂾋����̊������Z�o����
------------------------------------------------------------------------------*/
float CarController::CulcMoveRateOnRoad(void)
{
	//�u���K�������x�N�g��N�ƔC�Ӄx�N�g��A�Ƃ̓��ς́AA��N�ɓ��e�������̒����i�����t���j�ƂȂ�v�����𗘗p����
	auto otherJunctionPosition = m_CurrentRoad->GetOtherJunction( m_NextJunction)->GetPosition();
	auto roadVector = m_CurrentRoad->GetVector( m_NextJunction);
	Vector3 vector = m_pTransform->GetWorldPosition() - otherJunctionPosition;
	float length = Vector3::Dot( roadVector.Normalize(), vector);

	//�����ɂ��邽�߂ɒ����Ŋ���
	float rate = length / roadVector.Length();

	return rate;
}
