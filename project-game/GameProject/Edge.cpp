/*==============================================================================

    Edge.cpp - �n�[�t�G�b�W�\���[��
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
Edge::Edge( HalfEdgeDataStructure::Model* model, Vertex* start, Vertex* end)
	: m_Model(model), m_Start(start), m_End(end), m_Left(NULL), m_Right(NULL), m_Attribute(NULL)
{
	model->RegisterEdge(this); 
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Edge::~Edge()
{ 
	//�n�[�t�G�b�W�̍폜
	if (m_Right)
	{
		delete m_Right;
	}
	if (m_Left)
	{
		delete m_Left;
	}

	m_Model->UnregisterEdge( this);
	if( m_Attribute)
	{
		m_Attribute->OnDeleteEdge();
	}
}

/*------------------------------------------------------------------------------
	�x�N�g���̎擾
------------------------------------------------------------------------------*/
Vector3 Edge::GetVector(void)
{
	return m_End->GetPosition() - m_Start->GetPosition();
}

/*------------------------------------------------------------------------------
	��������
------------------------------------------------------------------------------*/
bool Edge::Divide(float rateFromStart, Vertex** ppOut)
{
	//�����`�F�b�N
	if (rateFromStart < 0.0f || rateFromStart > 1.0f)
	{
		return false;
	}

	//���_�̐���
	Vector3 direction = m_End->GetPosition() - m_Start->GetPosition();
	Vector3 position = m_Start->GetPosition() + direction * rateFromStart;
	auto vertex = m_Model->CreateVertex( position);
	
	//���_���o��
	if ( ppOut)
	{
		*ppOut = vertex;
	}

	//�ӂ̐���
	auto edge = m_Model->CreateEdge( vertex, m_End);

	//�ӂɓ_��o�^
	this->SetEnd( vertex);

	//�E���ʂ̐ݒ�
	if (this->GetRight())
	{
		auto rightnext = new HalfEdge( m_Model, edge->GetEnd());
		edge->SetRight( rightnext);
		rightnext->SetFace( this->GetRight()->GetFace());
		rightnext->SetEdge( edge);
		rightnext->SetNext( this->GetRight()->GetNext());
		this->GetRight()->SetNext( rightnext);
		this->GetRight()->SetEnd( vertex);
	}
	//�����ʂ̐ݒ�
	if (this->GetLeft())
	{
		auto leftnext = new HalfEdge( m_Model, this->GetStart());
		edge->SetLeft( this->GetLeft());
		leftnext->SetFace( this->GetLeft()->GetFace());
		this->GetLeft()->SetEdge( edge);
		this->SetLeft( leftnext);
		leftnext->SetEdge( this);
		leftnext->SetNext( edge->GetLeft()->GetNext());
		edge->GetLeft()->SetNext( leftnext);
		edge->GetLeft()->SetEnd( vertex);
	}
	
	//�n�[�t�G�b�W�̃y�A��ݒ�
	if( this->GetRight() && this->GetLeft())
	{
		this->GetLeft()->SetPair( this->GetRight());
		this->GetRight()->SetPair( this->GetLeft());
		edge->GetLeft()->SetPair( edge->GetRight());
		edge->GetRight()->SetPair( edge->GetLeft());
	}

	//�_�ɕӂ�o�^
	vertex->RegisterEdge( this);
	vertex->RegisterEdge( edge);
	edge->GetEnd()->UnregisterEdge( this);
	edge->GetEnd()->RegisterEdge( edge);

	return true;
}

/*------------------------------------------------------------------------------
	�ړ��ɂ��X�V
------------------------------------------------------------------------------*/
void Edge::UpdateByMove(void)
{
	if( m_Attribute)
	{
		m_Attribute->Update();
	}

	//�אڂ���ʂɒm�点��
	//�E
	if (m_Right)
	{
		m_Right->GetFace()->UpdateByMove();
	}

	//��
	if (m_Left)
	{
		m_Left->GetFace()->UpdateByMove();
	}
}
