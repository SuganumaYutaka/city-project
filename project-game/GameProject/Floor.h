/*==============================================================================

    Floor.h - �����̎��������[�t���A�i�K�w�j
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _FLOOR_H_
#define _FLOOR_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Tile;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Floor
{
public:
	Floor();
	~Floor();
	void Init( float height, float width);
	
	Tile* GetTile( void) { return m_Tile;}

	bool SetVertexBuffer( VERTEX_3D* pVtx, const Vector3& bottomLeftPosition, const Vector3& normal, const Vector3& vector);
	bool SetVertexBufferCurve( VERTEX_3D* pVtx, const Vector3& bottomLeftPosition, const Vector3& center, float radius);

	int CulcCountVertex( void);
	int CulcCountVertexCurve( float radius);

	int CulcCountPolygon( void);
	int CulcCountPolygonCurve( float radius);

	float GetHeight( void) { return m_Height;}

private:
	Tile* m_Tile;
	float m_Height;
	float m_Width;
};

#endif

