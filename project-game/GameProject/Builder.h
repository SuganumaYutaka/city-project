/*==============================================================================

    Builder.h - 建物の自動生成ー建物組み立て
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/
#ifndef _BUILDER_H_
#define _BUILDER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "BuildingParameter.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class BuildingGeometry;
class Wall;
class Floor;
class ShapeBox;
class ShapeCylinder;
class BuildingSurfacePattern;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Builder
{
public:
	bool operator() ( BuildingGeometry* geometry, GeometryParameter* parameter, BuildingSurfacePattern* surfacePattern);

private:
	BuildingGeometry* m_Geometry;
	GeometryParameter* m_Parameter;
	BuildingSurfacePattern* m_SurfacePattern;

	ShapeBox* CreateShapeBox( ShapeParameter* parameter);
	ShapeCylinder* CreateShapeCylinder( ShapeParameter* parameter);
	bool CreateFloor( Wall* wall);
	bool CreateFloorCurve( Wall* wall);
	bool CreateTile( Floor* floor);
	bool CreateTileCurve( Floor* floor);

	Vector3 MoveBottomLeftPosition( const Vector3& bottomLeftPosition, float angle);

};

#endif

