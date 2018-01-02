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
void TrafficRoad::RegisterCar(CarController* car)
{
	UnregisterCar( car);

	//���ɓ��B��������_�̏�񂩂獶�E�̂ǂ����ʍs���邩����
	if (car->GetNextJunction()->GetAttribute()->GetVertex() == m_Attribute->GetEdge()->GetEnd())
	{
		//�n�_���猩�č��Ԑ��ɑ}��
		if (m_LeftSideCars.size() == 0)
		{
			m_LeftSideCars.push_back( car);
			return;
		}

		//�}���n�_�̎n�_����̋����i�i�񂾋����j�ɉ����ēr���ɑ}��
		float insertRate = car->CulcMoveRateOnRoad();
		for (auto ite = m_LeftSideCars.begin(); ite != m_LeftSideCars.end(); ++ite)
		{
			float rate = (*ite)->CulcMoveRateOnRoad();
			if (insertRate <= rate)
			{
				m_LeftSideCars.insert( ite, car);
				return;
			}
		}

		//�Ō���ɑ}��
		m_LeftSideCars.push_back( car);
		return;
	}
	else
	{
		//�n�_���猩�ĉE�Ԑ��ɑ}��
		if (m_RightSideCars.size() == 0)
		{
			m_RightSideCars.push_back( car);
			return;
		}

		//�}���n�_�̎n�_����̋����i�i�񂾋����j�ɉ����ēr���ɑ}��
		float insertRate = car->CulcMoveRateOnRoad();
		for (auto ite = m_RightSideCars.begin(); ite != m_RightSideCars.end(); ++ite)
		{
			float rate = (*ite)->CulcMoveRateOnRoad();
			if (insertRate <= rate)
			{
				m_RightSideCars.insert( ite, car);
				return;
			}
		}

		//�Ō���ɑ}��
		m_RightSideCars.push_back( car);
		return;
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
const std::list<CarController*>& TrafficRoad::GetCars(TrafficJunction* nextJunction)
{
	//���ɓ��B��������_�̏�񂩂獶�E�̂ǂ����ʍs���邩����
	if (nextJunction->GetAttribute()->GetVertex() == m_Attribute->GetEdge()->GetEnd())
	{
		return m_LeftSideCars;
	}
	return m_RightSideCars;
}

/*------------------------------------------------------------------------------
	�����_�̎擾
------------------------------------------------------------------------------*/
std::list<TrafficJunction*> TrafficRoad::GetJunctions(void)
{
	std::list<TrafficJunction*> junctions;
	JunctionAttribute* junctionAttribute = (JunctionAttribute*)m_Attribute->GetEdge()->GetStart()->GetAttribute();
	junctions.push_back( junctionAttribute->GetTrafficJunction());
	junctionAttribute = (JunctionAttribute*)m_Attribute->GetEdge()->GetEnd()->GetAttribute();
	junctions.push_back( junctionAttribute->GetTrafficJunction());

	return junctions;
}

/*------------------------------------------------------------------------------
	���Α��̌����_�̎擾
------------------------------------------------------------------------------*/
TrafficJunction* TrafficRoad::GetOtherJunction(TrafficJunction* junction)
{
	JunctionAttribute* junctionAttribute = (JunctionAttribute*)m_Attribute->GetEdge()->GetStart()->GetAttribute();
	auto startJunction = junctionAttribute->GetTrafficJunction();
	junctionAttribute = (JunctionAttribute*)m_Attribute->GetEdge()->GetEnd()->GetAttribute();
	auto endJunction = junctionAttribute->GetTrafficJunction();

	if (startJunction == junction)
	{
		return endJunction;
	}
	else if (endJunction == junction)
	{
		return startJunction;
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	�x�N�g���̎擾
------------------------------------------------------------------------------*/
Vector3 TrafficRoad::GetVector(TrafficJunction* nextJunction)
{
	JunctionAttribute* junctionAttribute = (JunctionAttribute*)m_Attribute->GetEdge()->GetStart()->GetAttribute();
	auto startJunction = junctionAttribute->GetTrafficJunction();

	if (nextJunction == startJunction)
	{
		return m_Attribute->GetVector() * -1.0f;
	}
	
	return m_Attribute->GetVector();
}

