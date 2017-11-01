/*==============================================================================

    ModeResult.cpp - リザルトモード
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/6
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "ModeResult.h"
#include "GameObject.h"
#include "Component.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "ComponentInclude.h"

#include "ResultController.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
ModeResult::ModeResult()
{
	//何もしない
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
ModeResult::~ModeResult()
{
	
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void ModeResult::Init()
{
	//ノードの先頭
	m_pRoot = new GameObject( NULL);
	
	//オブジェクト生成
	GameObject *pObj;

	//タイトル
	pObj = new GameObject(m_pRoot);
	pObj->AddComponent<ResultController>();

	//カメラ
	GameObject *pCamera = new GameObject( m_pRoot);
	pCamera->AddComponent<Camera>();
	Manager::GetRenderManager()->SetMainCamera( pCamera->GetComponent<Camera>());	//メインカメラに設定
	pCamera->GetComponent<Transform>()->SetLocalPosition( Vector3( 0.0f, 2.5f, -2.5f));
	pCamera->AddComponent<CameraController>();

	//ライト
	GameObject *pLight = new GameObject( m_pRoot);
	pLight->AddComponent<Light>();
	pLight->GetComponent<Transform>()->SetLocalPosition( Vector3( 2.0f, 1.5f, -1.0f));

	//入力指示
	pObj = new GameObject( m_pRoot);
	auto renderer = pObj->AddComponent<SpriteRenderer>();
	renderer->LoadTexture( "data/TEXTURE/press_enter.png");
	pObj->m_pTransform->SetLocalPosition( Vector3( SCREEN_WIDTH * 0.5f, 600.0f, 0.0f));
	pObj->m_pTransform->SetLocalScale( Vector3( 500.0f, 75.0f, 0.0f));
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void ModeResult::Update()
{
	//ゲームオブジェクトの更新
	m_pRoot->UpdateAll();
}

