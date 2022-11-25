/*==============================================================================

    CityRule.h - 町の自動生成ールール
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/28
==============================================================================*/
#ifndef _CITY_RULE_H_
#define _CITY_RULE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Rule.h"
#include <vector>
#include <functional>
#include <random>

/*------------------------------------------------------------------------------
	前方宣言
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
	クラス定義
------------------------------------------------------------------------------*/
class CityRule : public HalfEdgeDataStructure::Rule
{
private:
	std::vector< std::function<bool(HalfEdgeDataStructure::Face*)>> m_RulesDivideFace;

	//面を分割するルール	戻り値　bool　引数　Face*
	static bool DivideFaceFunc1( HalfEdgeDataStructure::Face* face);
	static bool DivideFaceFunc2( HalfEdgeDataStructure::Face* face);

	static std::mt19937  m_rand;

public:
	CityRule();

	virtual bool DivideFace( HalfEdgeDataStructure::Face* face);
};

#endif

