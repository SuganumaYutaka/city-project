/*==============================================================================

    CityAttribute.cpp - 町の自動生成ー属性
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "CityAttribute.h"
#include "GameObject.h"
#include "Edge.h"
#include "Vertex.h"
#include "Face.h"
#include "HalfEdge.h"
#include "JunctionView.h"
#include "RoadView.h"
#include "BlockView.h"

#include "TrafficJunction.h"
#include "TrafficRoad.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define DEFAULT_ROAD_WIDTH (12.0f)			//デフォルトの道路幅

/*------------------------------------------------------------------------------
	交差点ーコンストラクタ
------------------------------------------------------------------------------*/
JunctionAttribute::JunctionAttribute( GameObject* parent)
{
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<JunctionView>();
	m_TrafficJunction = gameObject->AddComponent<TrafficJunction>();
}

/*------------------------------------------------------------------------------
	交差点ーデストラクタ
------------------------------------------------------------------------------*/
JunctionAttribute::~JunctionAttribute()
{
	if( m_View)
	{
		m_View->m_pGameObject->ReleaseReserve();
	}
}

/*------------------------------------------------------------------------------
	交差点ー初期化
------------------------------------------------------------------------------*/
void JunctionAttribute::Init(void)
{
	m_View->SetAttribute( this);
	m_TrafficJunction->SetAttribute( this);
}

/*------------------------------------------------------------------------------
	交差点ー更新
------------------------------------------------------------------------------*/
void JunctionAttribute::Update(void)
{
	m_View->UpdateAttribute();
	m_TrafficJunction->UpdateAttribute();
}

/*------------------------------------------------------------------------------
	位置を取得
------------------------------------------------------------------------------*/
const Vector3& JunctionAttribute::GetPosition(void)
{
	return GetVertex()->GetPosition();
}

/*------------------------------------------------------------------------------
	道路ーコンストラクタ
------------------------------------------------------------------------------*/
RoadAttribute::RoadAttribute( GameObject* parent) : Width( DEFAULT_ROAD_WIDTH)
{
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<RoadView>();
	m_TrafficRoad = gameObject->AddComponent<TrafficRoad>();
}

/*------------------------------------------------------------------------------
	道路ーデストラクタ
------------------------------------------------------------------------------*/
RoadAttribute::~RoadAttribute()
{
	if( m_View)
	{
		m_View->m_pGameObject->ReleaseReserve();
	}
}

/*------------------------------------------------------------------------------
	道路ー初期化
------------------------------------------------------------------------------*/
void RoadAttribute::Init(void)
{
	m_View->SetAttribute( this);
	m_TrafficRoad->SetAttribute( this);
}

/*------------------------------------------------------------------------------
	道路ー更新
------------------------------------------------------------------------------*/
void RoadAttribute::Update(void)
{
	m_View->UpdateAttribute();
	m_TrafficRoad->UpdateAttribute();
}

/*------------------------------------------------------------------------------
	道路ー長さを取得
------------------------------------------------------------------------------*/
float RoadAttribute::GetLength( void)
{
	Vector3 start = GetEdge()->GetStart()->GetPosition();
	Vector3 end = GetEdge()->GetEnd()->GetPosition();

	return Vector3::Distance( start, end);
}

/*------------------------------------------------------------------------------
	道路ー中心位置を取得
------------------------------------------------------------------------------*/
Vector3 RoadAttribute::GetCenterPosition( void)
{
	return ( GetEdge()->GetStart()->GetPosition() + GetEdge()->GetEnd()->GetPosition()) * 0.5f;
}

/*------------------------------------------------------------------------------
	道路ーベクトルを取得
------------------------------------------------------------------------------*/
Vector3 RoadAttribute::GetVector( void)
{
	return GetEdge()->GetVector();
}

/*------------------------------------------------------------------------------
	道路ー頂点情報を取得
------------------------------------------------------------------------------*/
std::vector<Vector3> RoadAttribute::GetVertices(void)
{
	std::vector<Vector3> vertices;
	auto edge = GetEdge();
	Vector3 start = GetEdge()->GetStart()->GetPosition();
	Vector3 end = GetEdge()->GetEnd()->GetPosition();
	Vector3 vector = GetVector();
	Vector3 vertical( -vector.z, vector.y, vector.x);
	vertical = vertical.Normalize();
	
	vertices.push_back( start + vertical * Width * 0.5f);
	vertices.push_back( end + vertical * Width * 0.5f);
	vertices.push_back( end - vertical * Width * 0.5f);
	vertices.push_back( start - vertical * Width * 0.5f);
	
	return vertices;
}

/*------------------------------------------------------------------------------
	区画ーコンストラクタ
------------------------------------------------------------------------------*/
BlockAttribute::BlockAttribute( GameObject* parent, BuildingRuleFactory* buildingRuleFactory, BuildingManager* buildingManager)
{
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<BlockView>();
	m_BuildingRuleFactory = buildingRuleFactory;
	m_BuildingManager = buildingManager;
}

/*------------------------------------------------------------------------------
	区画ーデストラクタ
------------------------------------------------------------------------------*/
BlockAttribute::~BlockAttribute()
{
	if( m_View)
	{
		m_View->m_pGameObject->ReleaseReserve();
	}
}

/*------------------------------------------------------------------------------
	区画ー初期化
------------------------------------------------------------------------------*/
void BlockAttribute::Init(void)
{
	m_View->SetAttribute( this);
}

/*------------------------------------------------------------------------------
	区画ー更新
------------------------------------------------------------------------------*/
void BlockAttribute::Update(void)
{
	m_View->UpdateAttribute();
}
