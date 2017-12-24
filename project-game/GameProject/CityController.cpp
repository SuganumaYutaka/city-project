/*==============================================================================
	
	CityController.cpp - 町の自動生成ーコントローラ
														Author : Yutaka Suganuma
														Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "CityController.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "HalfEdgeModel.h"
#include "CityRule.h"
#include "CityAttribute.h"

#include "InputKeyboard.h"

#include "BuildingRuleFactory.h"
#include "BuildingManager.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define CITY_WIDTH (200.0f)
#define CITY_HEIGHT (200.0f)


/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* CityController::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<CityController>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
CityController::CityController( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	//建物管理オブジェクト生成
	m_BuildingManager = new BuildingManager();

	//建物の自動生成システムを生成
	m_BuildingRuleFactory = new BuildingRuleFactory();

	//町の自動生成システム設定のハーフエッジデータ構造を生成
	auto attributeFactory = new CityAttributeFactory( m_pGameObject, m_BuildingRuleFactory, m_BuildingManager);
	m_Model = new Model( new CityRule(), attributeFactory);

	//はじめの面を生成
	Vector3 sizeHalf( CITY_WIDTH * 0.5f, 0.0f, CITY_HEIGHT * 0.5f);
	m_Model->CreateFirstFace( 
		Vector3( -sizeHalf.x, 0.0f, +sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, +sizeHalf.z),
		Vector3( -sizeHalf.x, 0.0f, -sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, -sizeHalf.z));
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void CityController::Uninit( void)
{
	delete m_Model;
	delete m_BuildingRuleFactory;
	delete m_BuildingManager;
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void CityController::Update()
{
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		m_Model->DivideAllFaces();
	}
}
