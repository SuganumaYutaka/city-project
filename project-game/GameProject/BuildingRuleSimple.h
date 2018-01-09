/*==============================================================================

    BuildingRuleSimple.h - �����̎��������[�����𐶐����郋�[���[�V���v���Ȍ���
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/12
==============================================================================*/
#ifndef _BUILDING_RULE_SIMPLE_H_
#define _BUILDING_RULE_SIMPLE_H_

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
class BuildingRuleSimple : public BuildingRule
{
public:
	static BuildingRule* Create( BuildingSurfacePattern* surfacePattern);

	BuildingRuleSimple( float shapeHeight, float groundFloorHeight, float floorHeight, float windowWidth, float entranceWidth)
		: m_ShapeHeight( shapeHeight), m_GroundFloorHeight( groundFloorHeight), m_FloorHeight( floorHeight)
		, m_WindowWidth( windowWidth), m_EntranceWidth( entranceWidth){}
	~BuildingRuleSimple() override;

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
};

#endif

