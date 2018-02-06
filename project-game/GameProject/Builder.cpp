/*==============================================================================

    Builder.cpp - 建物の自動生成ー建物組み立て
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "BuildingParameter.h"
#include "Builder.h"
#include "BuildingGeometry.h"
#include "ShapeBox.h"
#include "ShapeCylinder.h"
#include "Wall.h"
#include "Floor.h"
#include "TileDefault.h"
#include "TileCurve.h"
#include "BuildingSurfacePattern.h"

/*------------------------------------------------------------------------------
	建物組み立て
------------------------------------------------------------------------------*/
bool Builder::operator() (BuildingGeometry* geometry, GeometryParameter* parameter)
{
	m_Geometry = geometry;
	m_Parameter = parameter;

	//形状の生成
	auto shapeParameters = parameter->m_ShapeParameters;
	for (auto shapeParameter : shapeParameters)
	{
		switch (shapeParameter->m_Type)
		{
		case eShapeBox:
			{
				//形状の生成
				auto shape = CreateShapeBox( shapeParameter);

				//フロアの生成
				for (auto wall : shape->GetWalls())
				{
					CreateFloor( wall);

					//タイルの生成
					for (auto floor : wall->GetFloors())
					{
						CreateTile( floor);
					}

					//壁描画の更新
					wall->UpdateView( shape->GetMatrix());
				}
				break;
			}

		case eShapeCylinder:
			{
				//形状の生成
				auto shape = CreateShapeCylinder( shapeParameter);

				//フロアの生成
				for (auto wall : shape->GetWalls())
				{
					CreateFloorCurve( wall);

					//タイルの生成
					for (auto floor : wall->GetFloors())
					{
						CreateTileCurve( floor);
					}

					//壁描画の更新
					wall->UpdateView( shape->GetMatrix());
				}
				break;
			}
		}
	}

	return true;
}

/*------------------------------------------------------------------------------
	形状（四角柱）の生成
------------------------------------------------------------------------------*/
ShapeBox* Builder::CreateShapeBox( ShapeParameter* parameter)
{
	//形状の生成
	auto shape = new ShapeBox( m_Geometry->m_pGameObject);
	shape->Init( parameter->m_Position, parameter->m_Rotation, parameter->m_Size);
	m_Geometry->AddShape( shape);

	return shape;
}

/*------------------------------------------------------------------------------
	形状（円柱）の生成
------------------------------------------------------------------------------*/
ShapeCylinder* Builder::CreateShapeCylinder( ShapeParameter* parameter)
{
	//形状の生成
	auto shape = new ShapeCylinder( m_Geometry->m_pGameObject);
	shape->Init( parameter->m_Position, parameter->m_Rotation, parameter->m_Size.y, parameter->m_Size.x);
	m_Geometry->AddShape( shape);

	return shape;
}

/*------------------------------------------------------------------------------
	フロアの生成
------------------------------------------------------------------------------*/
bool Builder::CreateFloor( Wall* wall)
{
	//テクスチャの設定
	wall->LoadTexture( m_Parameter->m_SurfacePattern->GetTextureFileName());

	float height = wall->GetHeight();
	float width = wall->GetWidth();
	Vector3 bottomLeft = wall->GetBottomLeftPosition();
	auto normal = wall->GetNormal();
	Floor* floor = NULL;

	//1階
	floor = new Floor();
	floor->InitDefault( m_Parameter->m_GroundFloorHeight, width, bottomLeft, normal, eFloorGround);
	wall->AddFloor( floor);
	height -= m_Parameter->m_GroundFloorHeight;
	bottomLeft.y += m_Parameter->m_GroundFloorHeight;

	//それ以外のフロア
	for (;;)
	{
		if (height < m_Parameter->m_FloorHeight)
		{
			break;
		}
		floor = new Floor();
		floor->InitDefault( m_Parameter->m_FloorHeight, width, bottomLeft, normal, eFloorDefault);
		wall->AddFloor( floor);
		height -= m_Parameter->m_FloorHeight;
		bottomLeft.y += m_Parameter->m_FloorHeight;
	}

	//上部に余白
	floor = new Floor();
	floor->InitDefault( height, width, bottomLeft, normal, eFloorMargin);
	wall->AddFloor( floor);

	return true;
}

/*------------------------------------------------------------------------------
	フロアの生成（円に沿って曲がる）
------------------------------------------------------------------------------*/
bool Builder::CreateFloorCurve( Wall* wall)
{
	//テクスチャの設定
	wall->LoadTexture( m_Parameter->m_SurfacePattern->GetTextureFileName());

	float height = wall->GetHeight();
	float width = wall->GetWidth();
	Vector3 bottomLeft = wall->GetBottomLeftPosition();
	Floor* floor = NULL;

	//1階
	floor = new Floor();
	floor->InitCurve( m_Parameter->m_GroundFloorHeight, width, bottomLeft, eFloorGround);
	wall->AddFloor( floor);
	height -= m_Parameter->m_GroundFloorHeight;
	bottomLeft.y += m_Parameter->m_GroundFloorHeight;

	//それ以外のフロア
	for (;;)
	{
		if (height < m_Parameter->m_FloorHeight)
		{
			break;
		}
		floor = new Floor();
		floor->InitCurve( m_Parameter->m_FloorHeight, width, bottomLeft, eFloorDefault);
		wall->AddFloor( floor);
		height -= m_Parameter->m_FloorHeight;
		bottomLeft.y += m_Parameter->m_FloorHeight;
	}

	//上部に余白
	floor = new Floor();
	floor->InitCurve( height, width, bottomLeft, eFloorMargin);
	wall->AddFloor( floor);

	return true;
}

