/*==============================================================================

    Builder.h - �����̎��������[�����g�ݗ���
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/
#ifndef _BUILDER_H_
#define _BUILDER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "BuildingParameter.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class BuildingGeometry;
class Wall;
class Floor;
class ShapeBox;
class ShapeCylinder;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Builder
{
public:
	bool operator() ( BuildingGeometry* geometry, GeometryParameter* parameter);

private:
	BuildingGeometry* m_Geometry;
	GeometryParameter* m_Parameter;

	ShapeBox* CreateShapeBox( ShapeParameter* parameter);
	ShapeCylinder* CreateShapeCylinder( ShapeParameter* parameter);
	bool CreateFloor( Wall* wall);
	bool CreateFloorCurve( Wall* wall);
	bool CreateTile( Floor* floor);
	bool CreateTileCurve( Floor* floor);

	Vector3 MoveBottomLeftPosition( const Vector3& bottomLeftPosition, float angle);

};

#endif

