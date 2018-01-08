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
void Floor::Init( float height, float width, E_FLOOR_TYPE type, BuildingRule* rule)
{
	m_Height = height;
	m_Width = width;
	m_Type = type;

	//タイルの生成
	rule->ProceduralTile( this);
}

/*------------------------------------------------------------------------------
	頂点バッファの設定
------------------------------------------------------------------------------*/
bool Floor::SetVertexBuffer(VERTEX_3D* pVtx, const Vector3& bottomLeftPosition, const Vector3& normal, const Vector3& vector)
{
	//リスト状のタイルに設定させる
	Vector3 position = bottomLeftPosition;
	Tile* tile = m_Tile;
	for (;;)
	{	
		position = tile->SetVertexBuffer( pVtx, position, normal, vector);
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
	頂点バッファの設定（円に沿って曲げる）
------------------------------------------------------------------------------*/
bool Floor::SetVertexBufferCurve(VERTEX_3D* pVtx, const Vector3& bottomLeftPosition, const Vector3& center, float radius)
{
	//リスト状のタイルに設定させる
	Vector3 position = bottomLeftPosition;
	Tile* tile = m_Tile;
	for (;;)
	{	
		position = tile->SetVertexBufferCurve( pVtx, position, center, radius);
		pVtx += tile->CulcCountVertexCurve( radius);
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
	頂点数を算出（円に沿って曲げる場合）
------------------------------------------------------------------------------*/
int Floor::CulcCountVertexCurve( float radius)
{
	int count = 0;
	Tile* tile = m_Tile;
	for (;;)
	{
		count += tile->CulcCountVertexCurve( radius);
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
	ポリゴン数を算出（円に沿って曲げる場合）
------------------------------------------------------------------------------*/
int Floor::CulcCountPolygonCurve(float radius)
{
	int count = 0;
	Tile* tile = m_Tile;
	for (;;)
	{
		count += tile->CulcCountPolygonCurve( radius);
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
	}
	tile->SetNext( other->m_Tile);

	//Tileが消去されないようにotherのTileをNULLに
	other->m_Tile = NULL;
}
