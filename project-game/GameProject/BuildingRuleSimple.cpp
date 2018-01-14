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
#include "TileDefault.h"
#include "TileCurve.h"

#include "ShapeBox.h"

/*------------------------------------------------------------------------------
	���[���̐���
------------------------------------------------------------------------------*/
BuildingRule* BuildingRuleSimple::Create( BuildingSurfacePattern* surfacePattern)
{
	//���[���̐���
	auto rule = new BuildingRuleSimple();

	//�����_���̐ݒ�
	auto random = new Random();
	rule->m_Random = random;
	
	//�`��̍���
	random->SetRangeFloat( 10.0f, 50.0f);
	rule->m_ShapeHeight = random->GetFloat();

	//1�K�̍���
	random->SetRangeFloat( 2.0f, 2.5f);
	rule->m_GroundFloorHeight = random->GetFloat();
	
	//�t���A�̍���
	random->SetRangeFloat( 1.0f, 2.0f);
	rule->m_FloorHeight = random->GetFloat();

	//���̕�
	random->SetRangeFloat( 0.8f, 1.8f);
	rule->m_WindowWidth = random->GetFloat();

	//���ւ̕�
	random->SetRangeFloat( 2.0f, 2.5f);
	rule->m_EntranceWidth = random->GetFloat();

	//�\�ʃp�^�[��
	rule->SetSurfacePattern( surfacePattern);
	
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
	
	//ShapeBox�̐���
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
	auto normal = wall->GetNormal();
	Floor* floor = NULL;

	//1�K
	floor = new Floor();
	floor->InitDefault( m_GroundFloorHeight, width, bottomLeft, normal, eFloorGround, this);
	wall->AddFloor( floor);
	height -= m_GroundFloorHeight;
	bottomLeft.y += m_GroundFloorHeight;

	//����ȊO�̃t���A
	for (;;)
	{
		if (height < m_FloorHeight)
		{
			break;
		}
		floor = new Floor();
		floor->InitDefault( m_FloorHeight, width, bottomLeft, normal, eFloorDefault, this);
		wall->AddFloor( floor);
		height -= m_FloorHeight;
		bottomLeft.y += m_FloorHeight;
	}

	//�㕔�ɗ]��
	floor = new Floor();
	floor->InitDefault( height, width, bottomLeft, normal, eFloorMargin, this);
	wall->AddFloor( floor);

	return true;
}

/*------------------------------------------------------------------------------
	�^�C���̐���
------------------------------------------------------------------------------*/
bool BuildingRuleSimple::ProceduralTile( Floor* floor)
{
	Vector3 bottomLeftPosition = floor->GetBottomLeftPosition();
	auto normal = floor->GetNormal();
	auto vector = Vector3::Cross( normal, Vector3( 0.0f, 1.0f, 0.0f)).Normalize();
	float height = floor->GetHeight();
	float width = floor->GetWidth();
	TileDefault* tile = NULL;
	TileDefault* tileNext = NULL;
	
	//��������葫��Ȃ��ꍇ�ǂ̂ݐݒ�
	if (width < m_WindowWidth)
	{
		tile = new TileDefault();
		floor->SetTile( tile);
		tile->Init( height, width, bottomLeftPosition, normal, eTileWall, GetSurfacePattern()->GetWall());
		return true;
	}

	//���E�̐ݒ�
	if (floor->GetType() == eFloorBorder)
	{
		tile = new TileDefault();
		floor->SetTile( tile);
		tile->Init( height, width, bottomLeftPosition, normal, eTileBorder, GetSurfacePattern()->GetBorder());
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

			tile = new TileDefault();
			floor->SetTile( tile);
			tile->Init( height, wallWidth, bottomLeftPosition, normal, eTileWall, GetSurfacePattern()->GetWall());
			bottomLeftPosition += vector * wallWidth;

			tileNext = new TileDefault();
			tile->SetNext( tileNext);
			tileNext->Init( height, m_EntranceWidth, bottomLeftPosition, normal, eTileEntrance, GetSurfacePattern()->GetEntrance());
			bottomLeftPosition += vector * m_EntranceWidth;
			tile = tileNext;
			
			for (int i = 0; i < countWindow; i++)
			{
				tileNext = new TileDefault();
				tile->SetNext( tileNext);
				tileNext->Init( height, m_WindowWidth, bottomLeftPosition, normal, eTileWindow, GetSurfacePattern()->GetWindow());
				bottomLeftPosition += vector * m_WindowWidth;
				tile = tileNext;
			}
			
			tileNext = new TileDefault();
			tile->SetNext( tileNext);
			tileNext->Init( height, wallWidth, bottomLeftPosition, normal, eTileWall, GetSurfacePattern()->GetWall());
			bottomLeftPosition += vector * wallWidth;

			return true;
		}
	}

	//�]���̐ݒ�i���ׂĕǁj
	if (floor->GetType() == eFloorMargin)
	{
		tile = new TileDefault();
		floor->SetTile( tile);
		tile->Init( height, wallWidth, bottomLeftPosition, normal, eTileWall, GetSurfacePattern()->GetWall());
		bottomLeftPosition += vector * wallWidth;

		for (int i = 0; i < countWindow; i++)
		{
			tileNext = new TileDefault();
			tile->SetNext( tileNext);
			tileNext->Init( height, m_WindowWidth, bottomLeftPosition, normal, eTileWall, GetSurfacePattern()->GetWall());
			bottomLeftPosition += vector * m_WindowWidth;
			tile = tileNext;
		}
			
		tileNext = new TileDefault();
		tile->SetNext( tileNext);
		tileNext->Init( height, wallWidth, bottomLeftPosition, normal, eTileWall, GetSurfacePattern()->GetWall());
		bottomLeftPosition += vector * wallWidth;
		return true;
	}

	//�f�t�H���g�̐ݒ�i���Ɨ��[�ɕǁj
	tile = new TileDefault();
	floor->SetTile( tile);
	tile->Init( height, wallWidth, bottomLeftPosition, normal, eTileWall, GetSurfacePattern()->GetWall());
	bottomLeftPosition += vector * wallWidth;

	for (int i = 0; i < countWindow; i++)
	{
		tileNext = new TileDefault();
		tile->SetNext( tileNext);
		tileNext->Init( height, m_WindowWidth, bottomLeftPosition, normal, eTileWindow, GetSurfacePattern()->GetWindow());
		bottomLeftPosition += vector * m_WindowWidth;
		tile = tileNext;
	}
			
	tileNext = new TileDefault();
	tile->SetNext( tileNext);
	tileNext->Init( height, wallWidth, bottomLeftPosition, normal, eTileWall, GetSurfacePattern()->GetWall());
	bottomLeftPosition += vector * wallWidth;

	return true;
}

/*------------------------------------------------------------------------------
	�t���A�̐����i�~�ɉ����ċȂ���j
------------------------------------------------------------------------------*/
bool BuildingRuleSimple::ProceduralFloorCurve( Wall* wall)
{
	//TODO:

	return true;
}

/*------------------------------------------------------------------------------
	�^�C���̐����i�~�ɉ����ċȂ���j
------------------------------------------------------------------------------*/
bool BuildingRuleSimple::ProceduralTileCurve( Floor* floor)
{
	//TODO:

	return true;
}

