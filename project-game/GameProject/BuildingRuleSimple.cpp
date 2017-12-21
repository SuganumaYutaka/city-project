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
BuildingRule* BuildingRuleSimple::Create( BuildingSurfacePattern* surfacePattern)
{
	auto random = new Random();
	
	//�`��̍���(10-30)
	random->SetRangeFloat( 10.0f, 30.0f);
	float shapeHeight = random->GetFloat();

	//1�K�̍���(2-4)
	random->SetRangeFloat( 2.0f, 4.0f);
	float groundFloorHeight = random->GetFloat();
	
	//�t���A�̍���(2-3)
	random->SetRangeFloat( 2.0f, 3.0f);
	float floorHeight = random->GetFloat();

	//���̕�(1-3)
	random->SetRangeFloat( 1.0f, 3.0f);
	float windowWidth = random->GetFloat();

	//���ւ̕�(2-6)
	random->SetRangeFloat( 2.0f, 6.0f);
	float entranceWidth = random->GetFloat();

	//���[���̐���
	auto rule = new BuildingRuleSimple( shapeHeight, groundFloorHeight, floorHeight, windowWidth, entranceWidth);
	rule->SetSurfacePattern( surfacePattern);
	random->SetDefault();
	rule->m_Random = random;
	
	return rule;
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
BuildingRuleSimple::~BuildingRuleSimple()
{
	delete m_Random;
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
	
	//��������葫��Ȃ��ꍇ�ǂ̂ݐݒ�
	if (width < m_WindowWidth)
	{
		tile = new Tile();
		floor->SetTile( tile);
		tile->Init( height, width, eTileWall, GetSurfacePattern()->GetWall());
		return true;
	}

	//���E�̐ݒ�
	if (floor->GetType() == eFloorBorder)
	{
		tile = new Tile();
		floor->SetTile( tile);
		tile->Init( height, width, eTileBorder, GetSurfacePattern()->GetBorder());
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
			tile->Init( height, wallWidth, eTileWall, GetSurfacePattern()->GetWall());

			tileNext = new Tile();
			tile->SetNext( tileNext);
			tileNext->Init( height, m_EntranceWidth, eTileEntrance, GetSurfacePattern()->GetEntrance());
			tile = tileNext;
			
			for (int i = 0; i < countWindow; i++)
			{
				tileNext = new Tile();
				tile->SetNext( tileNext);
				tileNext->Init( height, m_WindowWidth, eTileWindow, GetSurfacePattern()->GetWindow());
				tile = tileNext;
			}
			
			tileNext = new Tile();
			tile->SetNext( tileNext);
			tileNext->Init( height, wallWidth, eTileWall, GetSurfacePattern()->GetWall());

			return true;
		}
	}

	//�]���̐ݒ�i���ׂĕǁj
	if (floor->GetType() == eFloorMargin)
	{
		tile = new Tile();
		floor->SetTile( tile);
		tile->Init( height, wallWidth, eTileWall, GetSurfacePattern()->GetWall());

		for (int i = 0; i < countWindow; i++)
		{
			tileNext = new Tile();
			tile->SetNext( tileNext);
			tileNext->Init( height, m_WindowWidth, eTileWall, GetSurfacePattern()->GetWall());
			tile = tileNext;
		}
			
		tileNext = new Tile();
		tile->SetNext( tileNext);
		tileNext->Init( height, wallWidth, eTileWall, GetSurfacePattern()->GetWall());

		return true;
	}

	//�f�t�H���g�̐ݒ�i���Ɨ��[�ɕǁj
	tile = new Tile();
	floor->SetTile( tile);
	tile->Init( height, wallWidth, eTileWall, GetSurfacePattern()->GetWall());

	for (int i = 0; i < countWindow; i++)
	{
		tileNext = new Tile();
		tile->SetNext( tileNext);
		tileNext->Init( height, m_WindowWidth, eTileWindow, GetSurfacePattern()->GetWindow());
		tile = tileNext;
	}
			
	tileNext = new Tile();
	tile->SetNext( tileNext);
	tileNext->Init( height, wallWidth, eTileWall, GetSurfacePattern()->GetWall());

	return true;
}
