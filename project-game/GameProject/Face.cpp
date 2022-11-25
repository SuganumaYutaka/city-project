/*==============================================================================

    Face.cpp - ハーフエッジ構造ー面
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
#include "Vector3.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Face::Face( HalfEdgeDataStructure::Model* model, HalfEdge* he)
	: m_Model(model), m_HalfEdge(he), m_Attribute( NULL)
{ 
	model->RegisterFace(this);

}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Face::~Face()
{
	
}

/*------------------------------------------------------------------------------
	削除
------------------------------------------------------------------------------*/
void Face::Delete(void)
{
	m_Model->UnregisterFace( this);
	if( m_Attribute)
	{
		m_Attribute->OnDeleteFace();
	}
}

/*------------------------------------------------------------------------------
	分割する
------------------------------------------------------------------------------*/
bool Face::Divide(Vertex* start, Vertex* end, Edge** ppOut)
{
	//引数の頂点が分割に適しているかチェック
	//この面の上にあるハーフエッジがあるか
	auto startHalfEdge = start->SearchHalfEdgeOnFace( this);
	if (!startHalfEdge)
	{
		return false;
	}
	auto endHalfEdge = end->SearchHalfEdgeOnFace( this);
	if (!endHalfEdge)
	{
		return false;
	}

	//新しい面を生成できるか（一直線上にあると面ができない）
	if (startHalfEdge->GetNext() == endHalfEdge || endHalfEdge->GetNext() == startHalfEdge)
	{
		return false;
	}
	Vector3 vec = end->GetPosition() - start->GetPosition();
	if (Vector3::Dot(endHalfEdge->GetVector().Normalize(), vec.Normalize()) == 1)
	{
		return false;
	}

	//辺の生成
	auto edge = m_Model->CreateEdge( start, end);
	auto right = new HalfEdge( m_Model, end);
	auto left = new HalfEdge( m_Model, start);
	edge->SetRight( right);
	edge->SetLeft( left);
	right->SetEdge( edge);
	left->SetEdge( edge);
	right->SetPair( left);
	left->SetPair( right);

	//点に辺を設定
	start->RegisterEdge( edge);
	end->RegisterEdge( edge);

	//辺を出力
	if (ppOut)
	{
		*ppOut = edge;
	}

	//関係性を設定
	right->SetNext( endHalfEdge->GetNext());
	left->SetNext( startHalfEdge->GetNext());
	startHalfEdge->SetNext( right);
	endHalfEdge->SetNext( left);
	right->SetFace( this);

	//面の生成
	auto face = m_Model->CreateFace( left);

	//面にハーフエッジを設定
	this->SetHalfEdge( right);
	
	//ハーフエッジに面を設定
	HalfEdge* he = left->GetNext();
	for (;;)
	{
		he->SetFace( face);
		if (he == left)
		{
			break;
		}
		he = he->GetNext();
	}
	he = right->GetNext();
	for (;;)
	{
		he->SetFace( this);
		if (he == right)
		{
			break;
		}
		he = he->GetNext();
	}

	//面の更新
	if( this->m_Attribute)
	{
		this->m_Attribute->Update();
	}
	if( face->m_Attribute)
	{
		face->m_Attribute->Update();
	}

	return true;
}

/*------------------------------------------------------------------------------
	分割するために必要なハーフエッジ（始点を終点とするもの）をさがす
------------------------------------------------------------------------------*/
HalfEdge* Face::SearchStartHalfEdgeForDivide(Vertex* start, Vertex* end)
{
	//始点を終点とするすべてのハーフエッジをチェック
	auto halfedges = start->GetHalfEdges();
	for (auto halfedge : halfedges)
	{
		if (halfedge->GetFace() == this)
		{
			return halfedge;
		}
	}

	//見つからない
	return NULL;
}

/*------------------------------------------------------------------------------
	一番長い辺をさがす
------------------------------------------------------------------------------*/
HalfEdge* Face::SearchLongestHalfEdge(void)
{
	float longestValue = 0.0f;
	HalfEdge* longestHalfEdge = NULL;
	auto he = m_HalfEdge->GetNext();
	for (;;)
	{
		float length = he->GetVector().Length();
		if ( length > longestValue)
		{
			longestValue = length;
			longestHalfEdge = he;
		}
		if (he == m_HalfEdge)
		{
			break;
		}
		he = he->GetNext();
	}

	return longestHalfEdge;
}

/*------------------------------------------------------------------------------
	一直線になるハーフエッジをさがす
------------------------------------------------------------------------------*/
HalfEdge* Face::SeachStraightLine( HalfEdge* startHalfEdge)
{
	auto he = startHalfEdge->GetNext();
	for (;;)
	{
		if (he->IsStraightLine(he->GetNext()))
		{
			return he;
		}
		if (he == startHalfEdge)
		{
			break;
		}
		he = he->GetNext();
	}
	return NULL;
}

/*------------------------------------------------------------------------------
	移動による更新
------------------------------------------------------------------------------*/
void Face::UpdateByMove(void)
{
	UpdateAttribute();
}

/*------------------------------------------------------------------------------
	属性情報の更新
------------------------------------------------------------------------------*/
void Face::UpdateAttribute(void)
{
	if( m_Attribute)
	{
		m_Attribute->Update();
	}
}