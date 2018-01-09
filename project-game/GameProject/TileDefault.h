/*==============================================================================

    TileDefault.h - �����̎��������[�^�C���i�f�t�H���g�j
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/8
==============================================================================*/
#ifndef _TILE_DEFAULT_H_
#define _TILE_DEFAULT_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Tile.h"

/*------------------------------------------------------------------------------
	�N���X��`
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

