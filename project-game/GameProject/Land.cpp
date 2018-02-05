/*==============================================================================

    Land.h - 町の自動生成ー土地
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Land.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "TrafficLand.h"
#include "MeshPolygonRenderer.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Land::Land(GameObject* parent)
{
	m_GameObject = new GameObject( parent);
	m_Renderer = m_GameObject->AddComponent<MeshPolygonRenderer>();
	m_TrafficLand = NULL;
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Land::~Land()
{
	m_GameObject->ReleaseReserve();

	m_Vertices.clear();
	m_Vertices.shrink_to_fit();
}

/*------------------------------------------------------------------------------
	初期化処理
------------------------------------------------------------------------------*/
void Land::Init( const std::vector<Vector3>& vertices)
{
	m_Vertices.clear();
	m_Vertices = vertices;
	m_Renderer->SetVertices( vertices);
}

/*------------------------------------------------------------------------------
	区画消去時の処理
------------------------------------------------------------------------------*/
void Land::OnDeleteBlock(void)
{
	
}

/*------------------------------------------------------------------------------
	中心位置を算出
------------------------------------------------------------------------------*/
Vector3 Land::CulcCenterPosition(void)
{
	Vector3 vec02 = m_Vertices[2] - m_Vertices[0];
	return m_Vertices[0] + vec02 * 0.5f;
}

/*------------------------------------------------------------------------------
	交通システムの設定
------------------------------------------------------------------------------*/
void Land::SetTraffic(std::list<RoadAttribute*> roadAttributes)
{
	if (!m_TrafficLand)
	{
		m_TrafficLand = m_GameObject->AddComponent<TrafficLand>();
	}

	m_TrafficLand->Init( roadAttributes);
}

