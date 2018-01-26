/*==============================================================================

    TileDefault.cpp - �����̎��������[�^�C���i�f�t�H���g�j
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/8
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "TileDefault.h"

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void TileDefault::Init( float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, E_TILE_TYPE type, const TextureUV& texUV)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = normal;
	m_Type = type;
	m_TexUV = texUV;
}

/*------------------------------------------------------------------------------
	�ʒu�Ɩ@���̍X�V
------------------------------------------------------------------------------*/
void TileDefault::Transform(D3DXMATRIX shapeMatrix)
{
	auto position = m_BottomLeftPosition.ConvertToDX();
	auto normal = m_Normal.ConvertToDX();

	D3DXVec3TransformCoord( &position, &position, &shapeMatrix);
	D3DXVec3TransformNormal( &normal, &normal, &shapeMatrix);

	m_BottomLeftPosition = Vector3::ConvertFromDX( position);
	m_Normal = Vector3::ConvertFromDX( normal);
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�̐ݒ�
------------------------------------------------------------------------------*/
void TileDefault::SetVertexBuffer(VERTEX_3D* pVtx)
{
	//�k�ރ|���S���ɒ��ӂ��Ē��_��ݒ�
	auto vector = Vector3::Cross( m_Normal, Vector3(0.0f, 1.0f, 0.0f)).Normalize();
	Vector3 nextPosition = m_BottomLeftPosition + vector * m_Width;

	/*pVtx[ 0].Pos = pVtx[ 1].Pos = D3DXVECTOR3( m_BottomLeftPosition.x, m_BottomLeftPosition.y, m_BottomLeftPosition.z);
	pVtx[ 2].Pos = D3DXVECTOR3( m_BottomLeftPosition.x, m_BottomLeftPosition.y + m_Height, m_BottomLeftPosition.z);
	pVtx[ 3].Pos = D3DXVECTOR3( nextPosition.x, nextPosition.y, nextPosition.z);
	pVtx[ 4].Pos = pVtx[ 5].Pos = D3DXVECTOR3( nextPosition.x, nextPosition.y + m_Height, nextPosition.z);*/

	pVtx[ 0].Pos = pVtx[ 1].Pos = D3DXVECTOR3( m_BottomLeftPosition.x, m_BottomLeftPosition.y + m_Height, m_BottomLeftPosition.z);
	pVtx[ 2].Pos = D3DXVECTOR3( m_BottomLeftPosition.x, m_BottomLeftPosition.y, m_BottomLeftPosition.z);
	pVtx[ 3].Pos = D3DXVECTOR3( nextPosition.x, nextPosition.y + m_Height, nextPosition.z);
	pVtx[ 4].Pos = pVtx[ 5].Pos = D3DXVECTOR3( nextPosition.x, nextPosition.y, nextPosition.z);

	for (int i = 0; i < 6; i++)
	{
		pVtx[ i].Normal = m_Normal.Normalize().ConvertToDX();
		pVtx[ i].Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].Tex = pVtx[5].Tex = D3DXVECTOR2( 0.0f, 0.0f); 
	
	//UV�iN���j
	pVtx[1].Tex = m_TexUV.GetTopLeft();
	pVtx[2].Tex = m_TexUV.GetBottomLeft();
	pVtx[3].Tex = m_TexUV.GetTopRight();
	pVtx[4].Tex = m_TexUV.GetBottomRight();
}

/*------------------------------------------------------------------------------
	���_���̎Z�o
------------------------------------------------------------------------------*/
int TileDefault::CulcCountVertex()
{
	return 4 + 2;
}

/*------------------------------------------------------------------------------
	�|���S�����̎Z�o
------------------------------------------------------------------------------*/
int TileDefault::CulcCountPolygon()
{
	return 6;
}
