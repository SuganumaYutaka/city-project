/*==============================================================================

    Edge.h - ハーフエッジ構造ー辺
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/
#ifndef _EDGE_H_
#define _EDGE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "HalfEdgeModel.h"
#include "Vector3.h"
#include "Attribute.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Vertex;
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
	class Edge
	{
	private:
		Vertex* m_Start;
		Vertex* m_End;
		HalfEdge* m_Left;
		HalfEdge* m_Right;
		HalfEdgeDataStructure::Model* m_Model;
		EdgeAttribute* m_Attribute;

	public:
		Edge( HalfEdgeDataStructure::Model* model, Vertex* start, Vertex* end, EdgeAttribute* attribute);
		~Edge(){ if( m_Attribute) delete m_Attribute; }

		Vertex* GetStart( void) const{ return m_Start;}
		void SetStart( Vertex* vertex) { m_Start = vertex; m_Attribute->Update();}

		Vertex* GetEnd( void) const{ return m_End;}
		void SetEnd( Vertex* vertex) { m_End = vertex; m_Attribute->Update();}

		HalfEdge* GetLeft( void) const { return m_Left;}
		void SetLeft( HalfEdge* he) { m_Left = he;}

		HalfEdge* GetRight( void) const { return m_Right;}
		void SetRight( HalfEdge* he) { m_Right = he;}

		Vector3 GetVector( void);

		bool Divide( float rateFromStart, Vertex** ppOut = NULL);

		EdgeAttribute* GetAttribute( void) { return m_Attribute;}

		void UpdateByMove( void);
	};
}

#endif

