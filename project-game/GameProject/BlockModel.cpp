/*==============================================================================

    BlockModel.cpp - 町の自動生成ー区画モデル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/12
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BlockModel.h"
#include "GameObject.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"
#include "CityAttribute.h"

#include "BuildingGeometry.h"
#include "BuildingRuleFactory.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define DEFAULT_ROAD_WIDTH (8.0f)			//デフォルトの道路幅
#define DEFAULT_LAND_SIZE (10.0f)			//デフォルトの土地サイズ
#define DISTANCE_OF_LANDS (0.8f)			//土地同士の間隔

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* BlockModel::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BlockModel>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
BlockModel::BlockModel( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();


}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void BlockModel::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	区画ー建物の生成
	※　四角形の区画のみ対応
------------------------------------------------------------------------------*/
bool BlockModel::CreateBuilding( BlockAttribute* attribute)
{
	//現在の建物を削除
	for (auto geometries : m_BuildingGeometries)
	{
		geometries->m_pGameObject->ReleaseReserve();
	}
	m_BuildingGeometries.clear();
	
	//区画から角にあたる4頂点を抽出
	std::vector<Vertex*> corners;
	auto face = attribute->GetFace();
	auto halfEdge = face->GetHalfEdge();
	for (;;)
	{
		//内積が0前後→角とする
		float dot = Vector3::Dot(halfEdge->GetVector().Normalize(), halfEdge->GetNext()->GetVector().Normalize());
		if (dot < 0.85f)
		{
			corners.push_back( halfEdge->GetEnd());
		}

		halfEdge = halfEdge->GetNext();
		if (halfEdge == face->GetHalfEdge())
		{
			break;
		}
	}

	//4頂点以外の場合は未対応
	if (corners.size() != 4)
	{
		return false;
	}

	//属性からルール生成ファクトリーを取得
	auto buildingRuleFactory = attribute->GetBuildingRuleFactory();

	//4頂点から4辺を生成
	std::vector<BlockEdge> edges;
	edges.resize( 4);
	for(int nCnt = 0; nCnt < 4; nCnt++)
	{
		//辺の開始位置の設定
		edges[ nCnt].vertices.push_back( corners[nCnt]->GetPosition());
	}
	for(int nCnt = 0; nCnt < 4; nCnt++)
	{
		//辺のベクトルの設定
		Vector3 nextPosition;
		if (nCnt != 3)
		{
			nextPosition = edges[ nCnt + 1].vertices[0];
		}
		else
		{
			nextPosition = edges[ 0].vertices[0];
		}
		edges[ nCnt].vector = nextPosition - edges[ nCnt].vertices[0];

		//辺に隣接する土地数の設定
		int numLand = (int)( edges[ nCnt].vector.Length() / DEFAULT_LAND_SIZE);

		//辺を土地数で割ったベクトル（となりの点へのベクトル）を設定
		auto vector  = edges[ nCnt].vector / (float)numLand;

		//辺を等分する点を設定
		for (int i = 0; i < numLand - 1; i++)
		{
			edges[ nCnt].vertices.push_back( edges[ nCnt].vertices[0] + vector * (float)( i + 1));
		}
	}

	//土地を設定
	std::vector<BlockLand> lands;
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//3点から面を作成するために、前の辺の最後の点を設定
		Vector3 pointFourth;
		if (nCnt != 0)
		{
			int prevSize = edges[ nCnt - 1].vertices.size();
			pointFourth = edges[ nCnt - 1].vertices[ prevSize - 1];
		}
		else
		{
			int prevSize = edges[ 3].vertices.size();
			pointFourth = edges[ 3].vertices[ prevSize - 1];
		}

		//土地を生成
		int numVertex = edges[nCnt].vertices.size();
		for (int i = 0; i < numVertex - 1; i++)
		{
			//4つ目の頂点（登録順は3番目）を設定
			Vector3 pointThird = pointFourth + edges[ nCnt].vector / (float)( numVertex);
			
			//土地を生成
			BlockLand land;
			land.vertices.resize(4);
			land.vertices[0] = edges[nCnt].vertices[i];
			land.vertices[1] = edges[nCnt].vertices[i + 1];
			land.vertices[2] = pointThird;
			land.vertices[3] = pointFourth;
			land.canCreateBuilding = true;
			lands.push_back( land);

			//次の生成にはpointThirdを利用する
			pointFourth = pointThird;
		}
	}

	//TODO: 土地の衝突判定→土地の合体
	
	//土地を狭める
	float roadWidthHalf = DEFAULT_ROAD_WIDTH * 0.5f;
	for (BlockLand& land : lands)
	{
		//土地を道路幅分狭める
		if ( !NarrowLand(land.vertices[0], land.vertices[3], DEFAULT_ROAD_WIDTH * 0.5f))
		{
			land.canCreateBuilding = false;
			continue;
		}
		if ( !NarrowLand(land.vertices[1], land.vertices[2], DEFAULT_ROAD_WIDTH * 0.5f))
		{
			land.canCreateBuilding = false;
			continue;
		}

		//道路幅分右にずらす
		if (!MoveLand(land.vertices[0], land.vertices[1], DEFAULT_ROAD_WIDTH * 0.5f))
		{
			land.canCreateBuilding = false;
			continue;
		}
		if (!MoveLand(land.vertices[3], land.vertices[2], DEFAULT_ROAD_WIDTH * 0.5f))
		{
			land.canCreateBuilding = false;
			continue;
		}

		//土地の右側を間隔分狭める
		if ( !NarrowLand(land.vertices[1], land.vertices[0], DISTANCE_OF_LANDS))
		{
			land.canCreateBuilding = false;
			continue;
		}
		if ( !NarrowLand(land.vertices[2], land.vertices[3], DISTANCE_OF_LANDS))
		{
			land.canCreateBuilding = false;
			continue;
		}
	}

	//建物を生成
	for (BlockLand& land : lands)
	{
		if (land.canCreateBuilding)
		{
			//建物の生成
			auto gameObject = new GameObject( m_pGameObject);
			auto geometry = gameObject->AddComponent<BuildingGeometry>();
			geometry->Init( land.vertices, buildingRuleFactory->CreateBuildingRule( land.vertices, attribute));
			m_BuildingGeometries.push_back( geometry);
		}
	}

	//辺・土地データの解放
	for (BlockEdge& edge : edges)
	{
		edge.vertices.clear();
		edge.vertices.shrink_to_fit();
	}
	for (BlockLand& land : lands)
	{
		land.vertices.clear();
		land.vertices.shrink_to_fit();
	}

	return true;
}

/*------------------------------------------------------------------------------
	区画ー土地を狭める
------------------------------------------------------------------------------*/
bool BlockModel::NarrowLand(Vector3& start, Vector3& end, float value)
{
	Vector3 vector;
	vector = end - start;
	if (vector.Length() < value)
	{
		return false;
	}
	vector = vector.Normalize();
	start += vector * value;

	return true;
}

/*------------------------------------------------------------------------------
	区画ー土地を移動させる
------------------------------------------------------------------------------*/
bool BlockModel::MoveLand(Vector3& start, Vector3& end, float value)
{
	Vector3 vector;
	vector = end - start;
	if (vector.Length() < value)
	{
		return false;
	}
	vector = vector.Normalize();
	start += vector * value;
	end += vector * value;

	return true;
}
