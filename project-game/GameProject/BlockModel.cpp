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

#include "BuildingController.h"
#include "BuildingRuleFactory.h"
#include "BuildingManager.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define DEFAULT_LAND_SIZE (10.0f)			//デフォルトの土地サイズ
#define DISTANCE_OF_LANDS (0.5f)			//土地同士の間隔

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
	m_BuildingControllers.clear();
}

/*------------------------------------------------------------------------------
	建物の生成
	※　四角形の区画のみ対応
------------------------------------------------------------------------------*/
bool BlockModel::CreateBuilding( BlockAttribute* attribute)
{
	//現在の建物を削除
	auto buildingManager = attribute->GetBuildingManager();
	for (auto controller : m_BuildingControllers)
	{
		controller->m_pGameObject->ReleaseReserve();
		buildingManager->Unregister(controller);
	}
	m_BuildingControllers.clear();
	
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
		//辺の開始位置を道路幅の半分だけ移動する
		Vector3 position = corners[nCnt]->GetPosition();
		Vector3 vector;
		if (nCnt == 3)
		{
			vector = corners[ 0]->GetPosition() - corners[ 3]->GetPosition();
		}
		else
		{
			vector = corners[ nCnt + 1]->GetPosition() - corners[ nCnt]->GetPosition();
		}
		Vector3 vectorVertical = Vector3::Cross( Vector3(0.0f, 1.0f, 0.0f), vector);
		position += vector.Normalize() * DEFAULT_ROAD_WIDTH * 0.5f;
		position += vectorVertical.Normalize() * DEFAULT_ROAD_WIDTH * 0.5f;

		//辺の開始位置の設定
		edges[ nCnt].vertices.push_back( position);
	}
	for(int nCnt = 0; nCnt < 4; nCnt++)
	{
		//辺の道路情報の設定
		if (nCnt != 3)
		{
			SetRoadsFromCorner( corners[nCnt], corners[nCnt + 1], &edges[nCnt], attribute);
		}
		else
		{
			SetRoadsFromCorner( corners[ nCnt], corners[ 0], &edges[nCnt], attribute);
		}
	}
	for (int nCnt = 0; nCnt < 4; nCnt++)
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
			edges[ nCnt].vertices.push_back( edges[ nCnt].vertices.back() + vector);
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
			pointFourth = edges[ nCnt - 1].vertices.back();
		}
		else
		{
			pointFourth = edges[ 3].vertices.back();
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

			//土地に隣接する道路情報を設定
			if( edges[ nCnt].roads.size() > 0)
			{
				Vector3 midVertex = ( land.vertices[0] + land.vertices[1]) * 0.5f;
				float lengthFromCorner = Vector3::Distance( midVertex, edges[ nCnt].vertices[ 0]);
				float lengthEdge = edges[ nCnt].vector.Length();
				float length = lengthFromCorner / lengthEdge;
				for (auto road : edges[nCnt].roads)
				{
					if (length < road.second)
					{
						land.roads.push_back( road.first);
					}
				}
			
				//角地には手前の辺の最後の道路情報も設定
				if (i == 0)
				{
					if (nCnt != 0)
					{
						auto road = edges[ nCnt - 1].roads.back();
						land.roads.push_back( road.first);
					}
					else
					{
						auto road = edges[ 3].roads.back();
						land.roads.push_back( road.first);
					}
				}
			}
			//土地を追加
			lands.push_back( land);

			//次の生成にはpointThirdを利用する
			pointFourth = pointThird;
		}
	}

	//土地を狭める
	float roadWidthHalf = DEFAULT_ROAD_WIDTH * 0.5f;
	for (BlockLand& land : lands)
	{
		if (!land.canCreateBuilding)
		{
			//continue;
		}

		//土地の左側を間隔分狭める
		if ( !NarrowLand(land.vertices[1], land.vertices[0], DISTANCE_OF_LANDS * 1.0f))
		{
			land.canCreateBuilding = false;
			continue;
		}
		if ( !NarrowLand(land.vertices[2], land.vertices[3], DISTANCE_OF_LANDS * 1.0f))
		{
			land.canCreateBuilding = false;
			continue;
		}
	}

	//土地の衝突判定
	for (auto ite1 = lands.begin(); ite1 != lands.end(); ++ite1)
	{
		for (auto ite2 = lands.begin(); ite2 != lands.end(); ++ite2)
		{
			if (ite1 != ite2 && (*ite1).canCreateBuilding && (*ite2).canCreateBuilding)
			{
				if (CollisionLand(*ite1, *ite2))
				{
					(*ite1).canCreateBuilding = false;
				}
			}
		}
	}

	//建物を生成
	for (BlockLand& land : lands)
	{
		if (land.canCreateBuilding)
		{
			//建物の生成
			auto gameObject = new GameObject( m_pGameObject);
			auto controller = gameObject->AddComponent<BuildingController>();
			controller->Init( land.vertices, buildingRuleFactory->CreateBuildingRule( land.vertices, attribute), land.roads, attribute->GetBuildingManager(), attribute->GetCarManager());
			m_BuildingControllers.push_back( controller);
		}
	}

	//辺・土地データの解放
	for (BlockEdge& edge : edges)
	{
		edge.vertices.clear();
		edge.vertices.shrink_to_fit();
		edge.roads.clear();
	}
	for (BlockLand& land : lands)
	{
		land.vertices.clear();
		land.vertices.shrink_to_fit();
		land.roads.clear();
	}

	return true;
}

