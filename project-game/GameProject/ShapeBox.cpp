/*==============================================================================

    ShapeBox.cpp - 建物の自動生成ーボックス
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/8
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "ShapeBox.h"
#include "Wall.h"
#include "Roof.h"
#include "DebugLog.h"

#include "BuildingRule.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
ShapeBox::ShapeBox(GameObject* buildingObject)
{
	SetBuildingObject( buildingObject);
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
ShapeBox::~ShapeBox()
{
	Uninit();
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void ShapeBox::Init(const Vector3& position, float rotation, const Vector3& size, BuildingRule* rule)
{
	SetPosition( position);
	SetRotation( rotation);
	m_Size = size;
	m_Rule = rule;

	//屋根の生成
	auto roof = new Roof(GetBuildingObject());
	roof->Init( position, rotation, size);
	AddRoof( roof);

	//壁の生成
	CreateWalls();
}

void ShapeBox::CreateWalls(void)
{
	//壁の消去
	if (GetWalls().size() != 0)
	{
		ClearWall();
	}

	auto matrix = GetMatrix();

	//手前
	Vector3 leftFront( m_Size.x * -0.5f, 0.0f, m_Size.z * 0.5f);
	auto frontWall = new Wall( GetBuildingObject());
	frontWall->InitDefault( matrix, m_Size.y, m_Size.x, leftFront, Vector3(0.0f, 0.0f, -1.0f), m_Rule);
	AddWall( frontWall);

	//右
	Vector3 rightFront( m_Size.x * -0.5f, 0.0f, m_Size.z * -0.5f);
	auto rightWall = new Wall( GetBuildingObject());
	rightWall->InitDefault( matrix, m_Size.y, m_Size.z, rightFront, Vector3(1.0f, 0.0f, 0.0f), m_Rule);
	AddWall( rightWall);

	//奥
	Vector3 rightBack( m_Size.x * +0.5f, 0.0f, m_Size.z * -0.5f);
	auto backWall = new Wall( GetBuildingObject());
	backWall->InitDefault( matrix, m_Size.y, m_Size.x, rightBack, Vector3(0.0f, 0.0f, 1.0f), m_Rule);
	AddWall( backWall);

	//左
	Vector3 leftBack( m_Size.x * 0.5f, 0.0f, m_Size.z * +0.5f);
	auto leftWall = new Wall( GetBuildingObject());
	leftWall->InitDefault( matrix, m_Size.y, m_Size.z, leftBack, Vector3(-1.0f, 0.0f, 0.0f), m_Rule);
	AddWall( leftWall);
}

/*------------------------------------------------------------------------------
	移動
------------------------------------------------------------------------------*/
void ShapeBox::Move(const Vector3& value)
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
void ShapeBox::Rotate( float value)
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
void ShapeBox::ScaleUpDown(const Vector3& value)
{
	m_Size += value;

	//壁の更新
	CreateWalls();

	//屋根の更新
	for (auto roof : GetRoofs())
	{
		roof->UpdateSize( m_Size);
	}
}

/*------------------------------------------------------------------------------
	拡大縮小（比率・等倍）
------------------------------------------------------------------------------*/
void ShapeBox::ScaleRate( float rate)
{
	m_Size *= rate;

	//壁の更新
	CreateWalls();

	//屋根の更新
	for (auto roof : GetRoofs())
	{
		roof->UpdateSize( m_Size);
	}
}

/*------------------------------------------------------------------------------
	拡大縮小（比率）
------------------------------------------------------------------------------*/
void ShapeBox::ScaleRate(const Vector3& rate)
{
	m_Size.x *= rate.x;
	m_Size.y *= rate.y;
	m_Size.z *= rate.z;

	//壁の更新
	CreateWalls();

	//屋根の更新
	for (auto roof : GetRoofs())
	{
		roof->UpdateSize( m_Size);
	}
}

/*------------------------------------------------------------------------------
	形状を確定させる
------------------------------------------------------------------------------*/
void ShapeBox::ConfirmShape(void)
{
	//4つの壁を融合する
	auto walls = GetWalls();
	auto wall = walls.front();
	for (auto other : walls)
	{
		if (wall == other)
		{
			continue;
		}
		wall->FusionSameShape( other);
		SubWall( other);
	}

	//描画情報の更新
	wall->UpdateView( GetMatrix());
}


