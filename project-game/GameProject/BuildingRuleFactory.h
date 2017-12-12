/*==============================================================================

    BuildingRuleFactory.h - 建物の自動生成ー建物を生成するルールを生成する
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/8
==============================================================================*/
#ifndef _BUILDING_RULE_FACTORY_H_
#define _BUILDING_RULE_FACTORY_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class BuildingSurfacePattern;
class BuildingRule;
class Land;
class BlockAttribute;

/*------------------------------------------------------------------------------
	列挙型定義
------------------------------------------------------------------------------*/
//大きさの段階
enum E_BUILDING_SIZE_LEVEL
{
	eBuildingSizeSmall = 0,
	eBuildingSizeMiddle,
	eBuildingSizeLarge,
	eBuildingSizeLevelCount
};

//フロアの種類
enum E_FLOOR_TYPE
{
	eFloorDefault = 0,
	eFloorGround,
	eFloorMargin,
	eFloorBorder,
	eFloorTypeCount
};

//タイルの種類
enum E_TILE_TYPE
{
	eTileWall = 0,
	eTileWindow,
	eTileEntrance,
	eTileBorder,
	eTileTypeCount
};

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BuildingRuleFactory
{
public:
	BuildingRuleFactory();
	~BuildingRuleFactory();

	BuildingRule* CreateBuildingRule( const std::vector<Vector3>& vertices, BlockAttribute* attribute);


private:
	std::vector<BuildingSurfacePattern*> m_SurfacePatterns;
	std::mt19937  m_rand;
};

#endif

