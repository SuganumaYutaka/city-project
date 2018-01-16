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
#include "BuildingRuleFactory.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Tile
{
public:
	virtual void Transform(D3DXMATRIX shapeMatrix) = 0;
	virtual void SetVertexBuffer( VERTEX_3D* pVtx) = 0;
	
	virtual int CulcCountVertex( void) = 0;
	virtual int CulcCountPolygon( void) = 0;
	
	void SetNext( Tile* next) { m_Next = next;}
	Tile* GetNext( void){ return m_Next;}
	float GetWidth( void){ return m_Width;}
	E_TILE_TYPE GetType(void){ return m_Type;}
	
protected:
	Tile* m_Next;
	float m_Height;
	float m_Width;
	Vector3 m_BottomLeftPosition;
	E_TILE_TYPE m_Type;
	TextureUV m_TexUV;
};

#endif

