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

