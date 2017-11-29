/*==============================================================================

    CityRule.h - ���̎��������[���[��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/28
==============================================================================*/
#ifndef _CITY_RULE_H_
#define _CITY_RULE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Rule.h"
#include <vector>
#include <functional>
#include <random>

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Vertex;
	class Edge;
	class Face;
	class HalfEdge;
	class Model;
}


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class CityRule : public Rule
	{
	private:
		std::vector< std::function<bool(Face*)>> m_RulesDivideFace;

		//�ʂ𕪊����郋�[��	�߂�l�@bool�@�����@Face*
		static bool DivideFaceFunc1( Face* face);
		static bool DivideFaceFunc2( Face* face);

		static std::mt19937  m_rand;

	public:
		CityRule();

		virtual bool DivideFace( Face* face);
	};
}

#endif

