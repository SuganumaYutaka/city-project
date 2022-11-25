/*==============================================================================

    FaceDivideFunc.cpp - 町の自動生成ー面を分割する処理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "FaceDivideFunc.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	面を分割する処理
------------------------------------------------------------------------------*/
bool FaceDivideFunc::operator() (HalfEdgeDataStructure::Face* face)
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

//別の分割方法
//#define BOARDER_RATE (2.5f)
//#define BOARDER_PARALLEL (0.7f)
///*------------------------------------------------------------------------------
//	長い辺を優先して分割
//------------------------------------------------------------------------------*/
//bool CityRule::DivideFaceFunc2(Face* face)
//{
//	HalfEdge* targetHalfEdge = NULL;
//	Vertex* start = NULL;
//	Vertex* end = NULL;
//	
//	//一番長いハーフエッジを取得する
//	targetHalfEdge = face->SearchLongestHalfEdge();
//	if (!targetHalfEdge)
//	{
//		return false;
//	}
//
//	//一番長い辺と次の辺を比較して四角形が崩れすぎていないかチェック
//	float rateToCheckTooLonger = targetHalfEdge->GetVector().Length() / targetHalfEdge->GetNext()->GetVector().Length();
//	if (rateToCheckTooLonger < BOARDER_RATE)
//	{
//		return false;
//	}
//
//	//ターゲットとある程度平行な反対側のハーフエッジを探す
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
//	//長いハーフエッジを分割する
//	if (!targetHalfEdge->GetEdge()->Divide(0.5f, &start))
//	{
//		return false;
//	}
//
//	//反対側のハーフエッジを分割
//	if (!otherSideHalfEdge->GetEdge()->Divide(0.5f, &end))
//	{
//		return false;
//	}
//
//	//面を分割する
//	if (!face->Divide(start, end))
//	{
//		return false;
//	}
//
//	return true;
//}