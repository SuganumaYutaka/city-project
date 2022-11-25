/*==============================================================================

    HalfEdge.h - ハーフエッジ構造ーハーフエッジ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/
#ifndef _HALF_EDGE_H_
#define _HALF_EDGE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "HalfEdgeModel.h"
#include "Vector3.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Edge;
	class Face;
	class Vertex;
}

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class HalfEdge
	{
	private:
		Vertex* m_End;
		Face* m_Face;
		Edge* m_Edge;
		HalfEdge* m_Pair;
		HalfEdge* m_Next;
		HalfEdgeDataStructure::Model* m_Model;

	public:
		HalfEdge( HalfEdgeDataStructure::Model* model, Vertex* end)
			: m_Model( model), m_End( end), m_Face(NULL), m_Edge(NULL), m_Pair(NULL), m_Next(NULL) {}

		Vertex* GetEnd( void) const{ return m_End;}
		void SetEnd( Vertex* vertex) { m_End = vertex;}

		Face* GetFace( void) const{ return m_Face;}
		void SetFace( Face* face) { m_Face = face;}

		Edge* GetEdge( void) const{ return m_Edge;}
		void SetEdge( Edge* edge) { m_Edge = edge;}

		HalfEdge* GetPair( void) const{ return m_Pair;}
		void SetPair( HalfEdge* he) { m_Pair = he;}

		HalfEdge* GetNext( void) const{ return m_Next;}
		void SetNext( HalfEdge* he) { m_Next = he;}

		HalfEdge* GetPrev( void);

		Vector3 GetVector( void);

		HalfEdge* SearchParallelHalfEdge( void);

		bool IsStraightLine( HalfEdge* next);
	};
}

#endif

