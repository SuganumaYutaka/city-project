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
#include "Land.h"
#include "CityAttributeManager.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	交差点ーコンストラクタ
------------------------------------------------------------------------------*/
JunctionAttribute::JunctionAttribute( Model* model, int id, CityAttributeManager* manager, GameObject* parent) : VertexAttribute( model, id), m_Manager( manager)
{
	m_GameObject = new GameObject( parent);
	m_GameObject->IsCreatedByOtherComponent = true;
	m_View = m_GameObject->AddComponent<JunctionView>();
	m_TrafficJunction = m_GameObject->AddComponent<TrafficJunction>();

	m_Manager->RegisterJunction( this);
}

/*------------------------------------------------------------------------------
	交差点ーデストラクタ
------------------------------------------------------------------------------*/
JunctionAttribute::~JunctionAttribute()
{
	
}

/*------------------------------------------------------------------------------
	交差点ー削除
------------------------------------------------------------------------------*/
void JunctionAttribute::Delete(void)
{
	m_GameObject->ReleaseReserve();

	//リンクを解除
	auto vertex = GetVertex();
	if (vertex)
	{
		vertex->LinkAttribute( NULL);
	}

	m_Manager->UnregisterJunction( this);

	delete this;
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
RoadAttribute::RoadAttribute( Model* model, int id, CityAttributeManager* manager, GameObject* parent) : EdgeAttribute( model, id), m_Manager( manager), Width( DEFAULT_ROAD_WIDTH)
{
	m_GameObject = new GameObject( parent);
	m_GameObject->IsCreatedByOtherComponent = true;
	m_View = m_GameObject->AddComponent<RoadView>();
	m_TrafficRoad = m_GameObject->AddComponent<TrafficRoad>();

	m_Manager->RegisterRoad( this);
}

/*------------------------------------------------------------------------------
	道路ーデストラクタ
------------------------------------------------------------------------------*/
RoadAttribute::~RoadAttribute()
{
	
}

/*------------------------------------------------------------------------------
	道路ー削除
------------------------------------------------------------------------------*/
void RoadAttribute::Delete(void)
{
	m_GameObject->ReleaseReserve();

	//リンクを解除
	auto edge = GetEdge();
	if (edge)
	{
		edge->LinkAttribute( NULL);
	}

	m_Manager->UnregisterRoad( this);

	delete this;
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
BlockAttribute::BlockAttribute( Model* model, int id, CityAttributeManager* manager, GameObject* parent) : FaceAttribute( model, id), m_Manager( manager)
{
	m_GameObject = new GameObject( parent);
	m_GameObject->IsCreatedByOtherComponent = true;
	m_View = m_GameObject->AddComponent<BlockView>();
	m_Lands.clear();

	m_Manager->RegisterBlock( this);
}

/*------------------------------------------------------------------------------
	区画ーデストラクタ
------------------------------------------------------------------------------*/
BlockAttribute::~BlockAttribute()
{
	
}

/*------------------------------------------------------------------------------
	区画ー削除
------------------------------------------------------------------------------*/
void BlockAttribute::Delete(void)
{
	m_GameObject->ReleaseReserve();

	//土地に消去を伝える
	int count = m_Lands.size();
	for (int i = 0; i < count; i++)
	{
		if (m_Lands[i])
		{
			m_Lands[i]->OnDeleteBlock();
		}
	}
	m_Lands.clear();
	m_Lands.shrink_to_fit();

	//リンクを解除
	auto face = GetFace();
	if (face)
	{
		face->LinkAttribute( NULL);
	}

	m_Manager->UnregisterBlock( this);

	delete this;
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

/*------------------------------------------------------------------------------
	区画ー土地をリンク
------------------------------------------------------------------------------*/
int BlockAttribute::LinkLand(Land* land)
{
	m_Lands.push_back( land);
	return m_Lands.size() - 1;
}

/*------------------------------------------------------------------------------
	区画ー土地のリンクの解除
------------------------------------------------------------------------------*/
void BlockAttribute::UnlinkLand(Land* land)
{
	int size = m_Lands.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Lands[i] == land)
		{
			m_Lands[i] = NULL;
		}
	}
}

/*------------------------------------------------------------------------------
	区画ー土地のリンクの解除
------------------------------------------------------------------------------*/
void BlockAttribute::UnlinkLand(int landID)
{
	m_Lands[ landID] = NULL;
}


