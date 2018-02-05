/*==============================================================================

    Face.h - ハーフエッジ構造ー面
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/
#ifndef _FACE_H_
#define _FACE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "HalfEdgeModel.h"
#include "Attribute.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class HalfEdge;
	class Model;
	class FaceAttribute;
}

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Face
	{
	private:
		HalfEdge* m_HalfEdge;
		Model* m_Model;
		HalfEdge* SearchStartHalfEdgeForDivide(Vertex* start, Vertex* end);
		FaceAttribute* m_Attribute;

	public:
		Face( Model* model, HalfEdge* he);
		~Face();

		HalfEdge* GetHalfEdge( void) const{ return m_HalfEdge;}
		void SetHalfEdge( HalfEdge* he) { m_HalfEdge = he;}

		bool Divide(Vertex* start, Vertex* end, Edge** ppOut = NULL);

		HalfEdge* SearchLongestHalfEdge( void);
		HalfEdge* SeachStraightLine( HalfEdge* startHalfEdge);

		void LinkAttribute( FaceAttribute* attribute){ m_Attribute = attribute;}
		FaceAttribute* GetAttribute( void) { return m_Attribute;}

		void UpdateByMove( void);
	};
}

#endif

