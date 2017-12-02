/*==============================================================================

    Vertex.h - �n�[�t�G�b�W�\���[���_
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/
#ifndef _VERTEX_H_
#define _VERTEX_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Vector3.h"
#include <list>
#include "HalfEdgeModel.h"
#include "Attribute.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Edge;
	class Face;
	class HalfEdge;
	class VertexAttribute;
}

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Vertex
	{
	private:
		Vector3 m_Position;
		std::list<Edge*> m_Edges;
		HalfEdgeDataStructure::Model* m_Model;
		VertexAttribute* m_Attribute;
		void UpdateByMove( void);

	public:
		Vertex( HalfEdgeDataStructure::Model* model, const Vector3& position, VertexAttribute* attribute);
		~Vertex() { if( m_Attribute) delete m_Attribute; }

		const Vector3& GetPosition( void) const { return m_Position;}
		void SetPosition( Vector3 position) { m_Position = position; UpdateByMove();}

		const std::list<Edge*>& GetEdges( void){ return m_Edges;}
		bool RegisterEdge( Edge* edge);
		bool UnregisterEdge( Edge* edge);

		std::list<HalfEdge*> GetHalfEdges( void);
		HalfEdge* SearchHalfEdgeOnFace( Face* face);

		VertexAttribute* GetAttribute( void) { return m_Attribute;}
	};
}

#endif

