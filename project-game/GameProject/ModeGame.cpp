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

#ifdef _DEBUG
	//m_pRoot->AddComponent<EditSystem>();
#endif // _DEBUG

	//カメラ
	GameObject *pCamera = new GameObject( m_pRoot);
	auto cameraComponent = pCamera->AddComponent<Camera>();
	//pCamera->m_pTransform->SetLocalPosition( Vector3( 0.0f, 400.0f, -70.0f));
	pCamera->m_pTransform->SetLocalPosition( Vector3( 0.0f, 5.0f, -5.0f));
	cameraComponent->SetFar( 2000.0f);
	pCamera->AddComponent<CameraController>();

	//スカイドーム
	auto pSky = new GameObject( m_pRoot);
	auto meshDome = pSky->AddComponent<MeshDomeRenderer>();
	meshDome->m_nLayer = eLayerSky;
	meshDome->SetShader( eShaderSky);
	meshDome->LoadTexture( "data/TEXTURE/sky00.jpg");
	meshDome->SetSize( 1000.0f, 1000.0f);

	//City
	//auto cityObject = new GameObject( m_pRoot);
	//auto cityController = cityObject->AddComponent<CityController>();

	//Building
	
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void ModeGame::Update()
{
	//ゲームオブジェクトの更新
	m_pRoot->UpdateAll();
}

