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
#include "TileSplit.h"
#include <math.h>

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define SPLIT_EPSILON (0.00001f)

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
void Wall::InitDefault( D3DXMATRIX shapeMatrix, float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, BuildingRule* rule)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = normal;
	m_IsCurve = false;

	//フロアの生成
	rule->ProceduralFloor( this);

	UpdateView( shapeMatrix);
}

/*------------------------------------------------------------------------------
	初期化（円に沿って曲げる）
------------------------------------------------------------------------------*/
void Wall::InitCurve( D3DXMATRIX shapeMatrix, float height, float width, const Vector3& bottomLeftPosition,  BuildingRule* rule)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = bottomLeftPosition.Normalize();
	m_IsCurve = true;

	//フロアの生成
	rule->ProceduralFloorCurve( this);

	UpdateView( shapeMatrix);
}

/*------------------------------------------------------------------------------
	描画情報の更新
------------------------------------------------------------------------------*/
bool Wall::UpdateView( D3DXMATRIX shapeMatrix)
{
	//壁の更新
	Transform( shapeMatrix);

	//Rendererの更新
	auto pVtx = m_Renderer->StartSetVertexBuffer( CulcCountVertex() + 1, CulcCountPolygon());
	for (auto floor : m_Floors)
	{
		floor->Transform( shapeMatrix);
		floor->SetVertexBuffer( pVtx);
		pVtx += floor->CulcCountVertex();
	}
	m_Renderer->EndSetVertexBuffer();

	return true;
}

/*------------------------------------------------------------------------------
	位置と法線の更新
------------------------------------------------------------------------------*/
void Wall::Transform(D3DXMATRIX shapeMatrix)
{
	auto position = m_BottomLeftPosition.ConvertToDX();
	auto normal = m_Normal.ConvertToDX();

	D3DXVec3TransformCoord( &position, &position, &shapeMatrix);
	D3DXVec3TransformNormal( &normal, &normal, &shapeMatrix);

	m_BottomLeftPosition = Vector3::ConvertFromDX( position);
	m_Normal = Vector3::ConvertFromDX( normal);
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

	//Rendererの消去
	other->m_Renderer->ReleaseReserve();
	other->m_Renderer = NULL;
}

/*------------------------------------------------------------------------------
	環状リストに変更
------------------------------------------------------------------------------*/
bool Wall::ChangeRingList(void)
{
	for (auto floor : m_Floors)
	{
		if (!floor->ChangeRingList())
		{
			return false;
		}
	}

	return true;
}

/*------------------------------------------------------------------------------
	ベクトルの取得
------------------------------------------------------------------------------*/
Vector3 Wall::GetVector(void)
{
	if (m_IsCurve)
	{
		return Vector3();
	}
	auto cross = Vector3::Cross( m_Normal, Vector3( 0.0f, 1.0f, 0.0f));
	return cross.Normalize() * m_Width;
}

/*------------------------------------------------------------------------------
	中心位置の取得
------------------------------------------------------------------------------*/
Vector3 Wall::GetCenter(void)
{
	if (!m_IsCurve)
	{
		return Vector3();
	}

	return m_BottomLeftPosition - m_Normal * GetRadius();
}

/*------------------------------------------------------------------------------
	半径の取得
------------------------------------------------------------------------------*/
float Wall::GetRadius(void)
{
	if (!m_IsCurve)
	{
		return 0.0f;
	}

	return m_Width * 0.5f / D3DX_PI;
}

/*------------------------------------------------------------------------------
	壁との裂け目処理
------------------------------------------------------------------------------*/
bool Wall::Split(Wall* other)
{
	if (m_IsCurve)
	{
		if (other->m_IsCurve)
		{
			if (!SplitCylinders(this, other))
			{
				return false;
			}
		}
		else
		{
			if (!SplitPlaneCylinder(other, this))
			{
				return false;
			}
		}
	}
	else
	{
		if (other->m_IsCurve)
		{
			if (!SplitPlaneCylinder(this, other))
			{
				return false;
			}
		}
		else
		{
			if (!SplitPlanes(this, other))
			{
				return false;
			}
		}
	}

	return true;
}

