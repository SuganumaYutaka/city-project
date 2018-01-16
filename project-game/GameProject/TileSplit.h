/*==============================================================================

    TileSplit.h - �����̎��������[�^�C���i����ځj
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/11
==============================================================================*/
#ifndef _TILE_SPLIT_H_
#define _TILE_SPLIT_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Tile.h"

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class TileSplit : public Tile
{
public:
	TileSplit() : m_OtherSplit(NULL){}
	void Init(TileSplit* other, const Vector3& bottomLeftPosition);
	void Transform(D3DXMATRIX shapeMatrix) override {}
	void SetVertexBuffer( VERTEX_3D* pVtx) override {}
	int CulcCountVertex( void) override { return 0;}
	int CulcCountPolygon( void) override{ return 0;}

	TileSplit* GetOtherSplit( void){ return m_OtherSplit;}
	Tile* GetTileOtherSplit( void){ return m_OtherSplit->GetNext(); }
	
private:
	TileSplit* m_OtherSplit;

};

#endif

