/*==============================================================================

    Tile.cpp - �����̎��������[�^�C��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "TileCurve.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define TILE_DIVIDE_PER_RADIAN (0.1f)	//���W�A���p�ɉ����ă^�C���𕪊�����

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void TileCurve::Init( float height, float width, const Vector3& bottomLeftPosition, E_TILE_TYPE type, const TextureUV& texUV)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Center = Vector3( 0.0f, bottomLeftPosition.y, 0.0f);
	m_Type = type;
	m_TexUV = texUV;
}

/*------------------------------------------------------------------------------
	�ʒu�Ɣ��a�̍X�V
------------------------------------------------------------------------------*/
void TileCurve::Transform(D3DXMATRIX shapeMatrix)
{
	auto position = m_BottomLeftPosition.ConvertToDX();
	auto center = m_Center.ConvertToDX();

	D3DXVec3TransformCoord( &position, &position, &shapeMatrix);
	D3DXVec3TransformCoord( &center, &center, &shapeMatrix);
	m_BottomLeftPosition = Vector3::ConvertFromDX( position);
	m_Center = Vector3::ConvertFromDX( center);
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�̐ݒ�i�~�ɉ����ċȂ���j
------------------------------------------------------------------------------*/
void TileCurve::SetVertexBuffer(VERTEX_3D* pVtx)
{
	//�k�ރ|���S���ɒ��ӂ��Ē��_��ݒ�
	//��]�p�̏���
	float angle = CulcAngle();
	int countDivide = CulcCountDivide();
	float deltaAngle = angle / (float)( countDivide + 1);
	D3DXMATRIX mtxRotate;
	D3DXMatrixIdentity( &mtxRotate);
	D3DXMatrixRotationY( &mtxRotate, deltaAngle);
	D3DXVECTOR3 positionFromCenter = (m_BottomLeftPosition - m_Center).ConvertToDX();

	//UV�l�ݒ�̏���
	D3DXVECTOR2 topTexUV = m_TexUV.GetTopLeft();
	D3DXVECTOR2 bottomTexUV = m_TexUV.GetBottomLeft();
	float deltaTexU = m_TexUV.GetSize().x / (float)( countDivide + 1);
	
	int countVertex = CulcCountVertex();
	for (int i = 1; i < countVertex - 2; i += 2)
	{
		//�ʒu�̐ݒ�
		D3DXVECTOR3 pos = m_Center.ConvertToDX() + positionFromCenter;
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

	//�k�ރ|���S���̐ݒ�
	pVtx[ 0].Pos = m_BottomLeftPosition.ConvertToDX();
	pVtx[ 0].Normal = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	pVtx[ 0].Color = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[ 0].Tex = D3DXVECTOR2( 0.0f, 0.0f);

	D3DXVECTOR3 lastPosition;
	D3DXMatrixRotationY( &mtxRotate, angle);
	D3DXVec3TransformCoord( &lastPosition, &( m_BottomLeftPosition - m_Center).ConvertToDX(), &mtxRotate);
	lastPosition += m_Center.ConvertToDX();
	lastPosition.y += m_Height;
	pVtx[ countVertex - 1].Pos = lastPosition;
	pVtx[ countVertex - 1].Normal = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	pVtx[ countVertex - 1].Color = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[ countVertex - 1].Tex = D3DXVECTOR2( 0.0f, 0.0f);
}

/*------------------------------------------------------------------------------
	���_���̎Z�o
------------------------------------------------------------------------------*/
int TileCurve::CulcCountVertex( void)
{
	int countDivide = CulcCountDivide();
	int countVertex = ( countDivide + 2) * 2 + 2;

	return countVertex;
}

/*------------------------------------------------------------------------------
	�|���S�����̎Z�o
------------------------------------------------------------------------------*/
int TileCurve::CulcCountPolygon(void)
{
	int countDivide = CulcCountDivide();

	return 6 + countDivide * 2;
}

/*------------------------------------------------------------------------------
	���a����p�x���Z�o
------------------------------------------------------------------------------*/
float TileCurve::CulcAngle( void)
{
	float radius = Vector3::Distance( m_BottomLeftPosition, m_Center);
	return m_Width / radius;
}

/*------------------------------------------------------------------------------
	���������Z�o
------------------------------------------------------------------------------*/
int TileCurve::CulcCountDivide(void)
{
	float angle = CulcAngle();
	return (int)( angle / TILE_DIVIDE_PER_RADIAN);
}

