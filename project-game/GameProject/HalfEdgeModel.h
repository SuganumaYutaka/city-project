/*==============================================================================

    HalfEdgeModel.h - ハーフエッジ構造ーモデル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/
#ifndef _HALF_EDGE_MODEL_H_
#define _HALF_EDGE_MODEL_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include <list>
#include "Vector3.h"
#include <vector>
#include <functional>

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Vertex;
	class Edge;
	class Face;
	class HalfEdge;
}

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Model
	{
	private:
		std::vector<Vertex*> m_Vertices;
		std::vector<Edge*> m_Edges;
		std::vector<Face*> m_Faces;
		
	public:
		~Model();

		Vertex* CreateVertex( const Vector3& position);
		Edge* CreateEdge( Vertex* start, Vertex* end);
		Face* CreateFace( HalfEdge* he);

		bool CreateFirstVertex( const Vector3& position);
		bool CreateFirstEdge( const Vector3& startPosition, const Vector3& endPosition);
		bool CreateFirstFace( 
			const Vector3& topLeftPosition, const Vector3& topRightPosition, 
			const Vector3& bottomLeftPosition, const Vector3& bottomRightPosition);

		Vertex* GetVertex( int id);
		int GetVertexID( Vertex* vertex);
		int GetVertexCount( void){ return (int)m_Vertices.size();}
		const std::vector<Vertex*>& GetVertices( void){ return m_Vertices;}
		void RegisterVertex( Vertex* vertex){ m_Vertices.push_back( vertex);}
		bool UnregisterVertex( Vertex* vertex);

		Edge* GetEdge( int id);
		int GetEdgeID( Edge* edge);
		int GetEdgeCount( void){ return (int)m_Edges.size();}
		const std::vector<Edge*>& GetEdges( void){ return m_Edges;}
		void RegisterEdge( Edge* edge){ m_Edges.push_back( edge);}
		bool UnregisterEdge( Edge* edge);

		Face* GetFace( int id);
		int GetFaceID( Face* face);
		int GetFaceCount( void){ return (int)m_Faces.size(); }
		const std::vector<Face*>& GetFaces( void){ return m_Faces;}
		void RegisterFace( Face* face){ m_Faces.push_back( face);}
		bool UnregisterFace( Face* face);

	};
}

#endif

