/*==============================================================================

    Vertex.h - ハーフエッジ構造ー頂点
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
	辺を登録
------------------------------------------------------------------------------*/
bool Vertex::RegisterEdge(Edge* edge)
{
	for (auto m_edge : m_Edges)
	{
		if (m_edge == edge)
		{
			return false;
		}
	}

	m_Edges.push_back( edge);
	return true;
}

/*------------------------------------------------------------------------------
	辺を削除
------------------------------------------------------------------------------*/
bool Vertex::UnregisterEdge(Edge* edge)
{
	for (auto ite = m_Edges.begin(); ite != m_Edges.end(); ++ite)
	{
		if (*ite == edge)
		{
			m_Edges.erase( ite);
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
	この頂点を終点とするハーフエッジを取得
------------------------------------------------------------------------------*/
std::list<HalfEdge*> Vertex::GetHalfEdges(void)
{
	std::list<HalfEdge*> halfedges;
	for (auto edge : m_Edges)
	{
		if (edge->GetLeft() != NULL && edge->GetLeft()->GetEnd() == this)
		{
			halfedges.push_back( edge->GetLeft());
		}
		else if (edge->GetRight() != NULL && edge->GetRight()->GetEnd() == this)
		{
			halfedges.push_back( edge->GetRight());
		}
	}

	return halfedges;
}

/*------------------------------------------------------------------------------
	この頂点を終点とするハーフエッジで、指定した面の上にあるものを探す
------------------------------------------------------------------------------*/
HalfEdge* Vertex::SearchHalfEdgeOnFace(Face* face)
{
	for (auto edge : m_Edges)
	{
		if (edge->GetLeft() != NULL && edge->GetLeft()->GetEnd() == this)
		{
			if (edge->GetLeft()->GetFace() == face)
			{
				return edge->GetLeft();
			}
		}
		if (edge->GetRight() != NULL && edge->GetRight()->GetEnd() == this)
		{
			if (edge->GetRight()->GetFace() == face)
			{
				return edge->GetRight();
			}
		}
	}

	return NULL;
}

