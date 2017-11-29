/*==============================================================================

    Edge.h - �n�[�t�G�b�W�\���[��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/
#ifndef _EDGE_H_
#define _EDGE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "HalfEdgeModel.h"
#include "Vector3.h"
#include "Attribute.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Vertex;
	class Face;
	class HalfEdge;
}

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
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
		Edge( HalfEdgeDataStructure::Model* model, Vertex* start, Vertex* end, EdgeAttribute* attribute)
			: m_Model(model), m_Start(start), m_End(end), m_Left(NULL), m_Right(NULL), m_Attribute(attribute) { model->RegisterEdge(this); }
		~Edge(){ if( m_Attribute) delete m_Attribute; }

		Vertex* GetStart( void) const{ return m_Start;}
		void SetStart( Vertex* vertex) { m_Start = vertex;}

		Vertex* GetEnd( void) const{ return m_End;}
		void SetEnd( Vertex* vertex) { m_End = vertex;}

		HalfEdge* GetLeft( void) const { return m_Left;}
		void SetLeft( HalfEdge* he) { m_Left = he;}

		HalfEdge* GetRight( void) const { return m_Right;}
		void SetRight( HalfEdge* he) { m_Right = he;}

		Vector3 GetVector( void);

		bool Divide( float rateFromStart, Vertex** ppOut = NULL);

		EdgeAttribute* GetAttribute( void) { return m_Attribute;}
	};
}

#endif

