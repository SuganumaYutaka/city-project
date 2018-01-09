/*==============================================================================

    Floor.cpp - 建物の自動生成ーフロア（階層）
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Floor.h"
#include "Tile.h"
#include "BuildingRule.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Floor::Floor() : m_Tile( NULL)
{
	m_Height = 1.0f;
	m_Width = 1.0f;
	m_Type = eFloorDefault;
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Floor::~Floor()
{
	if (!m_Tile)
	{
		return;
	}

	//タイルの消去
	Tile* targetTile = m_Tile;
	Tile* nextTile;
	for (;;)
	{
		nextTile = targetTile->GetNext();
		delete targetTile;
		if (!nextTile)
		{
			break;
		}
		targetTile = nextTile;
	}
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void Floor::InitDefault( float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, E_FLOOR_TYPE type, BuildingRule* rule)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = normal;
	m_Type = type;

	//タイルの生成
	rule->ProceduralTile( this);
}

/*------------------------------------------------------------------------------
	初期化（円に沿って曲げる）
------------------------------------------------------------------------------*/
void Floor::InitCurve( float height, float width, const Vector3& bottomLeftPosition, E_FLOOR_TYPE type, BuildingRule* rule)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = bottomLeftPosition.Normalize();
	m_Type = type;

	//タイルの生成
	rule->ProceduralTileCurve( this);
}

/*------------------------------------------------------------------------------
	タイルの座標の更新
------------------------------------------------------------------------------*/
void Floor::Transform(D3DXMATRIX shapeMatrix)
{
	if (!m_Tile)
	{
		return;
	}

	Tile* tile = m_Tile;
	for (;;)
	{	
		tile->Transform( shapeMatrix);
		tile = tile->GetNext();
		if (!tile)
		{
			break;
		}
	}
}

/*------------------------------------------------------------------------------
	頂点バッファの設定
------------------------------------------------------------------------------*/
bool Floor::SetVertexBuffer(VERTEX_3D* pVtx)
{
	//リスト状のタイルに設定させる
	Tile* tile = m_Tile;
	for (;;)
	{	
		tile->SetVertexBuffer( pVtx);
		pVtx += tile->CulcCountVertex();
		tile = tile->GetNext();
		if (!tile)
		{
			break;
		}
	}

	return true;
}

/*------------------------------------------------------------------------------
	頂点数を算出
------------------------------------------------------------------------------*/
int Floor::CulcCountVertex(void)
{
	int count = 0;
	Tile* tile = m_Tile;
	for (;;)
	{
		count += tile->CulcCountVertex();
		tile = tile->GetNext();
		if (tile == NULL)
		{
			break;
		}
	}

	return count;
}

/*------------------------------------------------------------------------------
	ポリゴン数を算出
------------------------------------------------------------------------------*/
int Floor::CulcCountPolygon(void)
{
	int count = 0;
	Tile* tile = m_Tile;
	for (;;)
	{
		count += tile->CulcCountPolygon();
		tile = tile->GetNext();
		if (tile == NULL)
		{
			break;
		}
	}

	return count;
}

/*------------------------------------------------------------------------------
	同一ShapeのFloorと融合する
------------------------------------------------------------------------------*/
void Floor::FusionSameShape(Floor* other)
{
	//最後のTileのNextをotherの最初のTileにする
	auto tile = m_Tile;
	for (;;)
	{
		if (!tile->GetNext())
		{
			break;
		}
		tile = tile->GetNext();
	}
	tile->SetNext( other->m_Tile);

	//Tileが消去されないようにotherのTileをNULLに
	other->m_Tile = NULL;
}
