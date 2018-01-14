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
#include "BuildingRule.h"
#include "TileSplit.h"

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
void Floor::InitDefault( float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, E_FLOOR_TYPE type, BuildingRule* rule)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = normal;
	m_Type = type;

	//�^�C���̐���
	rule->ProceduralTile( this);
}

/*------------------------------------------------------------------------------
	�������i�~�ɉ����ċȂ���j
------------------------------------------------------------------------------*/
void Floor::InitCurve( float height, float width, const Vector3& bottomLeftPosition, E_FLOOR_TYPE type, BuildingRule* rule)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = bottomLeftPosition.Normalize();
	m_Type = type;

	//�^�C���̐���
	rule->ProceduralTileCurve( this);
}

/*------------------------------------------------------------------------------
	�^�C���̍��W�̍X�V
------------------------------------------------------------------------------*/
void Floor::Transform(D3DXMATRIX shapeMatrix)
{
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

		//�󃊃X�g�̂Ƃ�NULL��Ԃ�
		else if (tile->GetNext() == m_Tile)
		{
			break;
		}

		tile = tile->GetNext();
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