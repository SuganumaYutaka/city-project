/*==============================================================================

    Car.cpp - ���̎��������[��ʃV�X�e���[��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Car.h"
#include "GameObject.h"
#include "CarManager.h"
#include "CarController.h"
#include "TrafficLand.h"
#include "TrafficRoad.h"
#include "TrafficJunction.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Car::Car(CarManager* manager, GameObject* parent)
{
	m_GameObject = new GameObject( parent);
	m_CarController = NULL;
	
	m_Manager = manager;
	m_Manager->RegisterCar( this);
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Car::~Car()
{
	
}

/*------------------------------------------------------------------------------
	����
------------------------------------------------------------------------------*/
void Car::Delete(void)
{
	m_GameObject->ReleaseReserve();
	m_Manager->UnregisterCar( this);
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void Car::Init(TrafficLand* spawnLand, TrafficLand* targetLand)
{
	if (!m_CarController)
	{
		m_CarController = m_GameObject->AddComponent<CarController>();
	}

	//�o���ʒu���瓹�H�ƌ����_���擾
	auto road = spawnLand->GetTrafficRoad().front();
	auto junction = road->GetJunctions().front();

	//�Ԃ̏�����
	m_CarController->Init( road, junction, targetLand);
	m_GameObject->m_pTransform->SetWorldPosition( spawnLand->m_pTransform->GetWorldPosition());

}


