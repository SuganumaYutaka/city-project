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

	//街全体の大きさ
	Vector2 citySize( 350.0f, 350.0f);

	//カメラ
	GameObject *pCamera = new GameObject( m_pRoot);
	auto cameraComponent = pCamera->AddComponent<Camera>();
	pCamera->m_pTransform->SetLocalPosition( Vector3( 0.0f, 150.0f, citySize.y * -0.8f));
	cameraComponent->SetFar( 2000.0f);
	pCamera->AddComponent<CameraController>();

	//ライト
	auto cameraObj = new GameObject( m_pRoot);
	cameraObj->AddComponent<Light>();
	cameraObj->m_pTransform->SetWorldRotationLookDirection( Vector3( 0.0f, -1.0f, 1.0f));

	//スカイドーム
	auto pSky = new GameObject( m_pRoot);
	auto meshDome = pSky->AddComponent<MeshDomeRenderer>();
	meshDome->SetDome( 30, 10, citySize.x * 1.1f, citySize.y * 1.1f);
	meshDome->m_nLayer = eLayerSky;
	meshDome->SetShader( eShaderSky);
	meshDome->LoadTexture( "data/TEXTURE/sky01.jpg");
	
	//床
	auto groundObject = new GameObject( m_pRoot);
	auto grountRanderer = groundObject->AddComponent<Polygon3DRenderer>();
	grountRanderer->LoadTexture("data/TEXTURE/black.jpg");
	groundObject->m_pTransform->SetLocalPositionY( -5.0f);
	groundObject->m_pTransform->SetLocalScale( citySize.x * 3.0f, 1.0f, citySize.y * 3.0f);

	//City
	auto cityObject = new GameObject( m_pRoot);
	auto cityController = cityObject->AddComponent<CityController>();
	//cityController->Init( citySize.x, citySize.y, 4, 40, true);
	cityController->Init( citySize.x, citySize.y, 0, 40, false);

	////shadowtest
	//auto sunObj = new GameObject( m_pRoot);
	//auto sun = sunObj->AddComponent<Sun>();
	//sunObj->m_pTransform->SetWorldPosition( Vector3( 250.0f, 250.0f, 250.0f));
	//sunObj->m_pTransform->SetWorldRotationLookAt( Vector3(0.0f, 0.0f, 0.0f));
	//
	//auto castObj = new GameObject( m_pRoot);
	//auto box = castObj->AddComponent<MeshBoxRenderer>();
	//box->LoadTexture( "data/TEXTURE/bill02.png");
	//castObj->m_pTransform->SetLocalScale( 50.0f, 200.0f, 50.0f);

	//auto groundObject = new GameObject( m_pRoot);
	//auto groundRenderer = groundObject->AddComponent<MeshField>();
	//groundRenderer->SetSize( 500.0f, 500.0f);

	//auto spriteObj = new GameObject( m_pRoot);
	//auto sprite = spriteObj->AddComponent<SpriteRenderer>();
	//sprite->LoadTexture("shadow");
	//Vector2 size = Vector2( 500.0f, 500.0f);
	//spriteObj->m_pTransform->SetLocalPosition( SCREEN_WIDTH - size.x * 0.5f, size.y * 0.5f, 0.0f);
	//spriteObj->m_pTransform->SetLocalScale( size.x, size.y, 1.0f);
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void ModeGame::Update()
{
	//ゲームオブジェクトの更新
	m_pRoot->UpdateAll();
}

