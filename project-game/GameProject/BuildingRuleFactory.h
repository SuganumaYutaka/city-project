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
	Random* m_Random;
};

#endif