/*------------------------------------------------------------------------------
	壁との裂け目処理（平面と平面）
------------------------------------------------------------------------------*/
bool Wall::SplitPlanes(Wall* source, Wall* dest)
{
	//壁との交点を算出
	Vector3 positionSplit;
	if (!CulcPositionSplitPlanes(source, dest, &positionSplit))
	{
		return false;
	}

	//始点からの距離を算出
	float length1 = Vector3::Distance( source->GetBottomLeftPosition(), positionSplit);
	float length2 = Vector3::Distance( dest->GetBottomLeftPosition(), positionSplit);
	
	//最後の余白分を除くフロア階層分境目を設定
	auto floors1 = source->GetFloors();
	auto floors2 = dest->GetFloors();

	//階数が少ない壁を基準として挿入処理
	if (floors1.size() <= floors2.size())
	{
		if (!InsertSplit(floors1, floors2, length1, length2, positionSplit))
		{
			return false;
		}
	}
	else
	{
		if (!InsertSplit(floors2, floors1, length2, length1, positionSplit))
		{
			return false;
		}
	}

	return true;
}

/*------------------------------------------------------------------------------
	壁との裂け目処理（円柱と円柱）
------------------------------------------------------------------------------*/
bool Wall::SplitCylinders(Wall* source, Wall* dest)
{
	//円同士の交点を二つ算出
	Vector3 point1, point2;
	if (!CulcPositionSplitCylinders(source, dest, &point1, &point2))
	{
		return false;
	}

	//交点と壁の始点との距離を算出
	Vector3 center1 = source->GetCenter();
	float radius1 = source->GetRadius();
	Vector3 center2 = dest->GetCenter();
	float radius2 = dest->GetRadius();
	float lengthSource1 = CulcLengthArc( source->GetBottomLeftPosition(), point1, center1, radius1);
	float lengthSource2 = CulcLengthArc( source->GetBottomLeftPosition(), point2, center1, radius1);
	float lengthDest1 = CulcLengthArc( dest->GetBottomLeftPosition(), point1, center2, radius2);
	float lengthDest2 = CulcLengthArc( dest->GetBottomLeftPosition(), point2, center2, radius2);

	//最後の余白分を除くフロア階層分境目を設定
	auto floors1 = source->GetFloors();
	auto floors2 = dest->GetFloors();

	//階数が少ない壁を基準として挿入処理
	if (floors1.size() <= floors2.size())
	{
		if (!InsertSplit(floors1, floors2, lengthSource1, lengthDest1, point1))
		{
			return false;
		}
		if (!InsertSplit(floors1, floors2, lengthSource2, lengthDest2, point2))
		{
			return false;
		}
	}
	else
	{
		if (!InsertSplit(floors2, floors1, lengthDest1, lengthSource1, point1))
		{
			return false;
		}
		if (!InsertSplit(floors2, floors1, lengthDest2, lengthSource2, point2))
		{
			return false;
		}
	}

	return true;
}

/*------------------------------------------------------------------------------
	壁との裂け目処理（平面と円柱）
------------------------------------------------------------------------------*/
bool Wall::SplitPlaneCylinder(Wall* plane, Wall* cylinder)
{


	return true;
}

/*------------------------------------------------------------------------------
	壁との交点を算出（平面と平面）
------------------------------------------------------------------------------*/
bool Wall::CulcPositionSplitPlanes(Wall* source, Wall* dest, Vector3* out)
{
	//変数の設定
	Vector3 s1 = source->GetBottomLeftPosition();
	Vector3 s2 = dest->GetBottomLeftPosition();
	Vector3 v1 = source->GetVector();
	Vector3 v2 = dest->GetVector();
	Vector3 n1 = v1.Normalize();
	Vector3 n2 = v2.Normalize();
	float work1 = Vector3::Dot( n1, n2);
	float work2 = 1 - work1 * work1;

	//平行か確認
	if( work2 > -SPLIT_EPSILON && work2 < SPLIT_EPSILON){ return 0;}

	//始点を結ぶベクトル
	Vector3 vs = s2 - s1;

	//始点からの距離
	float d1 = ( Vector3::Dot( vs, n1) - work1 * Vector3::Dot( vs, n2)) / work2;
	float d2 = ( work1 * Vector3::Dot( vs, n1) - Vector3::Dot( vs, n2)) / work2;

	//v1上の最近点
	Vector3 p1;
	p1.x = s1.x + d1 * n1.x;
	p1.y = s1.y + d1 * n1.y;
	p1.z = s1.z + d1 * n1.z;

	//v2上の最近点
	Vector3 p2;
	p2.x = s2.x + d2 * n2.x;
	p2.y = s2.y + d2 * n2.y;
	p2.z = s2.z + d2 * n2.z;

	//交差判定
	if (Vector3::Distance(p1, p2) > 0.01f)
	{
		return false;
	}

	//線分の長さ判定
	if (v1.Length() < d1)
	{
		return false;
	}
	if (v2.Length() < d2)
	{
		return false;
	}

	//線分の方向判定
	auto vec1 = p1 - s1;
	auto dot1 = Vector3::Dot( n1, vec1.Normalize());
	if (dot1 < 0)
	{
		return false;
	}
	auto vec2 = p2 - s2;
	auto dot2 = Vector3::Dot( n2, vec2.Normalize());
	if (dot2 < 0)
	{
		return false;
	}
	
	if(out)
	{
		*out = p1;
	}

	return true;
}

