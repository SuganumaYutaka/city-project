/*==============================================================================

    TileDefault.h - 建物の自動生成ータイル（デフォルト）
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/8
==============================================================================*/
#ifndef _TILE_DEFAULT_H_
#define _TILE_DEFAULT_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Tile.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class TileDefault : public Tile
{
public:
	void Init( float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, E_TILE_TYPE type, const TextureUV& texUV);
	void Transform(D3DXMATRIX shapeMatrix) override;
	void SetVertexBuffer( VERTEX_3D* pVtx) override;
	int CulcCountVertex( void) override;
	int CulcCountPolygon( void) override;
	
private:
	Vector3 m_Normal;
};

#endif

