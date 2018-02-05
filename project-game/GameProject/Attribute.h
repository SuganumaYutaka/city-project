/*==============================================================================

    Attribute.h - ハーフエッジ構造ー属性の抽象インターフェイス
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/28
==============================================================================*/
#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include <stdio.h>

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Model;
	class Vertex;
	class Edge;
	class Face;
}

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	//抽象インターフェイス
	class VertexAttribute
	{
	public:
		VertexAttribute( Model* model, int id);
		virtual ~VertexAttribute() {}
		virtual void Init(){}
		virtual void Update( void){}
		void OnDeleteVertex( void);
		Vertex* GetVertex( void);
		
	private:
		int m_ID;
		Model* m_Model;
	};

	class EdgeAttribute
	{
	public:
		EdgeAttribute( Model* model, int id);
		virtual ~EdgeAttribute() {}
		virtual void Init( void){}
		virtual void Update( void){}
		void OnDeleteEdge( void);
		Edge* GetEdge( void);

	private:
		int m_ID;
		Model* m_Model;
	};

	class FaceAttribute
	{
	public:
		FaceAttribute( Model* model, int id);
		virtual ~FaceAttribute() {}
		virtual void Init( void){}
		virtual void Update( void){}
		void OnDeleteFace( void);
		Face* GetFace( void);

	private:
		int m_ID;
		Model* m_Model;
	};
}

#endif

