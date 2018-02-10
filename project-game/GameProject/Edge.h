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
	class Model;
	class EdgeAttribute;
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
		Model* m_Model;
		EdgeAttribute* m_Attribute;

	public:
		Edge( Model* model, Vertex* start, Vertex* end);
		~Edge();
		void Delete( void);

		Vertex* GetStart( void) const{ return m_Start;}
		void SetStart( Vertex* vertex) { m_Start = vertex; UpdateAttribute();}

		Vertex* GetEnd( void) const{ return m_End;}
		void SetEnd( Vertex* vertex) { m_End = vertex; UpdateAttribute();}

		HalfEdge* GetLeft( void) const { return m_Left;}
		void SetLeft( HalfEdge* he) { m_Left = he;}

		HalfEdge* GetRight( void) const { return m_Right;}
		void SetRight( HalfEdge* he) { m_Right = he;}

		Vector3 GetVector( void);

		bool Divide( float rateFromStart, Vertex** ppOut = NULL);

		void LinkAttribute( EdgeAttribute* attribute){ m_Attribute = attribute;}
		EdgeAttribute* GetAttribute( void) { return m_Attribute;}

		void UpdateByMove( void);
		void UpdateAttribute( void);
		
	};
}

#endif

