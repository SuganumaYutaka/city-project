/*==============================================================================

    FaceDivideFunc.cpp - ���̎��������[�ʂ𕪊����鏈��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "FaceDivideFunc.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�ʂ𕪊����鏈��
------------------------------------------------------------------------------*/
bool FaceDivideFunc::operator() (HalfEdgeDataStructure::Face* face)
{
	//�Q�ň꒼���ɂȂ�n�[�t�G�b�W������Ƃ����̒��_���n�_�ɂ���
	HalfEdge* targetHalfEdge = NULL;
	Vertex* start = NULL;
	Vertex* end = NULL;
	targetHalfEdge = face->SeachStraightLine( face->GetHalfEdge());
	if (targetHalfEdge)
	{
		start = targetHalfEdge->GetEnd();
	}

	else
	{
		//��Ԓ����n�[�t�G�b�W���擾����
		targetHalfEdge = face->SearchLongestHalfEdge();
		if (!targetHalfEdge)
		{
			return false;
		}

		//�^�[�Q�b�g�ƕ��s�Ȕ��Α��̃n�[�t�G�b�W�����邩
		if (!targetHalfEdge->SearchParallelHalfEdge())
		{
			return false;
		}

		//�����n�[�t�G�b�W�𕪊�����
		if (!targetHalfEdge->GetEdge()->Divide(0.5f, &start))
		{
			return false;
		}
	}

	//�^�[�Q�b�g�ƕ��s�Ȕ��Α��̃n�[�t�G�b�W���擾
	HalfEdge* parallelHalfEdge = targetHalfEdge->SearchParallelHalfEdge();
	if (!parallelHalfEdge)
	{
		return false;
	}

	//���s�n�[�t�G�b�W�ƈ꒼���ɂȂ�n�[�t�G�b�W������Ƃ����̒��_���I�_�ɂ���
	if ( parallelHalfEdge->IsStraightLine(parallelHalfEdge->GetNext()))
	{
		end = parallelHalfEdge->GetEnd();
	}

	else
	{
		//���s�n�[�t�G�b�W�𕪊�
		if (!parallelHalfEdge->GetEdge()->Divide(0.5f, &end))
		{
			return false;
		}
	}

	//�ʂ𕪊�����
	if (!face->Divide(start, end))
	{
		return false;
	}

	return true;
}

//�ʂ̕������@
//#define BOARDER_RATE (2.5f)
//#define BOARDER_PARALLEL (0.7f)
///*------------------------------------------------------------------------------
//	�����ӂ�D�悵�ĕ���
//------------------------------------------------------------------------------*/
//bool CityRule::DivideFaceFunc2(Face* face)
//{
//	HalfEdge* targetHalfEdge = NULL;
//	Vertex* start = NULL;
//	Vertex* end = NULL;
//	
//	//��Ԓ����n�[�t�G�b�W���擾����
//	targetHalfEdge = face->SearchLongestHalfEdge();
//	if (!targetHalfEdge)
//	{
//		return false;
//	}
//
//	//��Ԓ����ӂƎ��̕ӂ��r���Ďl�p�`�����ꂷ���Ă��Ȃ����`�F�b�N
//	float rateToCheckTooLonger = targetHalfEdge->GetVector().Length() / targetHalfEdge->GetNext()->GetVector().Length();
//	if (rateToCheckTooLonger < BOARDER_RATE)
//	{
//		return false;
//	}
//
//	//�^�[�Q�b�g�Ƃ�����x���s�Ȕ��Α��̃n�[�t�G�b�W��T��
//	HalfEdge* otherSideHalfEdge = NULL;
//	otherSideHalfEdge = targetHalfEdge->GetNext();
//	for (;;)
//	{
//		float dot = Vector3::Dot( targetHalfEdge->GetVector().Normalize(), otherSideHalfEdge->GetVector().Normalize());
//		if (dot < -BOARDER_PARALLEL)
//		{
//			break;
//		}
//
//		otherSideHalfEdge = otherSideHalfEdge->GetNext();
//		if (otherSideHalfEdge == targetHalfEdge)
//		{
//			return false;
//		}
//	}
//
//	//�����n�[�t�G�b�W�𕪊�����
//	if (!targetHalfEdge->GetEdge()->Divide(0.5f, &start))
//	{
//		return false;
//	}
//
//	//���Α��̃n�[�t�G�b�W�𕪊�
//	if (!otherSideHalfEdge->GetEdge()->Divide(0.5f, &end))
//	{
//		return false;
//	}
//
//	//�ʂ𕪊�����
//	if (!face->Divide(start, end))
//	{
//		return false;
//	}
//
//	return true;
//}