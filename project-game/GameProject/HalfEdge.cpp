/*==============================================================================

    HalfEdgeModel.cpp - �n�[�t�G�b�W�\���[���f��
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
	��O�̃n�[�t�G�b�W���擾
------------------------------------------------------------------------------*/
HalfEdge* HalfEdge::GetPrev(void)
{
	HalfEdge* prev = this->m_Next;
	for (;;)
	{
		if (prev->GetNext() == this)
		{
			return prev;
		}
		
		if (prev == this)
		{
			break;
		}

		prev = prev->GetNext();
	}
	return NULL;
}

/*------------------------------------------------------------------------------
	�x�N�g���̎擾
------------------------------------------------------------------------------*/
Vector3 HalfEdge::GetVector(void)
{
	return m_End->GetPosition() - GetPrev()->GetEnd()->GetPosition();
}

/*------------------------------------------------------------------------------
	���s�ȃn�[�t�G�b�W��������
------------------------------------------------------------------------------*/
HalfEdge* HalfEdge::SearchParallelHalfEdge(void)
{
	auto parallel = this->GetNext();
	for (;;)
	{
		if (parallel == this)
		{
			return NULL;
		}

		//���ς� -1 �̂Ƃ��n�[�t�G�b�W�����s
		if (Vector3::Dot(this->GetVector().Normalize(), parallel->GetVector().Normalize()) == -1)
		{
			return parallel;
		}
		parallel = parallel->GetNext();
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	���̃n�[�t�G�b�W�ƈ꒼����
------------------------------------------------------------------------------*/
bool HalfEdge::IsStraightLine(HalfEdge* next)
{
	if (Vector3::Dot(this->GetVector().Normalize(), next->GetVector().Normalize()) == 1.0f)
	{
		return true;
	}

	return false;
}
