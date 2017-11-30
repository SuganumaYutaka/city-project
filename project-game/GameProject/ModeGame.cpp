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

	//ロード
	/*Text loadText;
	loadText.Load("data/SCRIPT/ModeGame.txt");
	m_pRoot->Load(loadText);*/

#ifdef _DEBUG
	m_pRoot->AddComponent<EditSystem>();
#endif // _DEBUG

	
	//オブジェクト生成
	GameObject* pTest = new GameObject( m_pRoot);
	pTest->m_pTransform->SetLocalPosition(1.0f, 2.0f, 3.0f);
	pTest->m_pTransform->SetLocalScale(1.0f, 2.0f, 3.0f);
	pTest->m_pTransform->SetLocalRotationEuler(1.0f, 2.0f, 3.0f);

	//カメラ
	GameObject *pCamera = new GameObject( m_pRoot);
	auto cameraComponent = pCamera->AddComponent<Camera>();
	pCamera->m_pTransform->SetLocalPosition( Vector3( 0.0f, 30.0f, -50.0f));
	cameraComponent->SetFar( 2000.0f);
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

	//ビル
	GameObject* pBill = NULL;
	MeshBoxRenderer* boxRenderer = NULL;
	BoxCollider* boxCollider = NULL;
	Vector3 size;
	for (int nCntZ = 0; nCntZ < 5; nCntZ++)
	{
		for (int nCntX = 0; nCntX < 5; nCntX++)
		{
			pBill = new GameObject( m_pRoot);
			boxCollider = pBill->AddComponent<BoxCollider>();
			boxCollider->m_IsTrigger = false;
			boxRenderer = pBill->AddComponent<MeshBoxRenderer>();
			boxRenderer->LoadTexture("data/TEXTURE/bill02.png");
			size = Vector3( 40.0f, 100.0f, 40.0f);
			//pBill->m_pTransform->SetLocalPosition( -680.0f + nCntX * 187.0f, size.y * 0.5f, -680.0f + nCntZ * 187.0f);
			pBill->m_pTransform->SetLocalPosition( -680.0f + nCntX * 187.0f, 0.0f, -680.0f + nCntZ * 187.0f);
			boxCollider->SetCenter( Vector3( 0.0f, size.y * 0.5f, 0.0f));
			boxRenderer->SetCenter( Vector3( 0.0f, size.y * 0.5f, 0.0f));
			boxCollider->SetSize(size);
			boxRenderer->SetSize(size);

			pBill = new GameObject( m_pRoot);
			boxCollider = pBill->AddComponent<BoxCollider>();
			boxCollider->m_IsTrigger = false;
			boxRenderer = pBill->AddComponent<MeshBoxRenderer>();
			boxRenderer->LoadTexture("data/TEXTURE/bill02.png");
			size = Vector3( 37.0f, 115.0f, 37.0f);
			//pBill->m_pTransform->SetLocalPosition( -680.0f + nCntX * 187.0f, size.y * 0.5f, -635.0f + nCntZ * 187.0f);
			pBill->m_pTransform->SetLocalPosition( -680.0f + nCntX * 187.0f, 0.0f, -635.0f + nCntZ * 187.0f);
			boxCollider->SetCenter( Vector3( 0.0f, size.y * 0.5f, 0.0f));
			boxRenderer->SetCenter( Vector3( 0.0f, size.y * 0.5f, 0.0f));
			boxCollider->SetSize(size);
			boxRenderer->SetSize(size);

			pBill = new GameObject( m_pRoot);
			boxCollider = pBill->AddComponent<BoxCollider>();
			boxCollider->m_IsTrigger = false;
			boxRenderer = pBill->AddComponent<MeshBoxRenderer>();
			boxRenderer->LoadTexture("data/TEXTURE/bill02.png");
			size = Vector3( 50.0f, 160.0f, 50.0f);
			//pBill->m_pTransform->SetLocalPosition( -635.0f + nCntX * 187.0f, size.y * 0.5f, -680.0f + nCntZ * 187.0f);
			pBill->m_pTransform->SetLocalPosition( -635.0f + nCntX * 187.0f, 0.0f, -680.0f + nCntZ * 187.0f);
			boxCollider->SetCenter( Vector3( 0.0f, size.y * 0.5f, 0.0f));
			boxRenderer->SetCenter( Vector3( 0.0f, size.y * 0.5f, 0.0f));
			boxCollider->SetSize(size);
			boxRenderer->SetSize(size);

			pBill = new GameObject( m_pRoot);
			boxCollider = pBill->AddComponent<BoxCollider>();
			boxCollider->m_IsTrigger = false;
			boxRenderer = pBill->AddComponent<MeshBoxRenderer>();
			boxRenderer->LoadTexture("data/TEXTURE/bill02.png");
			size = Vector3( 45.0f, 130.0f, 45.0f);
			//pBill->m_pTransform->SetLocalPosition( -635.0f + nCntX * 187.0f, size.y * 0.5f, -630.0f + nCntZ * 187.0f);
			pBill->m_pTransform->SetLocalPosition( -635.0f + nCntX * 187.0f, 0.0f, -630.0f + nCntZ * 187.0f);
			boxCollider->SetCenter( Vector3( 0.0f, size.y * 0.5f, 0.0f));
			boxRenderer->SetCenter( Vector3( 0.0f, size.y * 0.5f, 0.0f));
			boxCollider->SetSize(size);
			boxRenderer->SetSize(size);
		}
	}

	auto pPlayer = new GameObject( m_pRoot);
	auto playerModel = pPlayer->AddComponent<PartsAnimator>();
	playerModel->LoadAnimator( "data/MOTION/player.txt");
	auto col = pPlayer->AddComponent<BoxCollider>();
	size = Vector3( 30.0f, 75.0f, 30.0f);
	col->SetSize( size);
	col->SetCenter( Vector3(0.0f, 37.5f, 0.0f));

	////テストーレンダーターゲット
	//auto secondCamera = new GameObject( pCamera);
	//cameraComponent = secondCamera->AddComponent<Camera>();
	//cameraComponent->SetRenderTarget("test", false);
	//cameraComponent->SetFar( 2000.0f);
	//auto pSprite = new GameObject( m_pRoot);
	//auto sprite = pSprite->AddComponent<SpriteRenderer>();
	//sprite->LoadTexture("test");
	//pSprite->m_pTransform->SetLocalScale( 300.0f, 300.0f, 1.0f);
	
	//セーブ
	Text saveText;
	m_pRoot->Save( saveText);
	saveText.Save("data/SCRIPT/ModeGame.txt");
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void ModeGame::Update()
{
	//ゲームオブジェクトの更新
	m_pRoot->UpdateAll();
}

