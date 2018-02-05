/*==============================================================================

    HalfEdgeModel.cpp - ハーフエッジ構造ーモデル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "HalfEdgeModel.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Rule.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Model::~Model()
{
	for (auto face : m_Faces)
	{
		delete face;
	}
	m_Faces.clear();
	m_Faces.shrink_to_fit();

	for (auto edge : m_Edges)
	{
		delete edge;
	}
	m_Edges.clear();
	m_Edges.shrink_to_fit();
	
	for (auto vertex : m_Vertices)
	{
		delete vertex;
	}
	m_Vertices.clear();
	m_Vertices.shrink_to_fit();
}

/*------------------------------------------------------------------------------
	点を生成
------------------------------------------------------------------------------*/
Vertex* Model::CreateVertex(const Vector3& position)
{
	return new Vertex( this, position);
}

/*------------------------------------------------------------------------------
	辺を生成
------------------------------------------------------------------------------*/
Edge* Model::CreateEdge( Vertex* start, Vertex* end)
{
	return new Edge( this, start, end);
}

/*------------------------------------------------------------------------------
	面を生成
------------------------------------------------------------------------------*/
Face* Model::CreateFace( HalfEdge* he)
{
	return new Face( this, he);
}

/*------------------------------------------------------------------------------
	はじめての点を生成
------------------------------------------------------------------------------*/
bool Model::CreateFirstVertex(const Vector3& position)
{
	if (m_Vertices.size() > 0)
	{
		return false;
	}

	//点を生成
	CreateVertex( position);
	
	return true;
}

/*------------------------------------------------------------------------------
	はじめての辺を生成
------------------------------------------------------------------------------*/
bool Model::CreateFirstEdge(const Vector3& startPosition, const Vector3& endPosition)
{
	if (m_Vertices.size() > 0)
	{
		return false;
	}

	//点を生成
	auto start = CreateVertex( startPosition);
	auto end = CreateVertex( endPosition);

	//辺を生成
	auto edge = CreateEdge( start, end);
	
	//点に辺を設定
	start->RegisterEdge( edge);
	end->RegisterEdge( edge);

	//属性情報の更新
	start->GetAttribute()->Update();
	end->GetAttribute()->Update();
	edge->GetAttribute()->Update();

	return true;
}

/*------------------------------------------------------------------------------
	はじめての面を生成
------------------------------------------------------------------------------*/
bool Model::CreateFirstFace(
	const Vector3& topLeftPosition, const Vector3& topRightPosition,
	const Vector3& bottomLeftPosition, const Vector3& bottomRightPosition)
{
	if (m_Vertices.size() > 0)
	{
		return false;
	}

	//点を生成
	auto topLeft = CreateVertex( topLeftPosition);
	auto topRight = CreateVertex( topRightPosition);
	auto bottomLeft = CreateVertex( bottomLeftPosition);
	auto bottomRight = CreateVertex( bottomRightPosition);
	

	//辺を生成
	auto top = CreateEdge( topLeft, topRight);
	auto bottom = CreateEdge( bottomRight, bottomLeft);
	auto left = CreateEdge( bottomLeft, topLeft);
	auto right = CreateEdge( topRight, bottomRight);

	//点に辺を設定
	topLeft->RegisterEdge( top);
	topLeft->RegisterEdge( left);
	topRight->RegisterEdge( top);
	topRight->RegisterEdge( right);
	bottomLeft->RegisterEdge( bottom);
	bottomLeft->RegisterEdge( left);
	bottomRight->RegisterEdge( bottom);
	bottomRight->RegisterEdge( right);

	//ハーフエッジを生成
	auto heTop = new HalfEdge( this, topRight);
	auto heRight = new HalfEdge( this, bottomRight);
	auto heBottom = new HalfEdge( this, bottomLeft);
	auto heLeft = new HalfEdge( this, topLeft);

	//辺にハーフエッジを設定
	top->SetRight( heTop);
	right->SetRight( heRight);
	bottom->SetRight( heBottom);
	left->SetRight( heLeft);

	//ハーフエッジに辺とnextを設定
	heTop->SetEdge( top);
	heRight->SetEdge( right);
	heBottom->SetEdge( bottom);
	heLeft->SetEdge( left);
	heTop->SetNext( heRight);
	heRight->SetNext( heBottom);
	heBottom->SetNext( heLeft);
	heLeft->SetNext( heTop);

	//面を生成
	auto face = CreateFace( heTop);

	//ハーフエッジに面を設定
	heTop->SetFace( face);
	heRight->SetFace( face);
	heBottom->SetFace( face);
	heLeft->SetFace( face);

	//属性情報の更新
	topLeft->GetAttribute()->Update();
	topRight->GetAttribute()->Update();
	bottomLeft->GetAttribute()->Update();
	bottomRight->GetAttribute()->Update();
	top->GetAttribute()->Update();
	right->GetAttribute()->Update();
	bottom->GetAttribute()->Update();
	left->GetAttribute()->Update();
	face->GetAttribute()->Update();
	
	return true;
}

/*------------------------------------------------------------------------------
	頂点を取得
------------------------------------------------------------------------------*/
Vertex* Model::GetVertex(int id)
{
	if (m_Vertices.size() <= id)
	{
		return NULL;
	}

	return m_Vertices[ id];
}

/*------------------------------------------------------------------------------
	頂点のIDを取得
------------------------------------------------------------------------------*/
int Model::GetVertexID(Vertex* vertex)
{
	int size = m_Vertices.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Vertices[i] == vertex)
		{
			return i;
		}
	}

	//発見できない
	return -1;
}

/*------------------------------------------------------------------------------
	登録を解除
------------------------------------------------------------------------------*/
bool Model::UnregisterVertex( Vertex* vertex)
{
	int size = m_Vertices.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Vertices[i] == vertex)
		{
			m_Vertices[i] = NULL;
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
	辺を取得
------------------------------------------------------------------------------*/
Edge* Model::GetEdge(int id)
{
	if (m_Vertices.size() <= id)
	{
		return NULL;
	}

	return m_Edges[ id];
}

/*------------------------------------------------------------------------------
	辺のIDを取得
------------------------------------------------------------------------------*/
int Model::GetEdgeID( Edge* edge)
{
	int size = m_Edges.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Edges[i] == edge)
		{
			return i;
		}
	}

	//発見できない
	return -1;
}

/*------------------------------------------------------------------------------
	登録を解除
------------------------------------------------------------------------------*/
bool Model::UnregisterEdge( Edge* edge)
{
	int size = m_Edges.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Edges[i] == edge)
		{
			m_Edges[i] = NULL;
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
	面を取得
------------------------------------------------------------------------------*/
Face* Model::GetFace(int id)
{
	if (m_Vertices.size() <= id)
	{
		return NULL;
	}

	return m_Faces[ id];
}

/*------------------------------------------------------------------------------
	面のIDを取得
------------------------------------------------------------------------------*/
int Model::GetFaceID( Face* face)
{
	int size = m_Faces.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Faces[i] == face)
		{
			return i;
		}
	}

	//発見できない
	return -1;
}

/*------------------------------------------------------------------------------
	登録を解除
------------------------------------------------------------------------------*/
bool Model::UnregisterFace( Face* face)
{
	int size = m_Faces.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Faces[i] == face)
		{
			m_Faces[i] = NULL;
			return true;
		}
	}

	return false;
}
