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
class CityRule : public HalfEdgeDataStructure::Rule
{
private:
	std::vector< std::function<bool(HalfEdgeDataStructure::Face*)>> m_RulesDivideFace;

	//�ʂ𕪊����郋�[��	�߂�l�@bool�@�����@Face*
	static bool DivideFaceFunc1( HalfEdgeDataStructure::Face* face);
	static bool DivideFaceFunc2( HalfEdgeDataStructure::Face* face);

	static std::mt19937  m_rand;

public:
	CityRule();

	virtual bool DivideFace( HalfEdgeDataStructure::Face* face);
};

#endif
