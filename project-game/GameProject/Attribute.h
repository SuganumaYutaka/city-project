/*==============================================================================

    Attribute.h - �n�[�t�G�b�W�\���[�����̒��ۃC���^�[�t�F�C�X
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/28
==============================================================================*/
#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include <stdio.h>

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Vertex;
	class Edge;
	class Face;
}

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	//���ۃC���^�[�t�F�C�X
	class VertexAttribute
	{
	public:
		VertexAttribute(){ m_Vertex = NULL;}
		virtual ~VertexAttribute() {}
		void SetVertex( Vertex* vertex) { if( !m_Vertex) m_Vertex = vertex; }
		virtual void Init( void){}
		virtual void Update( void){}
		Vertex* GetVertex( void) { return m_Vertex;}

	private:
		Vertex* m_Vertex;
	};

	class EdgeAttribute
	{
	public:
		EdgeAttribute() { m_Edge = NULL;}
		virtual ~EdgeAttribute() {}
		void SetEdge( Edge* edge) { if( !m_Edge) m_Edge = edge; }
		virtual void Init( void){}
		virtual void Update( void){}
		Edge* GetEdge( void) { return m_Edge;}

	private:
		Edge* m_Edge;
	};

	class FaceAttribute
	{
	public:
		FaceAttribute() { m_Face = NULL;}
		virtual ~FaceAttribute() {}
		void SetFace( Face* face) { if( !m_Face) m_Face = face; }
		virtual void Init( void){}
		virtual void Update( void){}
		Face* GetFace( void) { return m_Face;}

	private:
		Face* m_Face;
	};

	class AttributeFactory
	{
	public:
		virtual VertexAttribute* CreateVertexAttribute( void) = 0;
		virtual EdgeAttribute* CreateEdgeAttribute( void) = 0;
		virtual FaceAttribute* CreateFaceAttribute( void) = 0;
	};
}

#endif

