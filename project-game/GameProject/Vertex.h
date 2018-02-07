/*==============================================================================

    Vertex.h - ハーフエッジ構造ー頂点
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/
#ifndef _VERTEX_H_
#define _VERTEX_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Vector3.h"
#include <list>
#include "HalfEdgeModel.h"
#include "Attribute.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Edge;
	class Face;
	class HalfEdge;
	class Model;
	class VertexAttribute;
}

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Vertex
	{
	private:
		Vector3 m_Position;
		std::list<Edge*> m_Edges;
		Model* m_Model;
		VertexAttribute* m_Attribute;

	public:
		Vertex( Model* model, const Vector3& position);
		~Vertex();
		void Delete( void);

		const Vector3& GetPosition( void) const { return m_Position;}
		void SetPosition( Vector3 position) { m_Position = position; UpdateByMove();}

		const std::list<Edge*>& GetEdges( void){ return m_Edges;}
		bool RegisterEdge( Edge* edge);
		bool UnregisterEdge( Edge* edge);

		std::list<HalfEdge*> GetHalfEdges( void);
		HalfEdge* SearchHalfEdgeOnFace( Face* face);

		void LinkAttribute( VertexAttribute* attribute){ m_Attribute = attribute;}
		VertexAttribute* GetAttribute( void) { return m_Attribute;}

		void UpdateByMove( void);
		void UpdateAttribute( void);
	};
}

#endif

