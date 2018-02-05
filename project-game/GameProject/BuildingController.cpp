/*==============================================================================

    BuildingController.cpp - 建物の自動生成ー建物コントローラ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/24
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BuildingController.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "Land.h"
#include "Shape.h"
#include "Wall.h"

#include "BuildingRule.h"
#include "BuildingGeometry.h"
#include "CityAttribute.h"
#include "BuildingManager.h"
#include "TrafficBuilding.h"

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* BuildingController::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BuildingController>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
BuildingController::BuildingController( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Geometry = NULL;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void BuildingController::Uninit( void)
{
	if (m_BuildingManager)
	{
		m_BuildingManager->Unregister( this);
	}
}

/*------------------------------------------------------------------------------
	初期化処理
------------------------------------------------------------------------------*/
bool BuildingController::Init( Land* land, BuildingRule* rule, std::list<RoadAttribute*> roads, BuildingManager* buildingManager, CarManager* carManager)
{
	m_BuildingManager = buildingManager;
	m_BuildingManager->Register( this);
	
	//ジオメトリの初期化
	if( !m_Geometry)
	{
		m_Geometry = m_pGameObject->AddComponent<BuildingGeometry>();
	}
	m_Geometry->Init( land, rule);

	////交通プログラムの建物の初期化
	//if (!m_TrafficBuilding)
	//{
	//	m_TrafficBuilding = m_pGameObject->AddComponent<TrafficBuilding>();
	//}
	//m_TrafficBuilding->Init( roads, carManager);

	return true;
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void BuildingController::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	ジオメトリを確定させる
------------------------------------------------------------------------------*/
void BuildingController::ConfirmGeometry(void)
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

/*------------------------------------------------------------------------------
	管理クラスが削除される際の処理
------------------------------------------------------------------------------*/
void BuildingController::OnBuildingManagerDelete()
{
	m_BuildingManager = NULL;
	m_pGameObject->ReleaseReserve();
}

