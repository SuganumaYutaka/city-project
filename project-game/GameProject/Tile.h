/*==============================================================================

    Tile.h - 建物の自動生成ータイル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _TILE_H_
#define _TILE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define TILE_DIVIDE_PER_RADIAN (0.5f)	//ラジアン角に応じてタイルを分割する基準

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Tile
{
public:
	Tile();
	void Init( float height, float width);

	Vector3 SetVertexBuffer( VERTEX_3D* pVtx, const Vector3& bottomLeftPosition, const Vector3& normal, const Vector3& vector);
	Vector3 SetVertexBufferCurve( VERTEX_3D* pVtx, const Vector3& bottomLeftPosition, const Vector3& center, float radius);

	void SetNext( Tile* next) { m_Next = next;}
	Tile* GetNext( void){ return m_Next;}

	float CulcAngle( float radius) { return m_Width / radius;}
	int CulcCountDivide( float radius);

	int CulcCountVertex( void);
	int CulcCountVertexCurve( float radius);

	int CulcCountPolygon( void);
	int CulcCountPolygonCurve( float radius);

	float GetWidth( void){ return m_Width;}
	
private:
	Tile* m_Next;
	float m_Height;
	float m_Width;
	TextureUV m_TexUV;
};

#endif

