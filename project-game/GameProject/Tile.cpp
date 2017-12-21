/*==============================================================================

    Tile.cpp - �����̎��������[�^�C��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Tile.h"
#include "BuildingRule.h"
#include "BuildingSurfacePattern.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Tile::Tile() : m_Next( NULL)
{
	m_Height = 1.0f;
	m_Width = 1.0f;
	m_Type = eTileWall;
}


/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void Tile::Init( float height, float width, E_TILE_TYPE type, const TextureUV& texUV)
{
	m_Height = height;
	m_Width = width;
	m_Type = type;
	m_TexUV = texUV;
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�̐ݒ�
------------------------------------------------------------------------------*/
Vector3 Tile::SetVertexBuffer(VERTEX_3D* pVtx, const Vector3& bottomLeftPosition, const Vector3& normal, const Vector3& vector)
{
	//�k�ރ|���S���ɒ��ӂ��Ē��_��ݒ�
	Vector3 nextPosition = bottomLeftPosition + vector * m_Width;

	pVtx[ 0].Pos = pVtx[ 1].Pos = D3DXVECTOR3( bottomLeftPosition.x, bottomLeftPosition.y, bottomLeftPosition.z);
	pVtx[ 2].Pos = D3DXVECTOR3( bottomLeftPosition.x, bottomLeftPosition.y + m_Height, bottomLeftPosition.z);
	pVtx[ 3].Pos = D3DXVECTOR3( nextPosition.x, nextPosition.y, nextPosition.z);
	pVtx[ 4].Pos = pVtx[ 5].Pos = D3DXVECTOR3( nextPosition.x, nextPosition.y + m_Height, nextPosition.z);

	for (int i = 0; i < 6; i++)
	{
		pVtx[ i].Normal = normal.ConvertToDX();
		pVtx[ i].Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].Tex = pVtx[5].Tex = D3DXVECTOR2( 0.0f, 0.0f); 
	
	//UV�iN���j
	pVtx[1].Tex = m_TexUV.GetBottomLeft();
	pVtx[2].Tex = m_TexUV.GetTopLeft();
	pVtx[3].Tex = m_TexUV.GetBottomRight();
	pVtx[4].Tex = m_TexUV.GetTopRight();

	return nextPosition;
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�̐ݒ�i�~�ɉ����ċȂ���j
------------------------------------------------------------------------------*/
Vector3 Tile::SetVertexBufferCurve(VERTEX_3D* pVtx, const Vector3& bottomLeftPosition,  const Vector3& center, float radius)
{
	//�k�ރ|���S���ɒ��ӂ��Ē��_��ݒ�
	
	//��]�p�̏���
	float angle = CulcAngle( radius);
	int countDivide = CulcCountDivide( radius);
	float deltaAngle = angle / (float)( countDivide + 1);
	D3DXMATRIX mtxRotate;
	D3DXMatrixIdentity( &mtxRotate);
	D3DXMatrixRotationY( &mtxRotate, deltaAngle);
	D3DXVECTOR3 positionFromCenter = (bottomLeftPosition - center).ConvertToDX();

	//UV�l�ݒ�̏���
	D3DXVECTOR2 topTexUV = m_TexUV.GetTopLeft();
	D3DXVECTOR2 bottomTexUV = m_TexUV.GetBottomLeft();
	float deltaTexU = m_TexUV.GetSize().x / (float)( countDivide + 1);
	
	int countVertex = CulcCountVertexCurve( radius);
	for (int i = 1; i < countVertex - 2; i++)
	{
		//�ʒu�̐ݒ�
		D3DXVECTOR3 pos = center.ConvertToDX() + positionFromCenter;
		pVtx[ i].Pos = pos;
		pos.y += m_Height;
		pVtx[ i + 1].Pos = pos;

		//�@���̐ݒ�
		D3DXVECTOR3 normal;
		D3DXVec3Normalize( &normal, &positionFromCenter);
		pVtx[ i].Normal = pVtx[ i + 1].Normal = normal;

		//�F�̐ݒ�
		pVtx[ i].Color = pVtx[ i + 1].Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

		//UV�̐ݒ�
		pVtx[ i].Tex = bottomTexUV;
		pVtx[ i + 1].Tex = topTexUV;
		
		//���̐ݒ�Ɍ����čX�V
		D3DXVec3TransformCoord( &positionFromCenter, &positionFromCenter, &mtxRotate);
		topTexUV.x += deltaTexU;
		bottomTexUV.x += deltaTexU;
	}

	//return���鎟��bottomLeftPosition��ݒ�
	D3DXVECTOR3 nextPosition;
	D3DXMatrixRotationY( &mtxRotate, angle);
	D3DXVec3TransformCoord( &nextPosition, &( bottomLeftPosition - center).ConvertToDX(), &mtxRotate);
	nextPosition += center.ConvertToDX();

	//�k�ރ|���S���̐ݒ�
	pVtx[ 0].Pos = bottomLeftPosition.ConvertToDX();
	pVtx[ 0].Normal = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	pVtx[ 0].Color = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[ 0].Tex = D3DXVECTOR2( 0.0f, 0.0f);

	D3DXVECTOR3 lastPosition = nextPosition;
	lastPosition.y += m_Height;
	pVtx[ countVertex - 1].Pos = lastPosition;
	pVtx[ countVertex - 1].Normal = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	pVtx[ countVertex - 1].Color = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[ countVertex - 1].Tex = D3DXVECTOR2( 0.0f, 0.0f);

	return Vector3::ConvertFromDX( nextPosition);
}

/*------------------------------------------------------------------------------
	���_���̎Z�o
------------------------------------------------------------------------------*/
int Tile::CulcCountVertex()
{
	return 4 + 2;
}

/*------------------------------------------------------------------------------
	���_���̎Z�o�i�~�ɉ����ċȂ���ꍇ�j
------------------------------------------------------------------------------*/
int Tile::CulcCountVertexCurve( float radius)
{
	int countDivide = CulcCountDivide( radius);
	int countVertex = ( countDivide + 2) * 2 + 2;

	return countVertex;
}

/*------------------------------------------------------------------------------
	�|���S�����̎Z�o
------------------------------------------------------------------------------*/
int Tile::CulcCountPolygon()
{
	return 6;
}

/*------------------------------------------------------------------------------
	�|���S�����̎Z�o�i�~�ɉ����ċȂ���ꍇ�j
------------------------------------------------------------------------------*/
int Tile::CulcCountPolygonCurve( float radius)
{
	int countDivide = CulcCountDivide( radius);


	return 6 + countDivide * 2;
}

/*------------------------------------------------------------------------------
	���������Z�o
------------------------------------------------------------------------------*/
int Tile::CulcCountDivide(float radius)
{
	float angle = CulcAngle( radius);
	return (int)( angle / TILE_DIVIDE_PER_RADIAN);
}

