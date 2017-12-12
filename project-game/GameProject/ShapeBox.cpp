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
	m_Position = position;
	m_Rotation = rotation;
	m_Size = size;

	//屋根の生成
	auto roof = new Roof(GetBuildingObject());
	roof->Init( position, rotation, size);
	AddRoof( roof);

	//壁の生成
	//右
	Vector3 rightFront( size.x * -0.5f, 0.0f, size.z * -0.5f);
	auto rightWall = new Wall( GetBuildingObject());
	rightWall->Init( size.y, size.z, rightFront, Vector3(1.0f, 0.0f, 0.0f), rule);
	AddWall( rightWall);

	//左
	Vector3 leftBack( size.x * 0.5f, 0.0f, size.z * +0.5f);
	auto leftWall = new Wall( GetBuildingObject());
	leftWall->Init( size.y, size.z, leftBack, Vector3(-1.0f, 0.0f, 0.0f), rule);
	AddWall( leftWall);

	//奥
	Vector3 rightBack( size.x * +0.5f, 0.0f, size.z * -0.5f);
	auto backWall = new Wall( GetBuildingObject());
	backWall->Init( size.y, size.x, rightBack, Vector3(0.0f, 0.0f, 1.0f), rule);
	AddWall( backWall);

	//手前
	Vector3 leftFront( size.x * -0.5f, 0.0f, size.z * 0.5f);
	auto frontWall = new Wall( GetBuildingObject());
	frontWall->Init( size.y, size.x, leftFront, Vector3(0.0f, 0.0f, -1.0f), rule);
	AddWall( frontWall);
}

/*------------------------------------------------------------------------------
	移動
------------------------------------------------------------------------------*/
void ShapeBox::Move(const Vector3& value)
{

}

/*------------------------------------------------------------------------------
	回転
------------------------------------------------------------------------------*/
void ShapeBox::Rotate( float value)
{

}

/*------------------------------------------------------------------------------
	拡大縮小
------------------------------------------------------------------------------*/
void ShapeBox::Scale(const Vector3& value)
{

}


