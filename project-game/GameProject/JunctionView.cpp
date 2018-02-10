/*==============================================================================

    JunctionView.h - ���̎��������[�����_�r���[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "JunctionView.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "Polygon3DRenderer.h"
#include "CityAttribute.h"

#include "Edge.h"
#include "Vertex.h"
#include "HalfEdge.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* JunctionView::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<JunctionView>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
JunctionView::JunctionView( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Attribute = NULL;
	m_IsUpdatedAttribute = false;

	//�����_���[�̐ݒ�
	m_Renderer = m_pGameObject->AddComponent<Polygon3DRenderer>();
	m_Renderer->LoadTexture( "data/TEXTURE/junction_+.png");
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void JunctionView::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void JunctionView::Update( void)
{
	if (!m_IsUpdatedAttribute)
	{
		return;
	}

	m_IsUpdatedAttribute = false;

	//View���̍X�V
	UpdateRenderer();
}

/*------------------------------------------------------------------------------
	�������̐ݒ�
------------------------------------------------------------------------------*/
void JunctionView::SetAttribute( JunctionAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;

	//View���̍X�V
	UpdateRenderer();
}

/*------------------------------------------------------------------------------
	�`����̍X�V
------------------------------------------------------------------------------*/
void JunctionView::UpdateRenderer(void)
{
	m_pTransform->SetLocalPosition( m_Attribute->GetPosition());
	m_pTransform->SetLocalPositionY( 0.02f);
	
	auto edges = m_Attribute->GetVertex()->GetEdges();
	if (edges.size() == 0)
	{
		return;
	}

	auto edgeBegin = edges.begin();
	auto roadAttribute = (RoadAttribute*)((*edgeBegin)->GetAttribute());
	float width = roadAttribute->GetWidth();
	m_pTransform->SetLocalScale( width, 1.0f, width);

	//�k���A�s���A�\���̐ݒ�
	switch (edges.size())
	{
	case 2:		//�k��
	{
		m_Renderer->LoadTexture( "data/TEXTURE/junction_L.png");

		//�x�N�g���̎擾
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

		//�O�ςɉ����ĉ�]
		if (Vector3::Cross(vec1, vec2).y < 0.0f)
		{
			m_pTransform->SetWorldRotationLookDirection( vec1);
		}
		else
		{
			m_pTransform->SetWorldRotationLookDirection( vec2);
		}

		break;
	}

	case 3:		//�s��
	{
		m_Renderer->LoadTexture( "data/TEXTURE/junction_T.png");
		
		//�����������Ă��铹�H���Ȃ����H���擾
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

		//�s���ɉ�]
		auto vec = targetEdge->GetVector();
		if (targetEdge->GetStart() == m_Attribute->GetVertex())
		{
			vec *= -1.0f;
		}
		m_pTransform->SetWorldRotationLookDirection( vec);

		break;
	}

	case 4:		//�\��
	{
		m_Renderer->LoadTexture( "data/TEXTURE/junction_+.png");


		break;
	}

	default:
		break;
	}
}
