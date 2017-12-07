/*==============================================================================

    Floor.cpp - �����̎��������[�t���A�i�K�w�j
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Floor.h"
#include "Tile.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Floor::Floor() : m_Tile( NULL)
{
	m_Height = 1.0f;
	m_Width = 1.0f;
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Floor::~Floor()
{
	//�^�C���̏���
	Tile* targetTile = m_Tile;
	Tile* nextTile;
	for (;;)
	{
		nextTile = targetTile->GetNext();
		delete targetTile;
		if (!nextTile)
		{
			break;
		}
	}
}


/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void Floor::Init( float height, float width)
{
	m_Height = height;
	m_Width = width;

	//TODO:�^�C���̐���

}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�̐ݒ�
------------------------------------------------------------------------------*/
bool Floor::SetVertexBuffer(VERTEX_3D* pVtx, const Vector3& bottomLeftPosition, const Vector3& normal, const Vector3& vector)
{
	//���X�g��̃^�C���ɐݒ肳����
	Vector3 position = bottomLeftPosition;
	Tile* tile = m_Tile;
	for (;;)
	{	
		position = tile->SetVertexBuffer( pVtx, position, normal, vector);
		tile->GetNext();
		if (!tile)
		{
			break;
		}
	}

	return true;
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�̐ݒ�i�~�ɉ����ċȂ���j
------------------------------------------------------------------------------*/
bool Floor::SetVertexBufferCurve(VERTEX_3D* pVtx, const Vector3& bottomLeftPosition, const Vector3& center, float radius)
{
	//���X�g��̃^�C���ɐݒ肳����
	Vector3 position = bottomLeftPosition;
	Tile* tile = m_Tile;
	for (;;)
	{	
		position = tile->SetVertexBufferCurve( pVtx, position, center, radius);
		tile->GetNext();
		if (!tile)
		{
			break;
		}
	}

	return true;
}

/*------------------------------------------------------------------------------
	���_�����Z�o
------------------------------------------------------------------------------*/
int Floor::CulcCountVertex(void)
{
	int count = 0;
	Tile* tile = m_Tile;
	for (;;)
	{
		count += tile->CulcCountVertex();
		tile = tile->GetNext();
		if (tile == NULL)
		{
			break;
		}
	}

	return count;
}

/*------------------------------------------------------------------------------
	���_�����Z�o�i�~�ɉ����ċȂ���ꍇ�j
------------------------------------------------------------------------------*/
int Floor::CulcCountVertexCurve( float radius)
{
	int count = 0;
	Tile* tile = m_Tile;
	for (;;)
	{
		count += tile->CulcCountVertexCurve( radius);
		tile = tile->GetNext();
		if (tile == NULL)
		{
			break;
		}
	}

	return count;
}

/*------------------------------------------------------------------------------
	�|���S�������Z�o
------------------------------------------------------------------------------*/
int Floor::CulcCountPolygon(void)
{
	int count = 0;
	Tile* tile = m_Tile;
	for (;;)
	{
		count += tile->CulcCountPolygon();
		tile = tile->GetNext();
		if (tile == NULL)
		{
			break;
		}
	}

	return count;
}

/*------------------------------------------------------------------------------
	�|���S�������Z�o�i�~�ɉ����ċȂ���ꍇ�j
------------------------------------------------------------------------------*/
int Floor::CulcCountPolygonCurve(float radius)
{
	int count = 0;
	Tile* tile = m_Tile;
	for (;;)
	{
		count += tile->CulcCountPolygonCurve( radius);
		tile = tile->GetNext();
		if (tile == NULL)
		{
			break;
		}
	}

	return count;
}
