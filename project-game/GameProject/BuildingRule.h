/*==============================================================================

    BuildingRule.h - 建物の自動生成ー建物を生成するルール
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/12
==============================================================================*/
#ifndef _BUILDING_RULE_H_
#define _BUILDING_RULE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

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
class BuildingRule
{
public:
	BuildingRule( ) : m_SurfacePattern( NULL) {}
	virtual ~BuildingRule(){}

	virtual bool ProceduralShape( BuildingGeometry* geometry) = 0;
	virtual bool ProceduralFloor( Wall* wall) = 0;
	virtual bool ProceduralTile( Floor* floor) = 0;

	void SetSurfacePattern( BuildingSurfacePattern* surfacePattern) { m_SurfacePattern = surfacePattern;}
	BuildingSurfacePattern* GetSurfacePattern( void) { return m_SurfacePattern;}

private:
	BuildingSurfacePattern* m_SurfacePattern;
	
};

#endif

