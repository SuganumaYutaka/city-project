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
#include "CityAttributeManager.h"
#include "CityAttribute.h"
#include "LandManager.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Land::Land( LandManager* manager, GameObject* parent)
{
	m_GameObject = new GameObject( parent);
	m_Renderer = m_GameObject->AddComponent<MeshPolygonRenderer>();
	m_TrafficLand = NULL;
	m_AttributeManager = NULL;
	m_BlockID = -1;

	m_Manager = manager;
	m_Manager->RegisterLand( this);
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Land::~Land()
{
	m_GameObject->ReleaseReserve();

	m_Vertices.clear();
	m_Vertices.shrink_to_fit();

	//リンクを解除
	if (m_AttributeManager)
	{
		m_AttributeManager->GetBlock( m_BlockID)->UnlinkLand( this);
	}
	m_Manager->UnregisterLand( this);
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
	//デストラクタ時にリンクの解除をしない
	m_AttributeManager = NULL;
	m_BlockID = -1;

	//消去する
	delete this;
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
	区画とリンクする
------------------------------------------------------------------------------*/
void Land::LinkAttribute(CityAttributeManager* attributeManager, int blockID)
{
	m_AttributeManager = attributeManager;
	m_BlockID = blockID;

	m_AttributeManager->GetBlock( m_BlockID)->LinkLand( this);
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

/*------------------------------------------------------------------------------
	建物をリンク
------------------------------------------------------------------------------*/
int Land::LinkBuilding(Building* building)
{
	m_Buildings.push_back( building);
	return m_Buildings.size() - 1;
}

/*------------------------------------------------------------------------------
	建物のリンクの解除
------------------------------------------------------------------------------*/
void Land::UnlinkBuilding(Building* building)
{
	int size = m_Buildings.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Buildings[i] == building)
		{
			m_Buildings[i] = NULL;
		}
	}
}

/*------------------------------------------------------------------------------
	建物のリンクの解除
------------------------------------------------------------------------------*/
void Land::UnlinkBuilding(int buildingID)
{
	m_Buildings[ buildingID] = NULL;
}

