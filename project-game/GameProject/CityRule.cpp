/*==============================================================================

    CityRule.cpp - ���̎��������[���[��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/28
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "CityRule.h"
#include "HalfEdgeModel.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Attribute.h"
#include <functional>

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�ÓI�����o�ϐ��錾
------------------------------------------------------------------------------*/
std::mt19937 CityRule::m_rand;

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
CityRule::CityRule()
{
	// �ʂ𕪊����郋�[���ݒ�
	m_RulesDivideFace.push_back( DivideFaceFunc1);
	m_RulesDivideFace.push_back( DivideFaceFunc2);

	std::random_device ran;
	m_rand.seed( ran());
}

/*------------------------------------------------------------------------------
	�ʂ𕪊����郋�[��	�߂�l�@bool�@�����@Face*
------------------------------------------------------------------------------*/
bool CityRule::DivideFace(Face* face)
{
	if (m_RulesDivideFace.size() == 0)
	{
		return false;
	}

	//�m���I�ɏ����𗣎U
	float rateNotDivide = 0.12f;
	
	//0.0f�`1.0f�̃����_���l
	float random = m_rand() % 10000 * 0.0001f;

	if (random <= rateNotDivide)
	{
		return false;
	}
	random -= rateNotDivide;

	return m_RulesDivideFace[0](face);
}

/*------------------------------------------------------------------------------
	�ʂ�^����ɂ���i�����`�A�����`�̂݊m���ɗL���j
------------------------------------------------------------------------------*/
bool CityRule::DivideFaceFunc1(Face* face)
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

/*------------------------------------------------------------------------------
	�ʂ��������炵���䗦�ŕ�������i�����`�A�����`�̂݊m���ɗL���j
------------------------------------------------------------------------------*/
bool CityRule::DivideFaceFunc2(Face* face)
{
	//�����ʂ����߂�
	const float rateCenter = 0.5f;
	const float rateRange = 0.5f;
	const float rate = rateCenter + rateRange * ( m_rand() % 1000 * 0.001f - 0.5f);

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

		//�����n�[�t�G�b�W�𕪊�����
		if( targetHalfEdge->GetEnd() == targetHalfEdge->GetEdge()->GetEnd())
		{
			if (!targetHalfEdge->GetEdge()->Divide( rate, &start))
			{
				return false;
			}
		}
		else
		{
			if (!targetHalfEdge->GetEdge()->Divide( 1 - rate, &start))
			{
				return false;
			}
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
		if( parallelHalfEdge->GetEnd() == parallelHalfEdge->GetEdge()->GetEnd())
		{
			if (!parallelHalfEdge->GetEdge()->Divide( 1 - rate, &end))
			{
				return false;
			}
		}
		else
		{
			if (!parallelHalfEdge->GetEdge()->Divide( rate, &end))
			{
				return false;
			}
		}
	}

	//�ʂ𕪊�����
	if (!face->Divide(start, end))
	{
		return false;
	}

	return true;
}