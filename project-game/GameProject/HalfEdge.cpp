/*==============================================================================

    HalfEdgeModel.cpp - ハーフエッジ構造ーモデル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	手前のハーフエッジを取得
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
	ベクトルの取得
------------------------------------------------------------------------------*/
Vector3 HalfEdge::GetVector(void)
{
	return m_End->GetPosition() - GetPrev()->GetEnd()->GetPosition();
}

/*------------------------------------------------------------------------------
	平行なハーフエッジをさがす
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

		//内積が -1 のときハーフエッジが平行
		if (Vector3::Dot(this->GetVector().Normalize(), parallel->GetVector().Normalize()) == -1)
		{
			return parallel;
		}
		parallel = parallel->GetNext();
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	次のハーフエッジと一直線か
------------------------------------------------------------------------------*/
bool HalfEdge::IsStraightLine(HalfEdge* next)
{
	if (Vector3::Dot(this->GetVector().Normalize(), next->GetVector().Normalize()) == 1.0f)
	{
		return true;
	}

	return false;
}
