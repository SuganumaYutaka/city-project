/*==============================================================================

    BuildingParameterSpawner.cpp - 建物の自動生成ー建物の生成に用いるパラメーターを生成
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BuildingParameterSpawner.h"
#include "BuildingParameter.h"
#include "BuildingSurfacePattern.h"

/*------------------------------------------------------------------------------
	ジオメトリ生成用パラメーター生成
------------------------------------------------------------------------------*/
GeometryParameter* GeometryParameterSpawner::operator()( const std::vector< BuildingSurfacePattern*>& surfacePatterns)
{
	//ランダムの初期化
	m_Random.ResetSeed();
	
	//パラメーター生成
	auto parameter = new GeometryParameter();
	
	//1階の高さ
	m_Random.SetRangeFloat( 1.2f, 1.5f);
	parameter->m_GroundFloorHeight = m_Random.GetFloat();
	
	//フロアの高さ
	m_Random.SetRangeFloat( 1.0f, 1.5f);
	parameter->m_FloorHeight = m_Random.GetFloat();

	//窓の幅
	m_Random.SetRangeFloat( 0.8f, 1.5f);
	parameter->m_WindowWidth = m_Random.GetFloat();

	//玄関の幅
	m_Random.SetRangeFloat( 2.0f, 2.5f);
	parameter->m_EntranceWidth = m_Random.GetFloat();

	//表面パターン
	m_Random.SetRangeInt( 0, surfacePatterns.size() - 1);
	parameter->m_SurfacePatternID = m_Random.GetInt();

	//ランダムのシード値
	parameter->m_RandomSeed = m_Random.GetSeed();

	return parameter;
}

/*------------------------------------------------------------------------------
	形状生成用パラメーター生成
------------------------------------------------------------------------------*/
void ShapeParameterSpawner::operator()(const std::vector<Vector3>& vertices, GeometryParameter* geometryParameter)
{
	//ランダムの初期化
	m_Random.ResetSeed();
	
	//土地の大きさ
	Vector3 vec01 = vertices[1] - vertices[0];
	Vector3 vec03 = vertices[3] - vertices[0];
	Vector3 landSize;
	landSize.x = vec01.Length();
	landSize.z = vec03.Length();

	//形状の高さ
	m_Random.SetRangeFloat( 20.0f, 25.0f);
	float heightMin = m_Random.GetFloat();
	m_Random.SetRangeFloat( 25.0f, 30.0f);
	float heightMax = heightMin + m_Random.GetFloat();

	//形状のサイズの比率
	float sizeRateMax = 0.9f;
	float sizeRateMin = 0.5f;
	
	//形状の生成数
	m_Random.SetRangeInt( 1, 2);
	int shapeCount = m_Random.GetInt();

	//形状の種類
	m_Random.SetRangeInt( 0, eShapeTypeCount - 1);
	E_SHAPE_TYPE type = (E_SHAPE_TYPE)m_Random.GetInt();

	//形状の生成
	for (int i = 0; i < shapeCount; i++)
	{
		auto shapeParameter = new ShapeParameter();

		//タイプの設定
		shapeParameter->m_Type = type;

		//ランダムの初期化
		m_Random.ResetSeed();
		
		//回転量の設定
		shapeParameter->m_Rotation = 0.0f;

		//大きさの設定
		m_Random.SetRangeFloat( sizeRateMin, sizeRateMax);
		shapeParameter->m_Size.x = landSize.x * m_Random.GetFloat();
		shapeParameter->m_Size.z = landSize.z * m_Random.GetFloat();

		//高さの設定
		m_Random.SetRangeFloat( heightMin, heightMax);
		shapeParameter->m_Size.y = m_Random.GetFloat();
	
		//位置の設定
		float range = ( landSize.x - shapeParameter->m_Size.x) * 0.5f;
		m_Random.SetRangeFloat( -range, range);
		shapeParameter->m_Position.x = m_Random.GetFloat();
		range = ( landSize.z - shapeParameter->m_Size.z) * 0.5f;
		m_Random.SetRangeFloat( -range, range);
		shapeParameter->m_Position.z = m_Random.GetFloat();
		//shapeParameter->m_Position.z = ( landSize.z - size.z) * 0.5f;

		//ジオメトリに追加
		geometryParameter->AddShapeParameter( shapeParameter);
	}
}
