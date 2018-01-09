/*==============================================================================

    TileCurve.h - 建物の自動生成ータイル（曲線）
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/8
==============================================================================*/
#ifndef _TILE_CURVE_H_
#define _TILE_CURVE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Tile.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class TileCurve : public Tile
{
public:
	void Init( float height, float width, const Vector3& bottomLeftPosition, E_TILE_TYPE type, const TextureUV& texUV);
	void Transform(D3DXMATRIX shapeMatrix) override;
	void SetVertexBuffer( VERTEX_3D* pVtx) override;
	int CulcCountVertex( void) override;
	int CulcCountPolygon( void) override;
	float CulcAngle( void);
	
private:
	int CulcCountDivide( void);

	Vector3 m_Center;

};

#endif

