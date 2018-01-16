/*==============================================================================

    BuildingRuleMultiShape.h - �����̎��������[�����𐶐����郋�[���[����Shape����Ȃ錚��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/14
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BuildingRuleMultiShape.h"
#include "BuildingSurfacePattern.h"
#include "BuildingGeometry.h"
#include "Wall.h"
#include "Floor.h"
#include "Tile.h"
#include "Land.h"
#include "TileDefault.h"
#include "TileCurve.h"

#include "ShapeBox.h"
#include "ShapeCylinder.h"

/*------------------------------------------------------------------------------
	���[���̐���
------------------------------------------------------------------------------*/
BuildingRule* BuildingRuleMultiShape::Create( BuildingSurfacePattern* surfacePattern)
{
	//���[���̐���
	auto rule = new BuildingRuleMultiShape();
	
	//�����_���̐ݒ�
	auto random = new Random();
	rule->m_Random = random;

	//�`��̍���
	random->SetRangeFloat( 20.0f, 30.0f);
	rule->m_ShapeHeightMin = random->GetFloat();
	random->SetRangeFloat( 0.0f, 30.0f);
	rule->m_ShapeHeightMax = rule->m_ShapeHeightMin + random->GetFloat();

	//�`��̃T�C�Y�̔䗦
	rule->m_ShapeSizeRateMax = 0.9f;
	rule->m_ShapeSizeRateMin = 0.5f;

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
BuildingRuleMultiShape::~BuildingRuleMultiShape()
{
	delete m_Random;
}

/*------------------------------------------------------------------------------
	�`��̐���
------------------------------------------------------------------------------*/
bool BuildingRuleMultiShape::ProceduralShape(BuildingGeometry* geometry)
{
	//�y�n�̑傫��
	auto land = geometry->GetLand();
	auto vertices = land->GetVertices();
	Vector3 vec01 = vertices[1] - vertices[0];
	Vector3 vec03 = vertices[3] - vertices[0];
	Vector3 landSize;
	landSize.x = vec01.Length();
	landSize.z = vec03.Length();

	//Shape�̐�
	//m_Random->SetRangeInt( 1, 3);
	//int shapeCount = m_Random->GetInt();
	int shapeCount = 3;
	
	//Shape�̎��
	m_Random->SetRangeInt( 0, 2);
	int type = m_Random->GetInt();
	if( type < 2)
	{
		//ShapeBox�̐���
		CreateShapeBoxes( shapeCount, geometry, landSize);
	}
	else
	{
		//ShapeCylinder�̐���
		CreateShapeCylinders( shapeCount, geometry, landSize);
	}
	return true;
}

/*------------------------------------------------------------------------------
	�t���A�̐���
------------------------------------------------------------------------------*/
bool BuildingRuleMultiShape::ProceduralFloor( Wall* wall)
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
bool BuildingRuleMultiShape::ProceduralTile( Floor* floor)
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
bool BuildingRuleMultiShape::ProceduralFloorCurve( Wall* wall)
{
	//�e�N�X�`���̐ݒ�
	wall->LoadTexture( GetSurfacePattern()->GetTextureFileName());

	float height = wall->GetHeight();
	float width = wall->GetWidth();
	Vector3 bottomLeft = wall->GetBottomLeftPosition();
	Floor* floor = NULL;

	//1�K
	floor = new Floor();
	floor->InitCurve( m_GroundFloorHeight, width, bottomLeft, eFloorGround, this);
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
		floor->InitCurve( m_FloorHeight, width, bottomLeft, eFloorDefault, this);
		wall->AddFloor( floor);
		height -= m_FloorHeight;
		bottomLeft.y += m_FloorHeight;
	}

	//�㕔�ɗ]��
	floor = new Floor();
	floor->InitCurve( height, width, bottomLeft, eFloorMargin, this);
	wall->AddFloor( floor);

	return true;
}

