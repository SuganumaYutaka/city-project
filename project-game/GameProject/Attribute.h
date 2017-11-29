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

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	//抽象インターフェイス
	class VertexAttribute
	{
	public:
		virtual ~VertexAttribute() {}
	};

	class EdgeAttribute
	{
	public:
		virtual ~EdgeAttribute() {}
	};

	class FaceAttribute
	{
	public:
		virtual ~FaceAttribute() {}
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

