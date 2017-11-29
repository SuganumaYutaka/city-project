/*==============================================================================

    CityRule.cpp - 町の自動生成ールール
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/28
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
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
	静的メンバ変数宣言
------------------------------------------------------------------------------*/
std::mt19937 CityRule::m_rand;

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
CityRule::CityRule()
{
	// 面を分割するルール設定
	m_RulesDivideFace.push_back( DivideFaceFunc1);
	m_RulesDivideFace.push_back( DivideFaceFunc2);

	std::random_device ran;
	m_rand.seed( ran());
}

/*------------------------------------------------------------------------------
	面を分割するルール	戻り値　bool　引数　Face*
------------------------------------------------------------------------------*/
bool CityRule::DivideFace(Face* face)
{
	if (m_RulesDivideFace.size() == 0)
	{
		return false;
	}

	//確率的に処理を離散
	float rateNotDivide = 0.12f;
	
	//0.0f～1.0fのランダム値
	float random = m_rand() % 10000 * 0.0001f;

	if (random <= rateNotDivide)
	{
		return false;
	}
	random -= rateNotDivide;

	return m_RulesDivideFace[0](face);
}

/*------------------------------------------------------------------------------
	面を真っ二つにする（正方形、長方形のみ確実に有効）
------------------------------------------------------------------------------*/
bool CityRule::DivideFaceFunc1(Face* face)
{
	//２つで一直線になるハーフエッジがあるときその中点を始点にする
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
		//一番長いハーフエッジを取得する
		targetHalfEdge = face->SearchLongestHalfEdge();
		if (!targetHalfEdge)
		{
			return false;
		}

		//ターゲットと平行な反対側のハーフエッジがあるか
		if (!targetHalfEdge->SearchParallelHalfEdge())
		{
			return false;
		}

		//長いハーフエッジを分割する
		if (!targetHalfEdge->GetEdge()->Divide(0.5f, &start))
		{
			return false;
		}
	}

	//ターゲットと平行な反対側のハーフエッジを取得
	HalfEdge* parallelHalfEdge = targetHalfEdge->SearchParallelHalfEdge();
	if (!parallelHalfEdge)
	{
		return false;
	}

	//平行ハーフエッジと一直線になるハーフエッジがあるときその中点を終点にする
	if ( parallelHalfEdge->IsStraightLine(parallelHalfEdge->GetNext()))
	{
		end = parallelHalfEdge->GetEnd();
	}

	else
	{
		//平行ハーフエッジを分割
		if (!parallelHalfEdge->GetEdge()->Divide(0.5f, &end))
		{
			return false;
		}
	}

	//面を分割する
	if (!face->Divide(start, end))
	{
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------
	面を少しずらした比率で分割する（正方形、長方形のみ確実に有効）
------------------------------------------------------------------------------*/
bool CityRule::DivideFaceFunc2(Face* face)
{
	//分割量を決める
	const float rateCenter = 0.5f;
	const float rateRange = 0.5f;
	const float rate = rateCenter + rateRange * ( m_rand() % 1000 * 0.001f - 0.5f);

	//２つで一直線になるハーフエッジがあるときその中点を始点にする
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
		//一番長いハーフエッジを取得する
		targetHalfEdge = face->SearchLongestHalfEdge();
		if (!targetHalfEdge)
		{
			return false;
		}

		//長いハーフエッジを分割する
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

	//ターゲットと平行な反対側のハーフエッジを取得
	HalfEdge* parallelHalfEdge = targetHalfEdge->SearchParallelHalfEdge();
	if (!parallelHalfEdge)
	{
		return false;
	}

	//平行ハーフエッジと一直線になるハーフエッジがあるときその中点を終点にする
	if ( parallelHalfEdge->IsStraightLine(parallelHalfEdge->GetNext()))
	{
		end = parallelHalfEdge->GetEnd();
	}

	else
	{
		//平行ハーフエッジを分割
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

	//面を分割する
	if (!face->Divide(start, end))
	{
		return false;
	}

	return true;
}