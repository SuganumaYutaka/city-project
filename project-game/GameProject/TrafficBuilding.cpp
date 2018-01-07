/*==============================================================================
	
	TrafficBuilding.cpp - ��ʃV�X�e���[�����_
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "TrafficBuilding.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "CityAttribute.h"

#include "Edge.h"
#include "Vertex.h"
#include "HalfEdge.h"

#include "TrafficRoad.h"
#include "CarController.h"
#include "CityAttribute.h"
#include "CarManager.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* TrafficBuilding::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<TrafficBuilding>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
TrafficBuilding::TrafficBuilding( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_CarManager = NULL;
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void TrafficBuilding::Init(std::list<RoadAttribute*> roadAttributes, CarManager* carManager)
{
	for (auto roadAttribute : roadAttributes)
	{
		m_Roads.push_back( roadAttribute->GetTrafficRoad());
	}
	m_CarManager = carManager;
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void TrafficBuilding::Uninit( void)
{

}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void TrafficBuilding::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	�Ԃ𐶐�
------------------------------------------------------------------------------*/
CarController* TrafficBuilding::CreateCar(void)
{
	if (!m_CarManager)
	{
		return NULL;
	}

	if (m_Roads.size() == 0)
	{
		return NULL;
	}

	//�Ԃ𐶐�
	auto road = m_Roads.front();
	auto junction = road->GetJunctions().front();
	auto carController = m_CarManager->CreateCarController( m_pTransform->GetWorldPosition(), m_pTransform->GetWorldRotation()
		,road, junction, this);

	//���ɂ�����
	//EnterCar( carController);

	//�����ɏo�ɂ�����
	road->RegisterCar( carController);

	return carController;
}

/*------------------------------------------------------------------------------
	�Ԃ���ɂ�����
------------------------------------------------------------------------------*/
void TrafficBuilding::EnterCar(CarController* car)
{
	if (!IsCarInBuilding( car))
	{
		//���ɂ�����
		m_Cars.push_back( car);

		//���H�̓o�^����������
		car->GetCurrentRoad()->UnregisterCar( car);
	}
}

/*------------------------------------------------------------------------------
	�Ԃ��o�ɂ�����
------------------------------------------------------------------------------*/
bool TrafficBuilding::ExitCar(CarController* car)
{
	for (auto ite = m_Cars.begin(); ite != m_Cars.end(); ++ite)
	{
		if (*ite == car)
		{
			//���H�ɓo�^����
			car->GetCurrentRoad()->RegisterCar( car);

			//�o�ɂ�����
			m_Cars.erase( ite);
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
	��ɓ��ꂽ�Ԃ��o�ɂ�����
------------------------------------------------------------------------------*/
CarController* TrafficBuilding::PopFrontCar(void)
{
	if (m_Cars.size() == 0)
	{
		return NULL;
	}

	auto car = m_Cars.front();

	//���H�ɓo�^����
	car->GetCurrentRoad()->RegisterCar( car);

	//�o�ɂ�����
	m_Cars.pop_front();

	return car;
}

/*------------------------------------------------------------------------------
	��ɓ��ꂽ�Ԃ��o�ɂ�����
------------------------------------------------------------------------------*/
CarController* TrafficBuilding::PopBackCar(void)
{
	if (m_Cars.size() == 0)
	{
		return NULL;
	}
	auto car = m_Cars.back();

	//���H�ɓo�^����
	car->GetCurrentRoad()->RegisterCar( car);

	//�o�ɂ�����
	m_Cars.pop_back();

	return car;
}

/*------------------------------------------------------------------------------
	����̎Ԃ����ɂ��Ă��邩�m�F����
------------------------------------------------------------------------------*/
bool TrafficBuilding::IsCarInBuilding(CarController* car)
{
	for (auto carInBuilding : m_Cars)
	{
		if (carInBuilding == car)
		{
			return true;
		}
	}

	return false;
}


