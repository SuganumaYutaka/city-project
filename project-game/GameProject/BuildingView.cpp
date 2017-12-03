/*==============================================================================

    BuildingView.cpp - 町の自動生成ー建物ビュー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/3
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BuildingView.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "Building.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* BuildingView::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BuildingView>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
BuildingView::BuildingView( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Building = NULL;

	//レンダラーの設定
	m_Renderer = m_pGameObject->AddComponent<MeshPolygonRenderer>();
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void BuildingView::Uninit( void)
{
	m_Building->UnregisterView();
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void BuildingView::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	建物情報の設定
------------------------------------------------------------------------------*/
void BuildingView::SetBuilding( Building* building)
{ 
	if( m_Building)
	{	
		return;
	}
	
	m_Building = building;

	//View情報の更新
	m_Renderer->SetVertices( m_Building->GetVertices());
}

