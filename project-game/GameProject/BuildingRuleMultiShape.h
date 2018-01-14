/*==============================================================================

    BuildingRuleMultiShape.h - 建物の自動生成ー建物を生成するルールー複数Shapeからなる建物
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/14
==============================================================================*/
#ifndef _BUILDING_RULE_MULTI_SHAPE_H_
#define _BUILDING_RULE_MULTI_SHAPE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "BuildingRule.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class BuildingSurfacePattern; 
class BuildingGeometry;
class Wall;
class Floor;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BuildingRuleMultiShape : public BuildingRule
{
public:
	static BuildingRule* Create( BuildingSurfacePattern* surfacePattern);

	~BuildingRuleMultiShape() override;

	bool ProceduralShape( BuildingGeometry* geometry) override;
	bool ProceduralFloor( Wall* wall)  override;
	bool ProceduralTile( Floor* floor)  override;
	bool ProceduralFloorCurve( Wall* wall)  override;
	bool ProceduralTileCurve( Floor* floor)  override;

	float GetShapeHeightMax(void) { return m_ShapeHeightMax;}
	float GetShapeHeightMin(void) { return m_ShapeHeightMin;}
	float GetGroundFloorHeight(void) { return m_GroundFloorHeight;}
	float GetFloorHeight(void) { return m_FloorHeight;}
	float GetWindowWidth(void) { return m_WindowWidth;}
	float GetEntranceWidth(void) { return m_EntranceWidth;}

private:
	//乱数
	Random* m_Random;

	//ProceduralShape
	float m_ShapeHeightMax;
	float m_ShapeHeightMin;
	float m_ShapeSizeRateMax;
	float m_ShapeSizeRateMin;

	//ProceduralFloor
	float m_GroundFloorHeight;
	float m_FloorHeight;

	//ProceduralTile
	float m_WindowWidth;
	float m_EntranceWidth;

	Vector3 MoveBottomLeftPosition( const Vector3& bottomLeftPosition, float angle);

	void CreateShapeBoxes( int shapeCount, BuildingGeometry* geometry, const Vector3& landSize);
	void CreateShapeCylinders( int shapeCount, BuildingGeometry* geometry, const Vector3& landSize);
};

#endif

