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

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define DEFAULT_WIDTH (2.0f)	//デフォルトの幅

/*------------------------------------------------------------------------------
	交差点ーコンストラクタ
------------------------------------------------------------------------------*/
JunctionAttribute::JunctionAttribute( GameObject* parent)
{
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<JunctionView>();
}

/*------------------------------------------------------------------------------
	交差点ーデストラクタ
------------------------------------------------------------------------------*/
JunctionAttribute::~JunctionAttribute()
{
	if( m_View)
	{
		m_View->ReleaseReserve();
	}
}

/*------------------------------------------------------------------------------
	交差点ー初期化
------------------------------------------------------------------------------*/
void JunctionAttribute::Init(void)
{
	m_View->SetAttribute( this);
}

/*------------------------------------------------------------------------------
	交差点ー更新
------------------------------------------------------------------------------*/
void JunctionAttribute::Update(void)
{
	m_View->UpdateAttribute();
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
RoadAttribute::RoadAttribute( GameObject* parent) : Width( DEFAULT_WIDTH)
{
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<RoadView>();
}

/*------------------------------------------------------------------------------
	道路ーデストラクタ
------------------------------------------------------------------------------*/
RoadAttribute::~RoadAttribute()
{
	if( m_View)
	{
		m_View->ReleaseReserve();
	}
}

/*------------------------------------------------------------------------------
	道路ー初期化
------------------------------------------------------------------------------*/
void RoadAttribute::Init(void)
{
	m_View->SetAttribute( this);
}

/*------------------------------------------------------------------------------
	道路ー更新
------------------------------------------------------------------------------*/
void RoadAttribute::Update(void)
{
	m_View->UpdateAttribute();
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
	区画ーコンストラクタ
------------------------------------------------------------------------------*/
BlockAttribute::BlockAttribute( GameObject* parent)
{
	m_ViewGameObject = new GameObject( parent);
	m_ViewGameObject->IsCreatedByOtherComponent = true;
	m_View = m_ViewGameObject->AddComponent<BlockView>();
}

/*------------------------------------------------------------------------------
	区画ーデストラクタ
------------------------------------------------------------------------------*/
BlockAttribute::~BlockAttribute()
{
	if( m_View)
	{
		m_View->ReleaseReserve();
	}

	for (auto building : m_Buildings)
	{
		delete building;
	}
	m_Buildings.clear();
}

/*------------------------------------------------------------------------------
	区画ー初期化
------------------------------------------------------------------------------*/
void BlockAttribute::Init(void)
{
	m_View->SetAttribute( this);
	
	CreateBuilding();
}

/*------------------------------------------------------------------------------
	区画ー更新
------------------------------------------------------------------------------*/
void BlockAttribute::Update(void)
{
	m_View->UpdateAttribute();

	CreateBuilding();
}

/*------------------------------------------------------------------------------
	区画ー建物の生成
------------------------------------------------------------------------------*/
bool BlockAttribute::CreateBuilding(void)
{
	//現在の建物を削除
	for (auto building : m_Buildings)
	{
		delete building;
	}
	m_Buildings.clear();



	return true;
}
