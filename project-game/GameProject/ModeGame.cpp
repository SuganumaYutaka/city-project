/*==============================================================================

    ModeGame.cpp - ゲームモード
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/6/8
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "ModeGame.h"
#include "GameObject.h"
#include "Component.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "ComponentInclude.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
ModeGame::ModeGame()
{
	//何もしない
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
ModeGame::~ModeGame()
{

}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void ModeGame::Init()
{
	//ノードの先頭
	m_pRoot = new GameObject(NULL);
	
	//オブジェクト生成

	//カメラ
	GameObject *pCamera = new GameObject( m_pRoot);
	auto cameraComponent = pCamera->AddComponent<Camera>();
	pCamera->m_pTransform->SetLocalPosition( Vector3( 0.0f, 1.0f, -2.5f));
	Manager::GetRenderManager()->SetMainCamera( pCamera->GetComponent<Camera>());	//メインカメラに設定
	cameraComponent->SetFar( 1200.0f);
	pCamera->AddComponent<CameraController>();

	//太陽
	auto pSun = new GameObject( m_pRoot);
	pSun->AddComponent<Sun>();

	//スカイドーム
	auto pSky = new GameObject( m_pRoot);
	auto meshDome = pSky->AddComponent<MeshDomeRenderer>();
	meshDome->m_nLayer = eLayerSky;
	meshDome->SetShader( eShaderSky);
	meshDome->LoadTexture( "data/TEXTURE/sky00.jpg");
	meshDome->SetSize( 1000.0f, 1000.0f);

	//フィールド
	auto pField = new GameObject( m_pRoot);
	auto meshField = pField->AddComponent<MeshField>();
	meshField->LoadTexture( "data/TEXTURE/field01.png");
	meshField->SetSize( 1500.0f, 1500.0f);
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void ModeGame::Update()
{
	//ゲームオブジェクトの更新
	m_pRoot->UpdateAll();
}

