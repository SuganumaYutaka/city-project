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

	//分割しない
	if (random <= rateNotDivide)
	{
		return false;
	}
	random -= rateNotDivide;

	//長すぎる辺を優先的に分割
	if (m_RulesDivideFace[1](face))
	{
		return true;
	}

	//メインの分割ルール
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

#define BOARDER_RATE (2.5f)
#define BOARDER_PARALLEL (0.7f)
/*------------------------------------------------------------------------------
	長い辺を優先して分割
------------------------------------------------------------------------------*/
bool CityRule::DivideFaceFunc2(Face* face)
{
	HalfEdge* targetHalfEdge = NULL;
	Vertex* start = NULL;
	Vertex* end = NULL;
	
	//一番長いハーフエッジを取得する
	targetHalfEdge = face->SearchLongestHalfEdge();
	if (!targetHalfEdge)
	{
		return false;
	}

	//一番長い辺と次の辺を比較して四角形が崩れすぎていないかチェック
	float rateToCheckTooLonger = targetHalfEdge->GetVector().Length() / targetHalfEdge->GetNext()->GetVector().Length();
	if (rateToCheckTooLonger < BOARDER_RATE)
	{
		return false;
	}

	//ターゲットとある程度平行な反対側のハーフエッジを探す
	HalfEdge* otherSideHalfEdge = NULL;
	otherSideHalfEdge = targetHalfEdge->GetNext();
	for (;;)
	{
		float dot = Vector3::Dot( targetHalfEdge->GetVector().Normalize(), otherSideHalfEdge->GetVector().Normalize());
		if (dot < -BOARDER_PARALLEL)
		{
			break;
		}

		otherSideHalfEdge = otherSideHalfEdge->GetNext();
		if (otherSideHalfEdge == targetHalfEdge)
		{
			return false;
		}
	}

	//長いハーフエッジを分割する
	if (!targetHalfEdge->GetEdge()->Divide(0.5f, &start))
	{
		return false;
	}

	//反対側のハーフエッジを分割
	if (!otherSideHalfEdge->GetEdge()->Divide(0.5f, &end))
	{
		return false;
	}

	//面を分割する
	if (!face->Divide(start, end))
	{
		return false;
	}

	return true;
}