/*------------------------------------------------------------------------------
	タイルの生成
------------------------------------------------------------------------------*/
bool Builder::CreateTile( Floor* floor)
{
	Vector3 bottomLeftPosition = floor->GetBottomLeftPosition();
	auto normal = floor->GetNormal();
	auto vector = Vector3::Cross( normal, Vector3( 0.0f, 1.0f, 0.0f)).Normalize();
	float height = floor->GetHeight();
	float width = floor->GetWidth();
	TileDefault* tile = NULL;
	TileDefault* tileNext = NULL;
	
	//幅が窓より足りない場合壁のみ設定
	if (width < m_Parameter->m_WindowWidth)
	{
		tile = new TileDefault();
		floor->SetTile( tile);
		tile->Init( height, width, bottomLeftPosition, normal, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
		return true;
	}

	//境界の設定
	if (floor->GetType() == eFloorBorder)
	{
		tile = new TileDefault();
		floor->SetTile( tile);
		tile->Init( height, width, bottomLeftPosition, normal, eTileBorder, m_Parameter->m_SurfacePattern->GetBorder());
		return true;
	}

	int countWindow = (int)( width / m_Parameter->m_WindowWidth);
	float wallWidth = ( width - countWindow * m_Parameter->m_WindowWidth) * 0.5f;

	//1階の設定（壁、入口、窓、壁）
	if (floor->GetType() == eFloorGround)
	{
		if (width > m_Parameter->m_EntranceWidth + wallWidth * 2.0f)
		{
			countWindow = (int)( ( width - m_Parameter->m_EntranceWidth) / m_Parameter->m_WindowWidth);
			wallWidth = ( width - countWindow * m_Parameter->m_WindowWidth - m_Parameter->m_EntranceWidth) * 0.5f;

			tile = new TileDefault();
			floor->SetTile( tile);
			tile->Init( height, wallWidth, bottomLeftPosition, normal, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
			bottomLeftPosition += vector * wallWidth;

			tileNext = new TileDefault();
			tile->SetNext( tileNext);
			tileNext->Init( height, m_Parameter->m_EntranceWidth, bottomLeftPosition, normal, eTileEntrance, m_Parameter->m_SurfacePattern->GetEntrance());
			bottomLeftPosition += vector * m_Parameter->m_EntranceWidth;
			tile = tileNext;
			
			for (int i = 0; i < countWindow; i++)
			{
				tileNext = new TileDefault();
				tile->SetNext( tileNext);
				tileNext->Init( height, m_Parameter->m_WindowWidth, bottomLeftPosition, normal, eTileWindow, m_Parameter->m_SurfacePattern->GetWindow());
				bottomLeftPosition += vector * m_Parameter->m_WindowWidth;
				tile = tileNext;
			}
			
			tileNext = new TileDefault();
			tile->SetNext( tileNext);
			tileNext->Init( height, wallWidth, bottomLeftPosition, normal, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
			bottomLeftPosition += vector * wallWidth;

			return true;
		}
	}

	//余白の設定（すべて壁）
	if (floor->GetType() == eFloorMargin)
	{
		tile = new TileDefault();
		floor->SetTile( tile);
		tile->Init( height, wallWidth, bottomLeftPosition, normal, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
		bottomLeftPosition += vector * wallWidth;

		for (int i = 0; i < countWindow; i++)
		{
			tileNext = new TileDefault();
			tile->SetNext( tileNext);
			tileNext->Init( height, m_Parameter->m_WindowWidth, bottomLeftPosition, normal, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
			bottomLeftPosition += vector * m_Parameter->m_WindowWidth;
			tile = tileNext;
		}
			
		tileNext = new TileDefault();
		tile->SetNext( tileNext);
		tileNext->Init( height, wallWidth, bottomLeftPosition, normal, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
		bottomLeftPosition += vector * wallWidth;
		return true;
	}

	//デフォルトの設定（窓と両端に壁）
	tile = new TileDefault();
	floor->SetTile( tile);
	tile->Init( height, wallWidth, bottomLeftPosition, normal, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
	bottomLeftPosition += vector * wallWidth;

	for (int i = 0; i < countWindow; i++)
	{
		tileNext = new TileDefault();
		tile->SetNext( tileNext);
		tileNext->Init( height, m_Parameter->m_WindowWidth, bottomLeftPosition, normal, eTileWindow, m_Parameter->m_SurfacePattern->GetWindow());
		bottomLeftPosition += vector * m_Parameter->m_WindowWidth;
		tile = tileNext;
	}
			
	tileNext = new TileDefault();
	tile->SetNext( tileNext);
	tileNext->Init( height, wallWidth, bottomLeftPosition, normal, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
	bottomLeftPosition += vector * wallWidth;

	return true;
}

/*------------------------------------------------------------------------------
	タイルの生成（円に沿って曲がる）
------------------------------------------------------------------------------*/
bool Builder::CreateTileCurve( Floor* floor)
{
	Vector3 bottomLeftPosition = floor->GetBottomLeftPosition();
	float height = floor->GetHeight();
	float width = floor->GetWidth();
	TileCurve* tile = NULL;
	TileCurve* tileNext = NULL;
	
	//幅が窓より足りない場合壁のみ設定
	if (width < m_Parameter->m_WindowWidth)
	{
		tile = new TileCurve();
		floor->SetTile( tile);
		tile->Init( height, width, bottomLeftPosition, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
		return true;
	}

	//境界の設定
	if (floor->GetType() == eFloorBorder)
	{
		tile = new TileCurve();
		floor->SetTile( tile);
		tile->Init( height, width, bottomLeftPosition, eTileBorder, m_Parameter->m_SurfacePattern->GetBorder());
		return true;
	}

	int countWindow = (int)( width / m_Parameter->m_WindowWidth);
	float wallWidth = ( width - countWindow * m_Parameter->m_WindowWidth) * 0.5f;

	//1階の設定（壁、入口、窓、壁）
	if (floor->GetType() == eFloorGround)
	{
		if (width > m_Parameter->m_EntranceWidth + wallWidth * 2.0f)
		{
			countWindow = (int)( ( width - m_Parameter->m_EntranceWidth) / m_Parameter->m_WindowWidth);
			wallWidth = ( width - countWindow * m_Parameter->m_WindowWidth - m_Parameter->m_EntranceWidth) * 0.5f;

			tile = new TileCurve();
			floor->SetTile( tile);
			tile->Init( height, wallWidth, bottomLeftPosition, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
			bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tile->CulcAngle());

			tileNext = new TileCurve();
			tile->SetNext( tileNext);
			tileNext->Init( height, m_Parameter->m_EntranceWidth, bottomLeftPosition, eTileEntrance, m_Parameter->m_SurfacePattern->GetEntrance());
			bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tileNext->CulcAngle());
			tile = tileNext;
			
			for (int i = 0; i < countWindow; i++)
			{
				tileNext = new TileCurve();
				tile->SetNext( tileNext);
				tileNext->Init( height, m_Parameter->m_WindowWidth, bottomLeftPosition, eTileWindow, m_Parameter->m_SurfacePattern->GetWindow());
				bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tileNext->CulcAngle());
				tile = tileNext;
			}
			
			tileNext = new TileCurve();
			tile->SetNext( tileNext);
			tileNext->Init( height, wallWidth, bottomLeftPosition, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
			bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tileNext->CulcAngle());

			return true;
		}
	}

	//余白の設定（すべて壁）
	if (floor->GetType() == eFloorMargin)
	{
		tile = new TileCurve();
		floor->SetTile( tile);
		tile->Init( height, wallWidth, bottomLeftPosition, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
		bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tile->CulcAngle());

		for (int i = 0; i < countWindow; i++)
		{
			tileNext = new TileCurve();
			tile->SetNext( tileNext);
			tileNext->Init( height, m_Parameter->m_WindowWidth, bottomLeftPosition, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
			bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tileNext->CulcAngle());
			tile = tileNext;
		}
			
		tileNext = new TileCurve();
		tile->SetNext( tileNext);
		tileNext->Init( height, wallWidth, bottomLeftPosition, eTileWall, m_Parameter->m_SurfacePattern->GetWall());
		bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tileNext->CulcAngle());
		return true;
	}

	//デフォルトの設定（すべて窓）
	float windowWidth = width / (float)countWindow;

	tile = new TileCurve();
	floor->SetTile( tile);
	tile->Init( height, windowWidth, bottomLeftPosition, eTileWindow, m_Parameter->m_SurfacePattern->GetWindow());
	bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tile->CulcAngle());

	for (int i = 0; i < countWindow - 1; i++)
	{
		tileNext = new TileCurve();
		tile->SetNext( tileNext);
		tileNext->Init( height, windowWidth, bottomLeftPosition, eTileWindow, m_Parameter->m_SurfacePattern->GetWindow());
		bottomLeftPosition = MoveBottomLeftPosition( bottomLeftPosition, tileNext->CulcAngle());
		tile = tileNext;
	}

	return true;
}

/*------------------------------------------------------------------------------
	円に沿って左下座標を移動する
------------------------------------------------------------------------------*/
Vector3 Builder::MoveBottomLeftPosition( const Vector3& bottomLeftPosition, float angle)
{
	D3DXMATRIX mtxRotation;
	D3DXMatrixIdentity( &mtxRotation);
	D3DXMatrixRotationY( &mtxRotation, -angle);

	D3DXVECTOR3 position = bottomLeftPosition.ConvertToDX();
	D3DXVec3TransformCoord( &position, &position, &mtxRotation);
	
	return Vector3::ConvertFromDX( position);
}



