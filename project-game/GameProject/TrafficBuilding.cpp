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
#include "CarFactory.h"
#include "CityAttribute.h"

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

	m_CarFactory = NULL;
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
	�������瓹�H��ݒ�
------------------------------------------------------------------------------*/
void TrafficBuilding::SetRoads(std::list<RoadAttribute*> roadAttributes)
{
	for (auto roadAttribute : roadAttributes)
	{
		m_Roads.push_back( roadAttribute->GetTrafficRoad());
	}
}

/*------------------------------------------------------------------------------
	CarFactory��ݒ�
------------------------------------------------------------------------------*/
void TrafficBuilding::SetCarFactory(CarFactory* carFactory)
{
	m_CarFactory = carFactory;
}

/*------------------------------------------------------------------------------
	�Ԃ𐶐�
------------------------------------------------------------------------------*/
CarController* TrafficBuilding::CreateCar(void)
{
	if (!m_CarFactory)
	{
		return NULL;
	}

	//TODO:

	return NULL;
}

/*------------------------------------------------------------------------------
	�Ԃ���ɂ�����
------------------------------------------------------------------------------*/
void TrafficBuilding::EnterCar(CarController* car)
{
	//TODO:

}

/*------------------------------------------------------------------------------
	�Ԃ��o�ɂ�����
------------------------------------------------------------------------------*/
CarController* TrafficBuilding::ExitCar(CarController* car)
{
	//TODO:

	return NULL;
}