/*------------------------------------------------------------------------------
	�^�C���̐����i�~�ɉ����ċȂ���j
------------------------------------------------------------------------------*/
bool BuildingRuleMultiShape::ProceduralTileCurve( Floor* floor)
{
	Vector3 bottomLeftPosition = floor->GetBottomLeftPosition();
	float height = floor->GetHeight();
	float width = floor->GetWidth();
	TileCurve* tile = NULL;
	TileCurve* tileNext = NULL;
	
	//��������葫��Ȃ��ꍇ�ǂ̂ݐݒ�
	if (width < m_WindowWidth)
	{
		tile = new TileCurve();
		floor->SetTile( tile);
		tile->Init( height, width, bottomLeftPosition, eTileWall, GetSurfacePattern()->GetWall());
		return true;
	}

	//���E�̐ݒ�
	if (floor->GetType() == eFloorBorder)
	{
		tile = new TileCurve();
		floor->SetTile( tile);
		tile->Init( height, width, bottomLeftPosition, eTileBorder, GetSurfacePattern()->GetBorder());
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

			tile = new TileCurve();
			floor->SetTile( tile);
			tile->Init( height, wallWidth, bottomLeftPosition, eTileWall, GetSurfacePattern()->GetWall());
			bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tile->CulcAngle());

			tileNext = new TileCurve();
			tile->SetNext( tileNext);
			tileNext->Init( height, m_EntranceWidth, bottomLeftPosition, eTileEntrance, GetSurfacePattern()->GetEntrance());
			bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tileNext->CulcAngle());
			tile = tileNext;
			
			for (int i = 0; i < countWindow; i++)
			{
				tileNext = new TileCurve();
				tile->SetNext( tileNext);
				tileNext->Init( height, m_WindowWidth, bottomLeftPosition, eTileWindow, GetSurfacePattern()->GetWindow());
				bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tileNext->CulcAngle());
				tile = tileNext;
			}
			
			tileNext = new TileCurve();
			tile->SetNext( tileNext);
			tileNext->Init( height, wallWidth, bottomLeftPosition, eTileWall, GetSurfacePattern()->GetWall());
			bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tileNext->CulcAngle());

			return true;
		}
	}

	//�]���̐ݒ�i���ׂĕǁj
	if (floor->GetType() == eFloorMargin)
	{
		tile = new TileCurve();
		floor->SetTile( tile);
		tile->Init( height, wallWidth, bottomLeftPosition, eTileWall, GetSurfacePattern()->GetWall());
		bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tile->CulcAngle());

		for (int i = 0; i < countWindow; i++)
		{
			tileNext = new TileCurve();
			tile->SetNext( tileNext);
			tileNext->Init( height, m_WindowWidth, bottomLeftPosition, eTileWall, GetSurfacePattern()->GetWall());
			bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tileNext->CulcAngle());
			tile = tileNext;
		}
			
		tileNext = new TileCurve();
		tile->SetNext( tileNext);
		tileNext->Init( height, wallWidth, bottomLeftPosition, eTileWall, GetSurfacePattern()->GetWall());
		bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tileNext->CulcAngle());
		return true;
	}

	//�f�t�H���g�̐ݒ�i���ׂđ��j
	float windowWidth = width / (float)countWindow;

	tile = new TileCurve();
	floor->SetTile( tile);
	tile->Init( height, windowWidth, bottomLeftPosition, eTileWindow, GetSurfacePattern()->GetWindow());
	bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tile->CulcAngle());

	for (int i = 0; i < countWindow - 1; i++)
	{
		tileNext = new TileCurve();
		tile->SetNext( tileNext);
		tileNext->Init( height, windowWidth, bottomLeftPosition, eTileWindow, GetSurfacePattern()->GetWindow());
		bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tileNext->CulcAngle());
		tile = tileNext;
	}

	return true;
}

