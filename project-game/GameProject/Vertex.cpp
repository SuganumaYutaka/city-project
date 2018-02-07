/*==============================================================================

    Vertex.h - �n�[�t�G�b�W�\���[���_
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Vertex::Vertex( HalfEdgeDataStructure::Model* model, const Vector3& position)
	: m_Model( model), m_Position(position), m_Attribute( NULL)
{ 
	model->RegisterVertex(this);
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Vertex::~Vertex()
{
	
}

/*------------------------------------------------------------------------------
	�폜
------------------------------------------------------------------------------*/
void Vertex::Delete(void)
{
	m_Model->UnregisterVertex( this);
	if( m_Attribute)
	{
		m_Attribute->OnDeleteVertex();
	}
}

/*------------------------------------------------------------------------------
	�ӂ�o�^
------------------------------------------------------------------------------*/
bool Vertex::RegisterEdge(Edge* edge)
{
	for (auto m_edge : m_Edges)
	{
		if (m_edge == edge)
		{
			return false;
		}
	}

	m_Edges.push_back( edge);

	if( m_Attribute)
	{
		m_Attribute->Update();
	}

	return true;
}

/*------------------------------------------------------------------------------
	�ӂ��폜
------------------------------------------------------------------------------*/
bool Vertex::UnregisterEdge(Edge* edge)
{
	for (auto ite = m_Edges.begin(); ite != m_Edges.end(); ++ite)
	{
		if (*ite == edge)
		{
			m_Edges.erase( ite);
			if( m_Attribute)
			{
				m_Attribute->Update();
			}
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
	���̒��_���I�_�Ƃ���n�[�t�G�b�W���擾
------------------------------------------------------------------------------*/
std::list<HalfEdge*> Vertex::GetHalfEdges(void)
{
	std::list<HalfEdge*> halfedges;
	for (auto edge : m_Edges)
	{
		if (edge->GetLeft() != NULL && edge->GetLeft()->GetEnd() == this)
		{
			halfedges.push_back( edge->GetLeft());
		}
		else if (edge->GetRight() != NULL && edge->GetRight()->GetEnd() == this)
		{
			halfedges.push_back( edge->GetRight());
		}
	}

	return halfedges;
}

/*------------------------------------------------------------------------------
	���̒��_���I�_�Ƃ���n�[�t�G�b�W�ŁA�w�肵���ʂ̏�ɂ�����̂�T��
------------------------------------------------------------------------------*/
HalfEdge* Vertex::SearchHalfEdgeOnFace(Face* face)
{
	for (auto edge : m_Edges)
	{
		if (edge->GetLeft() != NULL && edge->GetLeft()->GetEnd() == this)
		{
			if (edge->GetLeft()->GetFace() == face)
			{
				return edge->GetLeft();
			}
		}
		if (edge->GetRight() != NULL && edge->GetRight()->GetEnd() == this)
		{
			if (edge->GetRight()->GetFace() == face)
			{
				return edge->GetRight();
			}
		}
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	�ړ��ɂ��X�V
------------------------------------------------------------------------------*/
void Vertex::UpdateByMove(void)
{
	UpdateAttribute();

	//�אڂ���ӂɒm�点��
	for (auto edge : m_Edges)
	{
		edge->UpdateByMove();
	}
}

/*------------------------------------------------------------------------------
	�������̍X�V
------------------------------------------------------------------------------*/
void Vertex::UpdateAttribute(void)
{
	if( m_Attribute)
	{
		m_Attribute->Update();
	}
}