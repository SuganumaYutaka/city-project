/*==============================================================================

    BuildingGeometry.cpp - 建物の自動生成ー建物ジオメトリ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BuildingGeometry.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "Land.h"
#include "Shape.h"
#include "BuildingRule.h"

#include "Tile.h"
#include "TileSplit.h"
#include "WallRenderer.h"
#include "BuildingSurfacePattern.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define MAX_TILE (300)

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* BuildingGeometry::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BuildingGeometry>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
BuildingGeometry::BuildingGeometry( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void BuildingGeometry::Uninit( void)
{
	if (m_Rule)
	{
		delete m_Rule;
	}

	if (m_Land)
	{
		delete m_Land;
	}
	
	for (Shape* shape : m_Shapes)
	{
		delete shape;
	}
	m_Shapes.clear();
}

/*------------------------------------------------------------------------------
	初期化処理
------------------------------------------------------------------------------*/
bool BuildingGeometry::Init( const std::vector<Vector3>& vertices, BuildingRule* rule)
{
	//土地の生成
	m_Land = new Land( m_pGameObject);
	
	//Position(World座標)を土地の中心に
	Vector3 vec02 = vertices[2] - vertices[0];
	auto center = vertices[0] + vec02 * 0.5f;
	m_pTransform->SetWorldPosition( center);

	//土地が道路と平行となるように回転
	Vector3 vec21 = vertices[1] - vertices[2];
	m_pTransform->SetWorldRotationLookDirection( vec21);

	//相対位置を算出して土地を設定
	std::vector<Vector3> landVertices = vertices;
	for (auto& vertex : landVertices)
	{
		vertex -= center;
	}
	m_Land->Init( landVertices);

	//形状の生成
	rule->ProceduralShape( this);

	m_Rule = rule;

	return true;
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void BuildingGeometry::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	ジオメトリを確定させる
------------------------------------------------------------------------------*/
void BuildingGeometry::ConfirmGeometry(void)
{
	//単体Shapeの場合
	if (m_Shapes.size() == 1)
	{
		//壁の融合
		m_Shapes.front()->ConfirmShape();

		//描画の更新
		m_Shapes.front()->UpdateView();

		return;
	}

	//Shapeの融合
	//Shape同士で一度ずつ処理
	for (auto ite1 = m_Shapes.begin(); *ite1 != m_Shapes.back(); ++ite1)
	{
		auto ite2 = ite1;
		++ite2;
		for (; ite2 != m_Shapes.end(); ++ite2)
		{
			//交差するところに裂け目を設定
			(*ite1)->Split( *ite2);
		}
	}

	//形状を確定させる
	for (auto shape : m_Shapes)
	{
		shape->ConfirmShape();
	}

	//壁のRendererの消去
	for (auto shape : m_Shapes)
	{
		shape->ClearWallRenderer();
	}

	//各Shapeからフロア数分タイルの先頭を取得
	std::list<Tile*> tiles;
	int floorCount = 0;
	for (;;)
	{
		//先頭タイルを取得
		Tile* tile = NULL;
		for (auto shape : m_Shapes)
		{
			tile = SearchStartTile( shape, floorCount);
			if (tile)
			{
				//各フロアにつき１つ見つかればＯＫ
				break;
			}
		}

		//表示できる先頭タイルがないとき格納終了
		if (!tile)
		{
			break;
		}
		
		//タイルを格納して階数を一つ上げる
		tiles.push_back( tile);
		floorCount++;
	}

	//各Shapeの最後のFloorの先頭タイルを格納
	for (auto shape : m_Shapes)
	{
		auto tile = shape->GetTopTile();
		tiles.push_back( tile);
	}
	

	//1枚のWallRendererに表に出ている全フロアの全タイルの描画情報を設定
	//頂点数とポリゴン数を算出
	int vertexCount = 0;
	int polygonCount = 0;

	for (auto start : tiles)
	{
		//タイルのリストをたどって頂点数・ポリゴン数を加算
		auto tile = start;
		int tileCount = 0;
		for (;;)
		{
			vertexCount += tile->CulcCountVertex();
			polygonCount += tile->CulcCountPolygon();

			tile = tile->GetNext();
			if (!tile || tile == start)
			{
				break;
			}

			//他のShapeとのつなぎ目があるとき他のShapeのタイルにジャンプする
			if (tile->GetType() == eTileSplit)
			{
				auto split = dynamic_cast<TileSplit*>( tile);
				if (split)
				{
					tile = split->GetOtherSplit();
					
					//prevも設定
					auto prev = split->GetOtherSplit()->GetPrevTile();
					vertexCount += prev->CulcCountVertex();
					polygonCount += prev->CulcCountPolygon();
				}
			}

			//タイルが最大数を超えたら終了
			tileCount++;
			if (tileCount > MAX_TILE)
			{
				break;
			}
		}
	}

	//WallRendererを設定
	auto wallRenderer = m_pGameObject->AddComponent<WallRenderer>();
	wallRenderer->LoadTexture( m_Rule->GetSurfacePattern()->GetTextureFileName());
	auto pVtx = wallRenderer->StartSetVertexBuffer( vertexCount + 1, polygonCount);
	for (auto start : tiles)
	{
		//タイルのリストをたどって頂点情報を設定
		auto tile = start;
		int tileCount = 0;
		for (;;)
		{
			tile->SetVertexBuffer(pVtx);
			pVtx += tile->CulcCountVertex();

			tile = tile->GetNext();
			if (!tile || tile == start)
			{
				break;
			}

			//他のShapeとのつなぎ目があるとき他のShapeのタイルにジャンプする
			if (tile->GetType() == eTileSplit)
			{
				auto split = dynamic_cast<TileSplit*>( tile);
				if (split)
				{
					tile = split->GetOtherSplit();
					
					//prevも設定
					auto prev = split->GetOtherSplit()->GetPrevTile();
					prev->SetVertexBuffer(pVtx);
					pVtx += prev->CulcCountVertex();
				}
			}

			//タイルが最大数を超えたら終了
			tileCount++;
			if (tileCount > MAX_TILE)
			{
				break;
			}
		}
	}
	wallRenderer->EndSetVertexBuffer();

}

/*------------------------------------------------------------------------------
	指定階層の表にある先頭タイルを探索
------------------------------------------------------------------------------*/
Tile* BuildingGeometry::SearchStartTile(Shape* shape, int floorCount)
{
	//フロアの一番上は設定しない
	if (floorCount >= shape->GetFloorCount() - 1)
	{
		return NULL;
	}

	//Shapeから先頭タイルを取得
	auto tile = shape->GetStartTile( floorCount);
	if (!tile)
	{
		return NULL;
	}

	//環状リストをたどって表にあるタイルをさがす
	auto start = tile;
	for (;;)
	{
		//タイルが表にあるか判定
		if (CanLookTile(tile, shape))
		{
			//対象のタイル発見
			return tile;
		}

		tile = tile->GetNext();
		if (!tile || tile == start)
		{
			break;
		}
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	タイルを視認できるか
------------------------------------------------------------------------------*/
bool BuildingGeometry::CanLookTile(Tile* tile, Shape* owner)
{
	for (auto otherShape : m_Shapes)
	{
		if (owner == otherShape)
		{
			continue;
		}

		//タイルが他のShapeの中にあるか確認
		if ( otherShape->CollisionPoint(tile->GetBottomLeftPosition()))
		{
			//視認できない
			return false;
		}
	}
	
	//他のShapeと衝突なし→視認できる
	return true;
}
