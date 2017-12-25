/*==============================================================================
	
	TrafficRoad.cpp - ��ʃV�X�e���[�����_
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "TrafficRoad.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "CityAttribute.h"

#include "Edge.h"
#include "Vertex.h"
#include "HalfEdge.h"

#include "TrafficJunction.h"
#include "CarController.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* TrafficRoad::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<TrafficRoad>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
TrafficRoad::TrafficRoad( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Attribute = NULL;
	m_IsUpdatedAttribute = false;

	m_LeftSideCars.clear();
	m_RightSideCars.clear();
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void TrafficRoad::Uninit( void)
{
	m_Attribute->UnregisterView();
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void TrafficRoad::Update( void)
{
	if (m_IsUpdatedAttribute)
	{

		m_IsUpdatedAttribute = false;
	}

	

}

/*------------------------------------------------------------------------------
	�������̐ݒ�
------------------------------------------------------------------------------*/
void TrafficRoad::SetAttribute( RoadAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;
	m_IsUpdatedAttribute = true;
}

/*------------------------------------------------------------------------------
	�Ԃ̓o�^
------------------------------------------------------------------------------*/
void TrafficRoad::RegisterCar(CarController* car, TrafficJunction* nextJunction)
{
	if ( !m_Attribute || !nextJunction->GetAttribute())
	{
		return;
	}

	UnregisterCar( car);

	//���ɓ��B��������_�̏�񂩂獶�E�̂ǂ����ʍs���邩����
	if (nextJunction->GetAttribute()->GetVertex() == m_Attribute->GetEdge()->GetEnd())
	{
		m_LeftSideCars.push_back( car);
	}
	else
	{
		m_RightSideCars.push_back( car);
	}
}
	
/*------------------------------------------------------------------------------
	�Ԃ̓o�^������
------------------------------------------------------------------------------*/
void TrafficRoad::UnregisterCar(CarController* car)
{
	for (auto ite = m_LeftSideCars.begin(); ite != m_LeftSideCars.end(); ++ite)
	{
		if (*ite == car)
		{
			m_LeftSideCars.erase( ite);
			return;
		}
	}
	for (auto ite = m_RightSideCars.begin(); ite != m_RightSideCars.end(); ++ite)
	{
		if (*ite == car)
		{
			m_RightSideCars.erase( ite);
			return;
		}
	}
}

/*------------------------------------------------------------------------------
	�Ԃ̎擾
------------------------------------------------------------------------------*/
std::list<CarController*>* TrafficRoad::GetCars(TrafficJunction* nextJunction)
{
	if ( !m_Attribute || !nextJunction->GetAttribute())
	{
		return NULL;
	}

	//���ɓ��B��������_�̏�񂩂獶�E�̂ǂ����ʍs���邩����
	if (nextJunction->GetAttribute()->GetVertex() == m_Attribute->GetEdge()->GetEnd())
	{
		return &m_LeftSideCars;
	}
	else
	{
		return &m_RightSideCars;
	}

	return NULL;
}