/*------------------------------------------------------------------------------
	�~�ɉ����č������W���ړ�����
------------------------------------------------------------------------------*/
Vector3 BuildingRuleMultiShape::MoveBottomLeftPosition( const Vector3& bottomLeftPosition, float angle)
{
	D3DXMATRIX mtxRotation;
	D3DXMatrixIdentity( &mtxRotation);
	D3DXMatrixRotationY( &mtxRotation, angle);

	D3DXVECTOR3 position = bottomLeftPosition.ConvertToDX();
	D3DXVec3TransformCoord( &position, &position, &mtxRotation);
	
	return Vector3::ConvertFromDX( position);
}

/*------------------------------------------------------------------------------
	������Shape�𐶐��i�l�p���j
------------------------------------------------------------------------------*/
void BuildingRuleMultiShape::CreateShapeBoxes(int shapeCount, BuildingGeometry* geometry, const Vector3& landSize)
{
	for( int i = 0; i < shapeCount; i++)
	{
		//���łɐ�������Ă���Shape�����΂�
		for ( auto shape : geometry->GetShape())
		{
			shape->ScaleRate( Vector3( 1.0f, 1.5f, 1.0f));
		}

		//�����̃V�[�h�l���Đݒ�
		m_Random->ResetSeed();

		Vector3 size;

		//�傫���̐ݒ�
		m_Random->SetRangeFloat( m_ShapeSizeRateMin, m_ShapeSizeRateMax);
		size.x = landSize.x * m_Random->GetFloat();
		size.z = landSize.z * m_Random->GetFloat();
	
		//�ʒu�̐ݒ�
		Vector3 position;
		position.y = 0.0f;
		float range = ( landSize.x - size.x) * 0.5f;
		m_Random->SetRangeFloat( -range, range);
		position.x = m_Random->GetFloat();
		range = ( landSize.z - size.z) * 0.5f;
		m_Random->SetRangeFloat( -range, range);
		position.z = m_Random->GetFloat();
		//position.z = ( landSize.z - size.z) * 0.5f;

		//�����̐ݒ�
		m_Random->SetRangeFloat( m_ShapeHeightMin, m_ShapeHeightMax);
		size.y = m_Random->GetFloat();

		//�`��̐���
		auto shape = new ShapeBox( geometry->GetGameObject());
		shape->Init( position, 0.0f, size, this);
		geometry->AddShape( shape);
	}
}
	
/*------------------------------------------------------------------------------
	������Shape�𐶐��i�~���j
------------------------------------------------------------------------------*/
void BuildingRuleMultiShape::CreateShapeCylinders(int shapeCount, BuildingGeometry* geometry, const Vector3& landSize)
{
	for( int i = 0; i < shapeCount; i++)
	{
		//���łɐ�������Ă���Shape�����΂�
		for ( auto shape : geometry->GetShape())
		{
			shape->ScaleRate( Vector3( 1.0f, 1.5f, 1.0f));
		}

		//�����̃V�[�h�l���Đݒ�
		m_Random->ResetSeed();

		Vector3 size;

		//�傫���̐ݒ�
		m_Random->SetRangeFloat( m_ShapeSizeRateMin, m_ShapeSizeRateMax);
		size.x = landSize.x * m_Random->GetFloat();
		size.z = size.x;
	
		//�ʒu�̐ݒ�
		Vector3 position;
		position.y = 0.0f;
		/*float range = ( landSize.x - size.x) * 0.5f;
		m_Random->SetRangeFloat( -range, range);
		position.x = m_Random->GetFloat();
		range = ( landSize.z - size.z) * 0.5f;
		m_Random->SetRangeFloat( -range, range);
		position.z = m_Random->GetFloat();*/
		
		//�����̐ݒ�
		m_Random->SetRangeFloat( m_ShapeHeightMin, m_ShapeHeightMax);
		size.y = m_Random->GetFloat();

		//�`��̐���
		auto shape = new ShapeCylinder( geometry->GetGameObject());
		shape->Init( position, 0.0f, size.y, size.x * 0.5f, this);
		geometry->AddShape( shape);
	}
}

