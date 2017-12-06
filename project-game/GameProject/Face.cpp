/*==============================================================================

    Face.cpp - �n�[�t�G�b�W�\���[��
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
#include "Vector3.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Face::Face( HalfEdgeDataStructure::Model* model, HalfEdge* he, FaceAttribute* attribute)
	: m_Model(model), m_HalfEdge(he), m_Attribute( attribute)
{ 
	model->RegisterFace(this);
	m_Attribute->SetFace(this);
	m_Attribute->Init();
}

/*------------------------------------------------------------------------------
	��������
------------------------------------------------------------------------------*/
bool Face::Divide(Vertex* start, Vertex* end, Edge** ppOut)
{
	//�����̒��_�������ɓK���Ă��邩�`�F�b�N
	//���̖ʂ̏�ɂ���n�[�t�G�b�W�����邩
	auto startHalfEdge = start->SearchHalfEdgeOnFace( this);
	if (!startHalfEdge)
	{
		return false;
	}
	auto endHalfEdge = end->SearchHalfEdgeOnFace( this);
	if (!endHalfEdge)
	{
		return false;
	}

	//�V�����ʂ𐶐��ł��邩�i�꒼����ɂ���Ɩʂ��ł��Ȃ��j
	if (startHalfEdge->GetNext() == endHalfEdge || endHalfEdge->GetNext() == startHalfEdge)
	{
		return false;
	}
	Vector3 vec = end->GetPosition() - start->GetPosition();
	if (Vector3::Dot(endHalfEdge->GetVector().Normalize(), vec.Normalize()) == 1)
	{
		return false;
	}

	//�ӂ̐���
	auto edge = m_Model->CreateEdge( start, end);
	auto right = new HalfEdge( m_Model, end);
	auto left = new HalfEdge( m_Model, start);
	edge->SetRight( right);
	edge->SetLeft( left);
	right->SetEdge( edge);
	left->SetEdge( edge);
	right->SetPair( left);
	left->SetPair( right);

	//�_�ɕӂ�ݒ�
	start->RegisterEdge( edge);
	end->RegisterEdge( edge);

	//�ӂ��o��
	if (ppOut)
	{
		*ppOut = edge;
	}

	//�֌W����ݒ�
	right->SetNext( endHalfEdge->GetNext());
	left->SetNext( startHalfEdge->GetNext());
	startHalfEdge->SetNext( right);
	endHalfEdge->SetNext( left);
	right->SetFace( this);

	//�ʂ̐���
	auto face = m_Model->CreateFace( left);

	//�ʂɃn�[�t�G�b�W��ݒ�
	this->SetHalfEdge( right);
	
	//�n�[�t�G�b�W�ɖʂ�ݒ�
	HalfEdge* he = left->GetNext();
	for (;;)
	{
		he->SetFace( face);
		if (he == left)
		{
			break;
		}
		he = he->GetNext();
	}
	he = right->GetNext();
	for (;;)
	{
		he->SetFace( this);
		if (he == right)
		{
			break;
		}
		he = he->GetNext();
	}

	//�ʂ̍X�V
	this->m_Attribute->Update();
	//face->m_Attribute->Update();

	return true;
}

/*------------------------------------------------------------------------------
	�������邽�߂ɕK�v�ȃn�[�t�G�b�W�i�n�_���I�_�Ƃ�����́j��������
------------------------------------------------------------------------------*/
HalfEdge* Face::SearchStartHalfEdgeForDivide(Vertex* start, Vertex* end)
{
	//�n�_���I�_�Ƃ��邷�ׂẴn�[�t�G�b�W���`�F�b�N
	auto halfedges = start->GetHalfEdges();
	for (auto halfedge : halfedges)
	{
		if (halfedge->GetFace() == this)
		{
			return halfedge;
		}
	}

	//������Ȃ�
	return NULL;
}

/*------------------------------------------------------------------------------
	��Ԓ����ӂ�������
------------------------------------------------------------------------------*/
HalfEdge* Face::SearchLongestHalfEdge(void)
{
	float longestValue = 0.0f;
	HalfEdge* longestHalfEdge = NULL;
	auto he = m_HalfEdge->GetNext();
	for (;;)
	{
		float length = he->GetVector().Length();
		if ( length > longestValue)
		{
			longestValue = length;
			longestHalfEdge = he;
		}
		if (he == m_HalfEdge)
		{
			break;
		}
		he = he->GetNext();
	}

	return longestHalfEdge;
}

/*------------------------------------------------------------------------------
	�꒼���ɂȂ�n�[�t�G�b�W��������
------------------------------------------------------------------------------*/
HalfEdge* Face::SeachStraightLine( HalfEdge* startHalfEdge)
{
	auto he = startHalfEdge->GetNext();
	for (;;)
	{
		if (he->IsStraightLine(he->GetNext()))
		{
			return he;
		}
		if (he == startHalfEdge)
		{
			break;
		}
		he = he->GetNext();
	}
	return NULL;
}

/*------------------------------------------------------------------------------
	�ړ��ɂ��X�V
------------------------------------------------------------------------------*/
void Face::UpdateByMove(void)
{
	m_Attribute->Update();
}
