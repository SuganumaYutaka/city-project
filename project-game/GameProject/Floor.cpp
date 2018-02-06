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
#include "TileSplit.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define INSERT_PERMIT_RANGE (0.01f)

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Floor::Floor() : m_Tile( NULL)
{
	m_Height = 1.0f;
	m_Width = 1.0f;
	m_Type = eFloorDefault;
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Floor::~Floor()
{
	if (!m_Tile)
	{
		return;
	}

	//�^�C���̏���
	Tile* targetTile = m_Tile;
	Tile* nextTile;
	for (;;)
	{
		nextTile = targetTile->GetNext();
		delete targetTile;
		if (!nextTile || nextTile == m_Tile)
		{
			break;
		}
		targetTile = nextTile;
	}
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void Floor::InitDefault( float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, E_FLOOR_TYPE type)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = normal;
	m_Type = type;
}

/*------------------------------------------------------------------------------
	�������i�~�ɉ����ċȂ���j
------------------------------------------------------------------------------*/
void Floor::InitCurve( float height, float width, const Vector3& bottomLeftPosition, E_FLOOR_TYPE type)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = bottomLeftPosition.Normalize();
	m_Type = type;
}

/*------------------------------------------------------------------------------
	�ʒu�Ɩ@���̍X�V
------------------------------------------------------------------------------*/
void Floor::Transform(D3DXMATRIX shapeMatrix)
{
	//�t���A�̍X�V
	auto position = m_BottomLeftPosition.ConvertToDX();
	auto normal = m_Normal.ConvertToDX();

	D3DXVec3TransformCoord( &position, &position, &shapeMatrix);
	D3DXVec3TransformNormal( &normal, &normal, &shapeMatrix);

	m_BottomLeftPosition = Vector3::ConvertFromDX( position);
	m_Normal = Vector3::ConvertFromDX( normal);

	//�^�C���̍X�V
	if (!m_Tile)
	{
		return;
	}

	Tile* tile = m_Tile;
	for (;;)
	{	
		tile->Transform( shapeMatrix);
		tile = tile->GetNext();
		if (!tile || tile == m_Tile)
		{
			break;
		}
	}
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�̐ݒ�
------------------------------------------------------------------------------*/
bool Floor::SetVertexBuffer(VERTEX_3D* pVtx)
{
	//���X�g��̃^�C���ɐݒ肳����
	Tile* tile = m_Tile;
	for (;;)
	{	
		tile->SetVertexBuffer( pVtx);
		pVtx += tile->CulcCountVertex();
		tile = tile->GetNext();
		if (!tile || tile == m_Tile)
		{
			break;
		}

		//����Shape�Ƃ̂Ȃ��ڂ�����Ƃ�����Shape�̃^�C���ɃW�����v����
		if (tile->GetType() == eTileSplit)
		{
			auto split = dynamic_cast<TileSplit*>( tile);
			if (split)
			{
				tile = split->GetTileOtherSplit();
			}
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
		if (tile == NULL || tile == m_Tile)
		{
			break;
		}

		//����Shape�Ƃ̂Ȃ��ڂ�����Ƃ�����Shape�̃^�C���ɃW�����v����
		if (tile->GetType() == eTileSplit)
		{
			auto split = dynamic_cast<TileSplit*>( tile);
			if (split)
			{
				tile = split->GetTileOtherSplit();
			}
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
		if (tile == NULL || tile == m_Tile)
		{
			break;
		}

		//����Shape�Ƃ̂Ȃ��ڂ�����Ƃ�����Shape�̃^�C���ɃW�����v����
		if (tile->GetType() == eTileSplit)
		{
			auto split = dynamic_cast<TileSplit*>( tile);
			if (split)
			{
				tile = split->GetTileOtherSplit();
			}
		}
	}

	return count;
}

/*------------------------------------------------------------------------------
	����Shape��Floor�ƗZ������
------------------------------------------------------------------------------*/
void Floor::FusionSameShape(Floor* other)
{
	//�Ō��Tile��Next��other�̍ŏ���Tile�ɂ���
	auto tile = GetBackTile();
	tile->SetNext( other->m_Tile);

	//Tile����������Ȃ��悤��other��Tile��NULL��
	other->m_Tile = NULL;
}

/*------------------------------------------------------------------------------
	�I�[�̃^�C���̎擾
------------------------------------------------------------------------------*/
Tile* Floor::GetBackTile(void)
{
	auto tile = m_Tile;
	for (;;)
	{
		//Next��NULL�̏ꏊ���I�[�Ƃ��ĕԂ�
		if (!tile->GetNext())
		{
			return tile;
		}

		tile = tile->GetNext();

		//�󃊃X�g�̂Ƃ�NULL��Ԃ�
		if (tile == m_Tile)
		{
			break;
		}
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	�󃊃X�g�ɕύX
------------------------------------------------------------------------------*/
bool Floor::ChangeRingList(void)
{
	//�I�[Tile��Next�ɐ擪Tile��ݒ�
	auto back = GetBackTile();
	if (!back)
	{
		return false;
	}

	back->SetNext( m_Tile);
	return true;
}

/*------------------------------------------------------------------------------
	���ڂ̑}��
------------------------------------------------------------------------------*/
bool Floor::InsertSplit(TileSplit* split, float lengthFromBottomLeft)
{
	//BottomLeftPosition���琳�����ʒu�ɑ}��
	Tile* tile = m_Tile;
	float length = 0.0f;
	for (;;)
	{
		//�n�_����̋������^�C���̕�������
		length += tile->GetWidth();
		
		//�n�_���狗�������ڂ��������Ƃ��A�^�C���Ǝ��̃^�C���̊Ԃɑ}��
		if (length + 0.001f >= lengthFromBottomLeft)
		{
			split->SetNext( tile->GetNext());
			tile->SetNext( split);
			split->SetPrevTile( tile);
			return true;
		}

		tile = tile->GetNext();
		if (tile == NULL || tile == m_Tile)
		{
			break;
		}
	}

	////�덷�͈̔͂̂Ƃ��Ō�̃^�C����Next�ɐݒ�
	//if (lengthFromBottomLeft - length < INSERT_PERMIT_RANGE)
	//{
	//	auto tile = GetBackTile();
	//	split->SetNext( NULL);
	//	tile->SetNext( split);
	//	split->SetPrevTile( tile);
	//	return true;
	//}

	//�}���ӏ��Ȃ�
	return false;
}
