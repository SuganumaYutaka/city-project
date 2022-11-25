/*==============================================================================

    BuildingParameterSpawner.h - 建物の自動生成ー建物の生成に用いるパラメーターを生成
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/
#ifndef _BUILDING_PARAMETER_SPAWNER_H_
#define _BUILDING_PARAMETER_SPAWNER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class ShapeParameter;
class GeometryParameter;
class BuildingSurfacePattern;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class GeometryParameterSpawner
{
public:
	GeometryParameter* operator()( const std::vector< BuildingSurfacePattern*>& surfacePatterns);

private:
	Random m_Random;

};

class ShapeParameterSpawner
{
public:
	void operator()( const std::vector<Vector3>& vertices, GeometryParameter* geometryParameter);

private:
	Random m_Random;

};

#endif