/*------------------------------------------------------------------------------
	土地を狭める
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
	土地を移動させる
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

/*------------------------------------------------------------------------------
	区画の角から道路情報を設定
------------------------------------------------------------------------------*/
bool BlockModel::SetRoadsFromCorner(Vertex* corner, Vertex* next, BlockEdge* blockedge, BlockAttribute* attribute)
{
	float blockedgeLength = Vector3::Distance( next->GetPosition(), corner->GetPosition());

	//はじめのHalfedgeを設定
	HalfEdge* firstHalfedge = corner->SearchHalfEdgeOnFace( attribute->GetFace());
	if (!firstHalfedge)
	{
		return false;
	}
	firstHalfedge = firstHalfedge->GetNext();
	if (!firstHalfedge)
	{
		return false;
	}
	
	//距離に応じて辺に占める割合を算出
	float length = firstHalfedge->GetVector().Length();
	
	//Edgeを設定
	blockedge->roads.push_back( std::make_pair( (RoadAttribute*)( firstHalfedge->GetEdge()->GetAttribute()), length / blockedgeLength));

	//Edgeが一つのみのとき正常終了
	if (firstHalfedge->GetEnd() == next)
	{
		return true;
	}

	//Halfedgeがnextに到達するまで設定
	auto halfEdge = firstHalfedge->GetNext();
	for (;;)
	{
		length += halfEdge->GetVector().Length();
		blockedge->roads.push_back( std::make_pair( (RoadAttribute*)( halfEdge->GetEdge()->GetAttribute()), length / blockedgeLength));

		if (halfEdge->GetEnd() == next)
		{
			return true;
		}
		halfEdge = halfEdge->GetNext();
	}

	return false;
}

/*------------------------------------------------------------------------------
	土地と土地の衝突判定
------------------------------------------------------------------------------*/
bool BlockModel::CollisionLand(const BlockLand& source, const BlockLand& dest)
{
	//ある頂点がすべての辺に対して右側にある（外積で判定）→衝突あり
	for (auto& vertex : source.vertices)
	{
		Vector3 Vec01 = dest.vertices[1] - dest.vertices[0];
		Vector3 Vec0P = vertex - dest.vertices[0];
		Vector3 Cross0 = Vector3::Cross(Vec01, Vec0P);
		if (Cross0.y < 0.0f)
		{
			continue;
		}

		Vector3 Vec12 = dest.vertices[2] - dest.vertices[1];
		Vector3 Vec1P = vertex - dest.vertices[1];
		Vector3 Cross1 = Vector3::Cross(Vec12, Vec1P);
		if (Cross1.y < 0.0f)
		{
			continue;
		}

		Vector3 Vec23 = dest.vertices[3] - dest.vertices[2];
		Vector3 Vec2P = vertex - dest.vertices[2];
		Vector3 Cross2 = Vector3::Cross(Vec23, Vec2P);
		if (Cross2.y < 0.0f)
		{
			continue;
		}

		Vector3 Vec30 = dest.vertices[0] - dest.vertices[3];
		Vector3 Vec3P = vertex - dest.vertices[3];
		Vector3 Cross3 = Vector3::Cross(Vec30, Vec3P);
		if (Cross3.y < 0.0f)
		{
			continue;
		}

		//衝突あり
		return true;
	}

	for (auto& vertex : dest.vertices)
	{
		Vector3 Vec01 = source.vertices[1] - source.vertices[0];
		Vector3 Vec0P = vertex - source.vertices[0];
		Vector3 Cross0 = Vector3::Cross(Vec01, Vec0P);
		if (Cross0.y < 0.0f)
		{
			continue;
		}

		Vector3 Vec12 = source.vertices[2] - source.vertices[1];
		Vector3 Vec1P = vertex - source.vertices[1];
		Vector3 Cross1 = Vector3::Cross(Vec12, Vec1P);
		if (Cross1.y < 0.0f)
		{
			continue;
		}

		Vector3 Vec23 = source.vertices[3] - source.vertices[2];
		Vector3 Vec2P = vertex - source.vertices[2];
		Vector3 Cross2 = Vector3::Cross(Vec23, Vec2P);
		if (Cross2.y < 0.0f)
		{
			continue;
		}

		Vector3 Vec30 = source.vertices[0] - source.vertices[3];
		Vector3 Vec3P = vertex - source.vertices[3];
		Vector3 Cross3 = Vector3::Cross(Vec30, Vec3P);
		if (Cross3.y < 0.0f)
		{
			continue;
		}

		//衝突あり
		return true;
	}

	//衝突なし
	return false;
}
