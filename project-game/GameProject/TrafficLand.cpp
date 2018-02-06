/*==============================================================================
	
	TrafficLand.cpp - ��ʃV�X�e���[�y�n
														Author : Yutaka Suganuma
														Date   : 2018/2/3
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "TrafficLand.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "CityAttribute.h"

#include "Edge.h"
#include "Vertex.h"
#include "HalfEdge.h"

#include "TrafficRoad.h"
#include "CarController.h"
#include "CityAttribute.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* TrafficLand::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<TrafficLand>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
TrafficLand::TrafficLand( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void TrafficLand::Init(std::list<RoadAttribute*> roadAttributes)
{
	for (auto roadAttribute : roadAttributes)
	{
		m_Roads.push_back( roadAttribute->GetTrafficRoad());
	}
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void TrafficLand::Uninit( void)
{

}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void TrafficLand::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	�Ԃ���ɂ�����
------------------------------------------------------------------------------*/
void TrafficLand::EnterCar(CarController* car)
{
	if (!IsCarInLand( car))
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
bool TrafficLand::ExitCar(CarController* car)
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
CarController* TrafficLand::PopFrontCar(void)
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
CarController* TrafficLand::PopBackCar(void)
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
bool TrafficLand::IsCarInLand(CarController* car)
{
	for (auto carInLand : m_Cars)
	{
		if (carInLand == car)
		{
			return true;
		}
	}

	return false;
}


