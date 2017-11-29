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

	for (auto edge : m_Edges)
	{
		delete edge;
	}
	m_Edges.clear();
	
	for (auto vertex : m_Vertices)
	{
		delete vertex;
	}
	m_Vertices.clear();

	if (m_Rule)
	{
		delete m_Rule;
	}

	if (m_AttributeFactory)
	{
		delete m_AttributeFactory;
	}
}

/*------------------------------------------------------------------------------
	点を生成
------------------------------------------------------------------------------*/
Vertex* Model::CreateVertex(const Vector3& position)
{
	return new Vertex( this, position, m_AttributeFactory->CreateVertexAttribute());
}

/*------------------------------------------------------------------------------
	辺を生成
------------------------------------------------------------------------------*/
Edge* Model::CreateEdge( Vertex* start, Vertex* end)
{
	return new Edge( this, start, end, m_AttributeFactory->CreateEdgeAttribute());
}

/*------------------------------------------------------------------------------
	面を生成
------------------------------------------------------------------------------*/
Face* Model::CreateFace( HalfEdge* he)
{
	return new Face( this, he, m_AttributeFactory->CreateFaceAttribute());
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
	
	return true;
}

/*------------------------------------------------------------------------------
	すべての面を分割
------------------------------------------------------------------------------*/
bool Model::DivideAllFaces(void)
{
	if (m_Faces.size() == 0)
	{
		return false;
	}

	auto faces = m_Faces;

	for (auto face : faces)
	{
		//正方形・長方形と仮定して分割ルールを設定

		if (!m_Rule->DivideFace( face))
		{
			continue;
		}
	}

	return true;
}
