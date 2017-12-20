/*==============================================================================

    BuildingRuleSimple.cpp - �����̎��������[�����𐶐����郋�[���[�V���v���Ȍ���
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/12
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BuildingRuleSimple.h"
#include "BuildingSurfacePattern.h"
#include "BuildingGeometry.h"
#include "Wall.h"
#include "Floor.h"
#include "Tile.h"
#include "Land.h"

#include "ShapeBox.h"

/*------------------------------------------------------------------------------
	���[���̐���
------------------------------------------------------------------------------*/
BuildingRule* BuildingRuleSimple::Create( BuildingSurfacePattern* surfacePattern, std::mt19937& random)
{
	//�`��̍���(10-30)
	float shapeHeight = 10.0 + (float)( random() % 1000) * 0.001f * 20.0f;

	//1�K�̍���(2-4)
	float groundFloorHeight = 2.0f + (float)( random() % 1000) * 0.001f * 2.0f;
	
	//�t���A�̍���(2-3)
	float floorHeight = 2.0f + (float)( random() % 1000) * 0.001f * 1.0f;

	//���̕�(1-3)
	float windowWidth = 1.0f + (float)( random() % 1000) * 0.001f * 2.0f;

	//���ւ̕�(2-6)
	float entranceWidth = 2.0f + (float)( random() % 1000) * 0.001f * 4.0f;

	//���[���̐���
	auto rule = new BuildingRuleSimple( shapeHeight, groundFloorHeight, floorHeight, windowWidth, entranceWidth);
	rule->SetSurfacePattern( surfacePattern);

	return rule;
}

/*------------------------------------------------------------------------------
	�`��̐���
------------------------------------------------------------------------------*/
bool BuildingRuleSimple::ProceduralShape(BuildingGeometry* geometry)
{
	auto land = geometry->GetLand();
	auto vertices = land->GetVertices();
	Vector3 vec01 = vertices[1] - vertices[0];
	Vector3 vec03 = vertices[3] - vertices[0];
	Vector3 size;
	size.x = vec01.Length();
	size.z = vec03.Length();
	size.y = m_ShapeHeight;
	
	auto shape = new ShapeBox( geometry->GetGameObject());
	shape->Init( Vector3(0.0f, 0.0f, 0.0f), 0.0f, size, this);
	geometry->AddShape( shape);

	return true;
}

/*------------------------------------------------------------------------------
	�t���A�̐���
------------------------------------------------------------------------------*/
bool BuildingRuleSimple::ProceduralFloor( Wall* wall)
{
	//�e�N�X�`���̐ݒ�
	wall->LoadTexture( GetSurfacePattern()->GetTextureFileName());

	float height = wall->GetHeight();
	float width = wall->GetWidth();
	Vector3 bottomLeft = wall->GetBottomLeftPosition();
	Floor* floor = NULL;

	//1�K
	floor = new Floor();
	floor->Init( m_GroundFloorHeight, width, eFloorGround, this);
	wall->AddFloor( floor);
	height -= m_GroundFloorHeight;

	//����ȊO�̃t���A
	for (;;)
	{
		if (height < m_FloorHeight)
		{
			break;
		}
		floor = new Floor();
		floor->Init( m_FloorHeight, width, eFloorDefault, this);
		wall->AddFloor( floor);
		height -= m_FloorHeight;
	}

	//�㕔�ɗ]��
	floor = new Floor();
	floor->Init( height, width, eFloorMargin, this);
	wall->AddFloor( floor);

	return true;
}

/*------------------------------------------------------------------------------
	�^�C���̐���
------------------------------------------------------------------------------*/
bool BuildingRuleSimple::ProceduralTile( Floor* floor)
{
	float height = floor->GetHeight();
	float width = floor->GetWidth();
	Tile* tile = NULL;
	Tile* tileNext = NULL;

	//int countWindow = (int)( width / m_WindowWidth);
	//float wallWidth = ( width - countWindow * m_WindowWidth) * 0.5f;
	
	//��������葫��Ȃ��ꍇ�ǂ̂ݐݒ�
	if (width < m_WindowWidth)
	{
		tile = new Tile();
		floor->SetTile( tile);
		tile->Init( height, width, eTileWall, this);
		return true;
	}

	//���E�̐ݒ�
	if (floor->GetType() == eFloorBorder)
	{
		tile = new Tile();
		floor->SetTile( tile);
		tile->Init( height, width, eTileBorder, this);
		return true;
	}

	int countWindow = (int)( width / m_WindowWidth);
	float wallWidth = ( width - countWindow * m_WindowWidth) * 0.5f;

	//1�K�̐ݒ�i�ǁA�����A���A�ǁj
	if (floor->GetType() == eFloorGround)
	{
		if (width > m_EntranceWidth + wallWidth * 2.0f)
		{
			countWindow = (int)( ( width - m_EntranceWidth) / m_WindowWidth);
			wallWidth = ( width - countWindow * m_WindowWidth - m_EntranceWidth) * 0.5f;

			tile = new Tile();
			floor->SetTile( tile);
			tile->Init( height, wallWidth, eTileWall, this);

			tileNext = new Tile();
			tile->SetNext( tileNext);
			tileNext->Init( height, m_EntranceWidth, eTileEntrance, this);
			tile = tileNext;
			
			for (int i = 0; i < countWindow; i++)
			{
				tileNext = new Tile();
				tile->SetNext( tileNext);
				tileNext->Init( height, m_WindowWidth, eTileWindow, this);
				tile = tileNext;
			}
			
			tileNext = new Tile();
			tile->SetNext( tileNext);
			tileNext->Init( height, wallWidth, eTileWall, this);

			return true;
		}
	}

	//�]���̐ݒ�i���ׂĕǁj
	if (floor->GetType() == eFloorMargin)
	{
		tile = new Tile();
		floor->SetTile( tile);
		tile->Init( height, wallWidth, eTileWall, this);

		for (int i = 0; i < countWindow; i++)
		{
			tileNext = new Tile();
			tile->SetNext( tileNext);
			tileNext->Init( height, m_WindowWidth, eTileWall, this);
			tile = tileNext;
		}
			
		tileNext = new Tile();
		tile->SetNext( tileNext);
		tileNext->Init( height, wallWidth, eTileWall, this);

		return true;
	}

	//�f�t�H���g�̐ݒ�i���Ɨ��[�ɕǁj
	tile = new Tile();
	floor->SetTile( tile);
	tile->Init( height, wallWidth, eTileWall, this);

	for (int i = 0; i < countWindow; i++)
	{
		tileNext = new Tile();
		tile->SetNext( tileNext);
		tileNext->Init( height, m_WindowWidth, eTileWindow, this);
		tile = tileNext;
	}
			
	tileNext = new Tile();
	tile->SetNext( tileNext);
	tileNext->Init( height, wallWidth, eTileWall, this);

	return true;
}
