/*==============================================================================

    BuildingRuleFactory.cpp - 建物の自動生成ー建物を生成するルールを生成する
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/8
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BuildingRuleFactory.h"
#include "BuildingRule.h"
#include "Land.h"
#include "BuildingSurfacePattern.h"
#include "DebugLog.h"

#include "BuildingRuleSimple.h"
#include "BuildingRuleCylinder.h"
#include "BuildingRuleMultiShape.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
BuildingRuleFactory::BuildingRuleFactory()
{
	m_Random = new Random();

	//建物の表面パターンを読み込み
	m_SurfacePatterns.push_back( new BuildingSurfacePattern( "data/SCRIPT/BuildingSurfacePattern/test03.txt"));
	m_SurfacePatterns.push_back( new BuildingSurfacePattern( "data/SCRIPT/BuildingSurfacePattern/test02.txt"));
	

	m_Random->SetRangeInt( 0, (int)m_SurfacePatterns.size() - 1);
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
BuildingRuleFactory::~BuildingRuleFactory()
{
	for (auto pattern : m_SurfacePatterns)
	{
		delete pattern;
	}
	m_SurfacePatterns.clear();
	m_SurfacePatterns.shrink_to_fit();
}

/*------------------------------------------------------------------------------
	建物生成ルールを生成
------------------------------------------------------------------------------*/
BuildingRule* BuildingRuleFactory::CreateBuildingRule(const std::vector<Vector3>& vertices, BlockAttribute* attribute)
{
	//TODO:土地の大きさや区画の属性に応じてルールを生成

	BuildingRule* rule;

	//サーフェスパターンの決定
	int pattern = m_Random->GetInt();

	//ルールの生成
	//rule = BuildingRuleSimple::Create(m_SurfacePatterns[ pattern]);
	//rule = BuildingRuleCylinder::Create(m_SurfacePatterns[ pattern]);
	rule = BuildingRuleMultiShape::Create(m_SurfacePatterns[ pattern]);
	
	if (!rule)
	{
		DebugLog::Add( "BuildingRuleFactory:ルールの生成に失敗しました\n");
		return NULL;
	}

	return rule;
}
