/*==============================================================================

    ShapeCylinder.cpp - 建物の自動生成ーボックス
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/9
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "ShapeCylinder.h"
#include "Wall.h"
#include "Roof.h"
#include "DebugLog.h"

#include "BuildingRule.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
ShapeCylinder::ShapeCylinder(GameObject* buildingObject)
{
	SetBuildingObject( buildingObject);
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
ShapeCylinder::~ShapeCylinder()
{
	Uninit();
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void ShapeCylinder::Init(const Vector3& position, float rotation, float height, float radius, BuildingRule* rule)
{
	SetPosition( position);
	SetRotation( rotation);
	m_Height = height;
	m_Radius = radius;
	m_Rule = rule;

	Vector3 size = Vector3( radius * 2.0f, height, radius * 2.0f);

	//屋根の生成
	auto roof = new Roof(GetBuildingObject());
	roof->InitPlane( position, rotation, size, "data/TEXTURE/circle.png");
	AddRoof( roof);

	//壁の生成
	CreateWalls();
}

/*------------------------------------------------------------------------------
	壁の生成
------------------------------------------------------------------------------*/
void ShapeCylinder::CreateWalls(void)
{
	//壁の消去
	if (GetWalls().size() != 0)
	{
		ClearWall();
	}

	auto matrix = GetMatrix();

	Vector3 bottomLeft = Vector3( 0.0f, 0.0f, m_Radius);
	auto wall = new Wall( GetBuildingObject());
	wall->InitCurve( matrix, m_Height, m_Radius * 2 * D3DX_PI, bottomLeft, m_Rule);
	AddWall( wall);
}

/*------------------------------------------------------------------------------
	移動
------------------------------------------------------------------------------*/
void ShapeCylinder::Move(const Vector3& value)
{
	UpdatePosition( value);

	//壁の更新
	for( auto wall : GetWalls())
	{
		wall->UpdateView( GetMatrix());
	}

	//屋根の更新
	for (auto roof : GetRoofs())
	{
		roof->UpdatePosition( GetPosition());
	}
}

/*------------------------------------------------------------------------------
	回転
------------------------------------------------------------------------------*/
void ShapeCylinder::Rotate( float value)
{
	UpdateRotation( value);

	//壁の更新
	for( auto wall : GetWalls())
	{
		wall->UpdateView( GetMatrix());
	}

	//屋根の更新
	for (auto roof : GetRoofs())
	{
		roof->UpdateRotation( GetRotation());
	}
}

/*------------------------------------------------------------------------------
	拡大縮小（変化量）
------------------------------------------------------------------------------*/
void ShapeCylinder::ScaleUpDown(const Vector3& value)
{
	m_Radius += value.x;
	m_Height += value.y;

	//壁の更新
	CreateWalls();

	//屋根の更新
	for (auto roof : GetRoofs())
	{
		roof->UpdateSize( Vector3(m_Radius * 2.0f, m_Height, m_Radius * 2.0f));
	}
}

/*------------------------------------------------------------------------------
	拡大縮小（比率・等倍）
------------------------------------------------------------------------------*/
void ShapeCylinder::ScaleRate( float rate)
{
	m_Radius *= rate;
	m_Height *= rate;

	//壁の更新
	CreateWalls();

	//屋根の更新
	for (auto roof : GetRoofs())
	{
		roof->UpdateSize( Vector3(m_Radius * 2.0f, m_Height, m_Radius * 2.0f));
	}
}

/*------------------------------------------------------------------------------
	拡大縮小（比率）
------------------------------------------------------------------------------*/
void ShapeCylinder::ScaleRate(const Vector3& rate)
{
	m_Radius *= rate.x;
	m_Height *= rate.y;

	//壁の更新
	CreateWalls();

	//屋根の更新
	for (auto roof : GetRoofs())
	{
		roof->UpdateSize( Vector3(m_Radius * 2.0f, m_Height, m_Radius * 2.0f));
	}
}

/*------------------------------------------------------------------------------
	形状を確定させる
------------------------------------------------------------------------------*/
void ShapeCylinder::ConfirmShape(void)
{
	
}


