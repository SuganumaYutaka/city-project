/*==============================================================================

    CityAttribute.h - 町の自動生成ー属性
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/29
==============================================================================*/
#ifndef _CITY_ATTRIBUTE_H_
#define _CITY_ATTRIBUTE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Attribute.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	//点（交差点）
	class JunctionAttribute : public VertexAttribute
	{
	public:
		
	};

	//辺（道路）
	class RoadAttribute : public EdgeAttribute
	{
	private:
		float Width;
	public:
		RoadAttribute() : Width( 0.0f) {}
		float GetWidth( void) const{ return Width;}
		void SetWidth( float width){ Width = width;}
	};

	//面（区画）
	class LandAttribute : public FaceAttribute
	{
	public:
		
	};

	//ファクトリー
	class CityAttributeFactory : public AttributeFactory
	{
	public:
		virtual VertexAttribute* CreateVertexAttribute( void) { return new JunctionAttribute(); }
		virtual EdgeAttribute* CreateEdgeAttribute( void) { return new RoadAttribute(); }
		virtual FaceAttribute* CreateFaceAttribute( void){ return new LandAttribute(); }
	};
}

#endif

