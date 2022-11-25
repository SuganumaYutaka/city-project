/*==============================================================================

    Building.h - 町の自動生成ー建物
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Building.h"
#include "GameObject.h"
#include "BuildingGeometry.h"
#include "Land.h"
#include "LandManager.h"
#include "BuildingManager.h"
#include "BuildingParameter.h"
#include "WallRenderer.h"
#include "Transform.h"
#include "BuildingSurfacePattern.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Building::Building(BuildingManager* manager, GameObject* parent)
{
	m_GameObject = new GameObject( parent);
	m_Geometry = NULL;
	m_LandManager = NULL;
	m_LandID = -1;

	m_Manager = manager;
	m_Manager->Register( this);
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Building::~Building()
{
	
}

/*------------------------------------------------------------------------------
	消去
------------------------------------------------------------------------------*/
void Building::Delete(void)
{
	m_GameObject->ReleaseReserve();

	//リンクを解除
	if (m_LandManager)
	{
		m_LandManager->GetLand( m_LandID)->UnlinkBuilding( this);
	}
	m_Manager->Unregister( this);

	delete this;
}

/*------------------------------------------------------------------------------
	ジオメトリの初期化
------------------------------------------------------------------------------*/
void Building::InitGeometry(GeometryParameter* parameter, BuildingSurfacePattern* surfacePattern)
{
	//すでにあるジオメトリと描画情報を削除
	if (m_Geometry)
	{
		m_Geometry->ReleaseReserve();
		m_Geometry = NULL;
		auto walls = m_GameObject->GetComponentList<WallRenderer>();
		for (auto wall : walls)
		{
			wall->ReleaseReserve();
		}
	}

	m_Geometry = m_GameObject->AddComponent<BuildingGeometry>();
	m_Geometry->Init( parameter, surfacePattern);
}

/*------------------------------------------------------------------------------
	土地消去時の処理
------------------------------------------------------------------------------*/
void Building::OnDeleteLand(void)
{
	//消去時にリンクの解除をしない
	m_LandManager = NULL;
	m_LandID = -1;

	//消去する
	Delete();
}

/*------------------------------------------------------------------------------
	土地とリンクする
------------------------------------------------------------------------------*/
void Building::LinkLand( LandManager* landManager, int landID)
{
	m_LandManager = landManager;
	m_LandID = landID;

	m_LandManager->GetLand( m_LandID)->LinkBuilding( this);

	//GameObjectを移動
	m_GameObject->m_pTransform->SetWorldPosition( m_LandManager->GetLand( m_LandID)->GetGameObject()->m_pTransform->GetWorldPosition());
}

/*------------------------------------------------------------------------------
	ジオメトリを確定させる
------------------------------------------------------------------------------*/
void Building::ConfirmGeometry(void)
{
	if (!m_Geometry)
	{
		return;
	}
	m_Geometry->ConfirmGeometry();
	
	//ジオメトリ情報の削除
	m_Geometry->ReleaseReserve();
	m_Geometry =NULL;
}
