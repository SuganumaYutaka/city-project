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
#include "BuildingRule.h"

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
void Wall::Init( float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, BuildingRule* rule)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;

	//フロアの生成
	rule->ProceduralFloor( this);

	UpdateView( bottomLeftPosition, normal);
}

/*------------------------------------------------------------------------------
	初期化（円に沿って曲げる）
------------------------------------------------------------------------------*/
void Wall::Init( float height, float width, const Vector3& bottomLeftPosition, const Vector3& center, float radius, BuildingRule* rule)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;

	//フロアの生成
	rule->ProceduralFloor( this);

	UpdateView( bottomLeftPosition, center, radius);
}

/*------------------------------------------------------------------------------
	描画情報の更新
------------------------------------------------------------------------------*/
bool Wall::UpdateView( const Vector3& bottomLeftPosition, const Vector3& normal)
{
	m_BottomLeftPosition = bottomLeftPosition;

	//Rendererの更新
	Vector3 position = bottomLeftPosition;
	Vector3 vector = Vector3::Cross( normal, Vector3(0.0f, 1.0f, 0.0f)).Normalize();
	
	auto pVtx = m_Renderer->StartSetVertexBuffer( CulcCountVertex() + 1, CulcCountPolygon());
	
	for (auto floor : m_Floors)
	{
		floor->SetVertexBuffer( pVtx, position, normal, vector);
		auto countVertex = floor->CulcCountVertex();
		pVtx += countVertex;
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
	m_BottomLeftPosition = bottomLeftPosition;

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

/*------------------------------------------------------------------------------
	フロアの追加
------------------------------------------------------------------------------*/
void Wall::AddFloor( Floor* floor)
{ 
	m_Floors.push_back( floor);
}

/*------------------------------------------------------------------------------
	フロアの削除
------------------------------------------------------------------------------*/
void Wall::SubFloor(Floor* floor)
{
	for (auto ite = m_Floors.begin(); ite != m_Floors.end(); ++ite)
	{
		if (*ite == floor)
		{
			delete floor;
			m_Floors.erase( ite);
			return;
		}
	}

	DebugLog::Add( "WallにFloorが見つからない\n");
}

/*------------------------------------------------------------------------------
	テクスチャのロード
------------------------------------------------------------------------------*/
void Wall::LoadTexture(std::string fileName)
{
	m_Renderer->LoadTexture( fileName);
}

/*------------------------------------------------------------------------------
	同一ShapeのWallと融合する
------------------------------------------------------------------------------*/
void Wall::FusionSameShape(Wall* other)
{
	//Floorの融合
	auto destFloor = other->m_Floors.begin();
	for (auto sourceFloor = m_Floors.begin(); sourceFloor != m_Floors.end(); ++sourceFloor)
	{
		(*sourceFloor)->FusionSameShape( (*destFloor));
		++destFloor;
	}
}
