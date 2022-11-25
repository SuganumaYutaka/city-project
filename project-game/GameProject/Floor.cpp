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
#include "TileSplit.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define INSERT_PERMIT_RANGE (0.01f)

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
		if (!nextTile || nextTile == m_Tile)
		{
			break;
		}
		targetTile = nextTile;
	}
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void Floor::InitDefault( float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, E_FLOOR_TYPE type)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = normal;
	m_Type = type;
}

/*------------------------------------------------------------------------------
	初期化（円に沿って曲げる）
------------------------------------------------------------------------------*/
void Floor::InitCurve( float height, float width, const Vector3& bottomLeftPosition, E_FLOOR_TYPE type)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = bottomLeftPosition.Normalize();
	m_Type = type;
}

/*------------------------------------------------------------------------------
	位置と法線の更新
------------------------------------------------------------------------------*/
void Floor::Transform(D3DXMATRIX shapeMatrix)
{
	//フロアの更新
	auto position = m_BottomLeftPosition.ConvertToDX();
	auto normal = m_Normal.ConvertToDX();

	D3DXVec3TransformCoord( &position, &position, &shapeMatrix);
	D3DXVec3TransformNormal( &normal, &normal, &shapeMatrix);

	m_BottomLeftPosition = Vector3::ConvertFromDX( position);
	m_Normal = Vector3::ConvertFromDX( normal);

	//タイルの更新
	if (!m_Tile)
	{
		return;
	}

	Tile* tile = m_Tile;
	for (;;)
	{	
		tile->Transform( shapeMatrix);
		tile = tile->GetNext();
		if (!tile || tile == m_Tile)
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
		if (!tile || tile == m_Tile)
		{
			break;
		}

		//他のShapeとのつなぎ目があるとき他のShapeのタイルにジャンプする
		if (tile->GetType() == eTileSplit)
		{
			auto split = dynamic_cast<TileSplit*>( tile);
			if (split)
			{
				tile = split->GetTileOtherSplit();
			}
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
		if (tile == NULL || tile == m_Tile)
		{
			break;
		}

		//他のShapeとのつなぎ目があるとき他のShapeのタイルにジャンプする
		if (tile->GetType() == eTileSplit)
		{
			auto split = dynamic_cast<TileSplit*>( tile);
			if (split)
			{
				tile = split->GetTileOtherSplit();
			}
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
		if (tile == NULL || tile == m_Tile)
		{
			break;
		}

		//他のShapeとのつなぎ目があるとき他のShapeのタイルにジャンプする
		if (tile->GetType() == eTileSplit)
		{
			auto split = dynamic_cast<TileSplit*>( tile);
			if (split)
			{
				tile = split->GetTileOtherSplit();
			}
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
	auto tile = GetBackTile();
	tile->SetNext( other->m_Tile);

	//Tileが消去されないようにotherのTileをNULLに
	other->m_Tile = NULL;
}

/*------------------------------------------------------------------------------
	終端のタイルの取得
------------------------------------------------------------------------------*/
Tile* Floor::GetBackTile(void)
{
	auto tile = m_Tile;
	for (;;)
	{
		//NextがNULLの場所を終端として返す
		if (!tile->GetNext())
		{
			return tile;
		}

		tile = tile->GetNext();

		//環状リストのときNULLを返す
		if (tile == m_Tile)
		{
			break;
		}
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	環状リストに変更
------------------------------------------------------------------------------*/
bool Floor::ChangeRingList(void)
{
	//終端TileのNextに先頭Tileを設定
	auto back = GetBackTile();
	if (!back)
	{
		return false;
	}

	back->SetNext( m_Tile);
	return true;
}

/*------------------------------------------------------------------------------
	境目の挿入
------------------------------------------------------------------------------*/
bool Floor::InsertSplit(TileSplit* split, float lengthFromBottomLeft)
{
	//BottomLeftPositionから正しい位置に挿入
	Tile* tile = m_Tile;
	float length = 0.0f;
	for (;;)
	{
		//始点からの距離をタイルの幅分足す
		length += tile->GetWidth();
		
		//始点から距離が境目よりも遠いとき、タイルと次のタイルの間に挿入
		if (length + 0.001f >= lengthFromBottomLeft)
		{
			split->SetNext( tile->GetNext());
			tile->SetNext( split);
			split->SetPrevTile( tile);
			return true;
		}

		tile = tile->GetNext();
		if (tile == NULL || tile == m_Tile)
		{
			break;
		}
	}

	////誤差の範囲のとき最後のタイルのNextに設定
	//if (lengthFromBottomLeft - length < INSERT_PERMIT_RANGE)
	//{
	//	auto tile = GetBackTile();
	//	split->SetNext( NULL);
	//	tile->SetNext( split);
	//	split->SetPrevTile( tile);
	//	return true;
	//}

	//挿入箇所なし
	return false;
}