/*------------------------------------------------------------------------------
	壁との交点を算出（円柱の側面同士）
------------------------------------------------------------------------------*/
bool Wall::CulcPositionSplitCylinders(Wall* source, Wall* dest, Vector3* out1, Vector3* out2)
{
	Vector3 center1 = source->GetCenter();
	Vector3 center2 = dest->GetCenter();
	float radius1 = source->GetRadius();
	float radius2 = dest->GetRadius();
	Vector3 vec = center2 - center1;
	float len = vec.Length();

	//距離が離れているか判定
	if (len >= radius1 + radius2)
	{
		return false;
	}

	//円が円を内包しているか判定
	if (abs(radius1 - radius2) >= len)
	{
		return false;
	}

	//完全に重なるか判定
	if (radius1 == radius2 && len == 0)
	{
		return false;
	}

	//X軸に対するvの角度
	float angle = atan2f( vec.z, vec.x);

	//中心を結んだベクトルと交点のなす角を算出
	float value = acosf( (len * len + radius1 * radius1 - radius2 * radius2) / ( radius1 * 2.0f * len));

	*out1 = center1 + Vector3( radius1 * cosf( angle + value), 0.0f, radius1 * sinf( angle + value));
	*out2 = center2 + Vector3( radius1 * cosf( angle - value), 0.0f, radius1 * sinf( angle - value));

	//交点が近すぎる場合
	if (Vector3::Distance(*out1, *out2) < SPLIT_EPSILON)
	{
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------
	弧の長さを算出
------------------------------------------------------------------------------*/
float Wall::CulcLengthArc(const Vector3& point1, const Vector3& point2, const Vector3& center, float radius)
{
	//内積で角度を算出
	Vector3 v1 = point1 - center;
	Vector3 v2 = point2 - center;
	float angle = acos( Vector3::Dot( v1.Normalize(), v2.Normalize()));

	//外積で0～2PIにする
	Vector3 cross = Vector3::Cross( v1, v2);
	if (cross.y > 0.0f)
	{
		angle = 2 * D3DX_PI - angle;
	}

	return angle * radius;
}

/*------------------------------------------------------------------------------
	境目の挿入
------------------------------------------------------------------------------*/
bool Wall::InsertSplit(const std::list<Floor*>& floors1, const std::list<Floor*>& floors2, float length1, float length2, Vector3 positionSplit)
{
	auto ite2 = floors2.begin();
	auto ite1 = floors1.begin();
	for(;;)
	{
		if (ite1 == floors1.end())
		{
			 break;
		}
		if ((*ite1)->GetType() == eFloorMargin)
		{
			break;
		}

		//境目となるタイルを生成して各フロアに挿入する
		TileSplit* split1 = new TileSplit();
		TileSplit* split2 = new TileSplit();
		split1->Init( split2, positionSplit);
		split2->Init( split1, positionSplit);

		if (!(*ite1)->InsertSplit(split1, length1))
		{
			delete split1;
			delete split2;
			return false;
		}
		if (!(*ite2)->InsertSplit(split2, length2))
		{
			delete split1;
			delete split2;
			return false;
		}

		//境目の位置を高さ分ずらす
		positionSplit.y += (*ite1)->GetHeight();

		++ite1;
		++ite2;
	}

	return true;
}

/*------------------------------------------------------------------------------
	フロアの先頭タイルを取得
------------------------------------------------------------------------------*/
Tile* Wall::GetStartTile(int floorCount)
{
	//階層が一致するフロアの先頭タイルを返す
	int count = 0;
	for (auto floor : m_Floors)
	{
		if (count == floorCount)
		{
			return floor->GetTile();
		}
		count++;
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	レンダラーの消去
------------------------------------------------------------------------------*/
void Wall::ClearRenderer(void)
{
	if (m_Renderer)
	{
		m_Renderer->ReleaseReserve();
		m_Renderer = NULL;
	}
}

/*------------------------------------------------------------------------------
	一番上のタイルを取得
------------------------------------------------------------------------------*/
Tile* Wall::GetTopTile(void)
{
	return m_Floors.back()->GetTile();
}
