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
	class Rule;
	class AttributeFactory;
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
		std::list<Vertex*> m_Vertices;
		std::list<Edge*> m_Edges;
		std::list<Face*> m_Faces;
		
		Rule* m_Rule;
		AttributeFactory* m_AttributeFactory;

	public:
		Model( Rule* rule, AttributeFactory* attributeFactory) : m_Rule( rule), m_AttributeFactory( attributeFactory){}
		~Model();

		Vertex* CreateVertex( const Vector3& position);
		Edge* CreateEdge( Vertex* start, Vertex* end);
		Face* CreateFace( HalfEdge* he);

		bool CreateFirstVertex( const Vector3& position);
		bool CreateFirstEdge( const Vector3& startPosition, const Vector3& endPosition);
		bool CreateFirstFace( 
			const Vector3& topLeftPosition, const Vector3& topRightPosition, 
			const Vector3& bottomLeftPosition, const Vector3& bottomRightPosition);

		bool DivideAllFaces( void);

		const std::list<Vertex*>& GetVertices( void){ return m_Vertices;}
		void RegisterVertex( Vertex* vertex){ m_Vertices.push_back( vertex);}
		bool UnregisterDeleteVertex( Vertex* vertex)
		{
			for (auto ite = m_Vertices.begin(); ite != m_Vertices.end(); ++ite)
			{
				if (*ite == vertex)
				{
					m_Vertices.erase( ite);
					return true;
				}
			}
			return false;
		}

		const std::list<Edge*>& GetEdges( void){ return m_Edges;}
		void RegisterEdge( Edge* edge){ m_Edges.push_back( edge);}
		bool UnregisterDeleteEdge( Edge* edge)
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

		const std::list<Face*>& GetFaces( void){ return m_Faces;}
		void RegisterFace( Face* face){ m_Faces.push_back( face);}
		bool UnregisterDeleteFace( Face* face)
		{
			for (auto ite = m_Faces.begin(); ite != m_Faces.end(); ++ite)
			{
				if (*ite == face)
				{
					m_Faces.erase( ite);
					return true;
				}
			}
			return false;
		}
	};
}

#endif

