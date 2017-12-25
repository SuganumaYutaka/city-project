/*==============================================================================
	
	TrafficJunction.cpp - ��ʃV�X�e���[�����_
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "TrafficJunction.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "CityAttribute.h"

#include "Edge.h"
#include "Vertex.h"
#include "HalfEdge.h"

#include "TrafficRoad.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* TrafficJunction::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<TrafficJunction>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
TrafficJunction::TrafficJunction( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Attribute = NULL;
	m_IsUpdatedAttribute = false;

	m_Roads.clear();
	m_CanMoveVertical = false;
	m_CanMoveHorizontal = false;
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void TrafficJunction::Uninit( void)
{
	m_Attribute->UnregisterView();
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void TrafficJunction::Update( void)
{
	if (m_IsUpdatedAttribute)
	{
		UpdateJunction();

		m_IsUpdatedAttribute = false;
	}

	

}

/*------------------------------------------------------------------------------
	�������̐ݒ�
------------------------------------------------------------------------------*/
void TrafficJunction::SetAttribute( JunctionAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;
	m_IsUpdatedAttribute = true;
}

/*------------------------------------------------------------------------------
	�����_���̍X�V
------------------------------------------------------------------------------*/
void TrafficJunction::UpdateJunction(void)
{
	auto edges = m_Attribute->GetVertex()->GetEdges();
	if (edges.size() <= 0)
	{
		return;
	}
	
	//�k���A�s���A�\���̐ݒ�
	RoadAttribute* roadAttribute = NULL;

	switch (edges.size())
	{
	case 2:		//�k��
	{
		m_Roads.resize( 2);

		//�x�N�g���̎擾
		auto edgeBegin = edges.begin();
		Vector3 vec1 = (*edgeBegin)->GetVector();
		if (!(*edgeBegin)->GetRight())
		{
			vec1 *= -1.0f;
		}
		++edgeBegin;
		Vector3 vec2 = (*edgeBegin)->GetVector();
		if (!(*edgeBegin)->GetRight())
		{
			vec2 *= -1.0f;
		}

		//�O�ςɉ����Đݒ�
		edgeBegin = edges.begin();
		if (Vector3::Cross(vec1, vec2).y < 0.0f)
		{
			roadAttribute = (RoadAttribute*)((*edgeBegin)->GetAttribute());
			m_Roads[ eJunctionBranchBack] = roadAttribute->GetTrafficRoad();
			++edgeBegin;
			roadAttribute = (RoadAttribute*)((*edgeBegin)->GetAttribute());
			m_Roads[ eJunctionBranchRight] = roadAttribute->GetTrafficRoad();
		}
		else
		{
			roadAttribute = (RoadAttribute*)((*edgeBegin)->GetAttribute());
			m_Roads[ eJunctionBranchRight] = roadAttribute->GetTrafficRoad();
			++edgeBegin;
			roadAttribute = (RoadAttribute*)((*edgeBegin)->GetAttribute());
			m_Roads[ eJunctionBranchBack] = roadAttribute->GetTrafficRoad();
		}
		
		break;
	}

	case 3:		//�s��
	{
		m_Roads.resize( 3);

		//�����������Ă��铹�H���Ȃ����H(Back)���擾
		Edge* targetEdge = NULL;
		for (auto edge1 : edges)
		{
			bool isTarget = true;

			//���ӂƌ����������Ă��邩
			for(auto edge2 : edges)
			{
				if (edge1 != edge2)
				{
					auto dot = Vector3::Dot(edge1->GetVector().Normalize(), edge2->GetVector().Normalize());
					if ( dot < -0.5f || dot > 0.5f)
					{
						isTarget = false;
						break;
					}
				}
			}

			if (isTarget)
			{
				targetEdge = edge1;
				break;
			}
		}
		roadAttribute = (RoadAttribute*)(targetEdge->GetAttribute());
		m_Roads[ eJunctionBranchBack] = roadAttribute->GetTrafficRoad();

		//�O�ς��獶�E�̓��H��ݒ�
		Vector3 vec1 = targetEdge->GetVector();
		if ( targetEdge->GetEnd() == m_Attribute->GetVertex())
		{
			vec1 *= -1.0f;
		}
		for (auto edge : edges)
		{
			if (edge == targetEdge)
			{
				continue;
			}

			Vector3 vec2 = edge->GetVector();
			if ( edge->GetEnd() == m_Attribute->GetVertex())
			{
				vec2 *= -1.0f;
			}
			
			if (Vector3::Cross(vec1, vec2).y > 0.0f)
			{
				roadAttribute = (RoadAttribute*)(edge->GetAttribute());
				m_Roads[ eJunctionBranchLeft] = roadAttribute->GetTrafficRoad();
			}
			else
			{
				roadAttribute = (RoadAttribute*)(edge->GetAttribute());
				m_Roads[ eJunctionBranchRight] = roadAttribute->GetTrafficRoad();
			}
		}

		break;
	}

	case 4:		//�\��
	{
		m_Roads.resize( 4);

		//begin����ɓ��ςƊO�ς�4�����ɐݒ�
		auto edgeBegin = edges.begin();
		roadAttribute = (RoadAttribute*)((*edgeBegin)->GetAttribute());
		m_Roads[ eJunctionBranchBack] = roadAttribute->GetTrafficRoad();
		Vector3 vec1 = (*edgeBegin)->GetVector();
		if ( (*edgeBegin)->GetEnd() == m_Attribute->GetVertex())
		{
			vec1 *= -1.0f;
		}
		for (auto edge : edges)
		{
			if (edge == (*edgeBegin))
			{
				continue;
			}

			Vector3 vec2 = edge->GetVector();
			if ( edge->GetEnd() == m_Attribute->GetVertex())
			{
				vec2 *= -1.0f;
			}
			
			//���ςŔ��Α�
			auto dot = Vector3::Dot( vec1.Normalize(), vec2.Normalize());
			if (dot < -0.5f)
			{
				roadAttribute = (RoadAttribute*)(edge->GetAttribute());
				m_Roads[ eJunctionBranchFront] = roadAttribute->GetTrafficRoad();
				continue;
			}

			//�O�ςō��E
			if (Vector3::Cross(vec1, vec2).y > 0.0f)
			{
				roadAttribute = (RoadAttribute*)(edge->GetAttribute());
				m_Roads[ eJunctionBranchLeft] = roadAttribute->GetTrafficRoad();
			}
			else
			{
				roadAttribute = (RoadAttribute*)(edge->GetAttribute());
				m_Roads[ eJunctionBranchRight] = roadAttribute->GetTrafficRoad();
			}
		}

		break;
	}

	default:
		break;
	}
}

/*------------------------------------------------------------------------------
	�E�܂ł��邩
------------------------------------------------------------------------------*/
bool TrafficJunction::CanTurnRight(TrafficRoad* road)
{
	//TODO:�t���O�ƑΌ��Ԃ��`�F�b�N

	return true;
}

/*------------------------------------------------------------------------------
	���܂ł��邩
------------------------------------------------------------------------------*/
bool TrafficJunction::CanTurnLeft(TrafficRoad* road)
{
	//TODO:�t���O�ƑΌ��Ԃ��`�F�b�N

	return true;
}

/*------------------------------------------------------------------------------
	���i�ł��邩
------------------------------------------------------------------------------*/
bool TrafficJunction::CanGoStraight(TrafficRoad* road)
{
	//TODO:�t���O�ƑΌ��Ԃ��`�F�b�N

	return true;
}
