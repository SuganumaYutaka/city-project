/*==============================================================================

    BuildingRuleCylinder.h - �����̎��������[�����𐶐����郋�[���[�~��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/9
==============================================================================*/
#ifndef _BUILDING_RULE_CYLINDER_H_
#define _BUILDING_RULE_CYLINDER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "BuildingRule.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class BuildingSurfacePattern; 
class BuildingGeometry;
class Wall;
class Floor;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class BuildingRuleCylinder : public BuildingRule
{
public:
	static BuildingRule* Create( BuildingSurfacePattern* surfacePattern);

	BuildingRuleCylinder( float shapeHeight, float groundFloorHeight, float floorHeight, float windowWidth, float entranceWidth)
		: m_ShapeHeight( shapeHeight), m_GroundFloorHeight( groundFloorHeight), m_FloorHeight( floorHeight)
		, m_WindowWidth( windowWidth), m_EntranceWidth( entranceWidth){}
	~BuildingRuleCylinder() override;

	bool ProceduralShape( BuildingGeometry* geometry) override;
	bool ProceduralFloor( Wall* wall)  override;
	bool ProceduralTile( Floor* floor)  override;
	bool ProceduralFloorCurve( Wall* wall)  override;
	bool ProceduralTileCurve( Floor* floor)  override;

	float GetShapeHeight(void) { return m_ShapeHeight;}
	float GetGroundFloorHeight(void) { return m_GroundFloorHeight;}
	float GetFloorHeight(void) { return m_FloorHeight;}
	float GetWindowWidth(void) { return m_WindowWidth;}
	float GetEntranceWidth(void) { return m_EntranceWidth;}

private:
	//����
	Random* m_Random;

	//ProceduralShape
	float m_ShapeHeight;

	//ProceduralFloor
	float m_GroundFloorHeight;
	float m_FloorHeight;

	//ProceduralTile
	float m_WindowWidth;
	float m_EntranceWidth;

	Vector3 MoveBottomLeftPosition( const Vector3& bottomLeftPosition, float angle);
};

#endif

