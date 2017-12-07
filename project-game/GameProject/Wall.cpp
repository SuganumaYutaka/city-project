/*==============================================================================

    Wall.cpp - 建物の自動生成ー壁
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Wall.h"
#include "Floor.h"
#include "WallRenderer.h"
#include "GameObject.h"
#include "ComponentInclude.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Wall::Wall( GameObject* buildingObject)
{
	m_Height = 1.0f;
	m_Width = 1.0f;
	m_Floors.clear();

	m_Renderer = buildingObject->AddComponent<WallRenderer>();
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Wall::~Wall()
{
	//フロアの消去
	for (auto floor : m_Floors)
	{
		delete floor;
	}
	m_Floors.clear();
}


/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void Wall::Init( float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal)
{
	m_Height = height;
	m_Width = width;

	//TODO:フロアの生成


	UpdateView( bottomLeftPosition, normal);
}

/*------------------------------------------------------------------------------
	初期化（円に沿って曲げる）
------------------------------------------------------------------------------*/
void Wall::Init( float height, float width, const Vector3& bottomLeftPosition, const Vector3& center, float radius)
{
	m_Height = height;
	m_Width = width;

	//TODO:フロアの生成


	UpdateView( bottomLeftPosition, center, radius);
}

/*------------------------------------------------------------------------------
	描画情報の更新
------------------------------------------------------------------------------*/
bool Wall::UpdateView( const Vector3& bottomLeftPosition, const Vector3& normal)
{
	//Rendererの更新
	Vector3 position = bottomLeftPosition;
	Vector3 vector = Vector3::Cross( normal, Vector3(0.0f, 1.0f, 0.0f)).Normalize();
	
	auto pVtx = m_Renderer->StartSetVertexBuffer( CulcCountVertex(), CulcCountPolygon());
	
	for (auto floor : m_Floors)
	{
		floor->SetVertexBuffer( pVtx, position, normal, vector);
		position.y += floor->GetHeight();
	}

	m_Renderer->EndSetVertexBuffer();

	return true;
}

/*------------------------------------------------------------------------------
	描画情報の更新（円に沿って曲げる）
------------------------------------------------------------------------------*/
bool Wall::UpdateView( const Vector3& bottomLeftPosition, const Vector3& center, float radius)
{
	//Rendererの更新
	Vector3 position = bottomLeftPosition;

	auto pVtx = m_Renderer->StartSetVertexBuffer( CulcCountVertex(), CulcCountPolygon());

	for (auto floor : m_Floors)
	{
		floor->SetVertexBufferCurve( pVtx, position, center, radius);
		position.y += floor->GetHeight();
	}

	m_Renderer->EndSetVertexBuffer();

	return true;
}

/*------------------------------------------------------------------------------
	頂点数を算出
------------------------------------------------------------------------------*/
int Wall::CulcCountVertex(void)
{
	int count = 0;
	for (auto floor : m_Floors)
	{
		count += floor->CulcCountVertex();
	}

	return count - 1;
}

/*------------------------------------------------------------------------------
	頂点数を算出（円に沿って曲げる場合）
------------------------------------------------------------------------------*/
int Wall::CulcCountVertexCurve( float radius)
{
	int count = 0;
	for (auto floor : m_Floors)
	{
		count += floor->CulcCountVertexCurve( radius);
	}

	return count - 1;
}

/*------------------------------------------------------------------------------
	ポリゴン数を算出
------------------------------------------------------------------------------*/
int Wall::CulcCountPolygon(void)
{
	int count = 0;
	for (auto floor : m_Floors)
	{
		count += floor->CulcCountPolygon();
	}

	return count - 2;
}

/*------------------------------------------------------------------------------
	ポリゴン数を算出（円に沿って曲げる場合）
------------------------------------------------------------------------------*/
int Wall::CulcCountPolygonCurve(float radius)
{
	int count = 0;
	for (auto floor : m_Floors)
	{
		count += floor->CulcCountPolygonCurve( radius);
	}

	return count - 2;
}