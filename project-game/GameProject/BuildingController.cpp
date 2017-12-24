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

#include "BuildingRule.h"
#include "BuildingGeometry.h"
#include "CityAttribute.h"
#include "BuildingManager.h"

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
bool BuildingController::Init( const std::vector<Vector3>& vertices, BuildingRule* rule, std::list<RoadAttribute*> roads, BuildingManager* manager)
{
	m_BuildingManager = manager;
	m_BuildingManager->Register( this);
	
	//ジオメトリの初期化
	if( !m_Geometry)
	{
		m_Geometry = m_pGameObject->AddComponent<BuildingGeometry>();
	}
	bool hr = false;
	hr = m_Geometry->Init( vertices, rule);

	return hr;
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void BuildingController::Update( void)
{
	
}

