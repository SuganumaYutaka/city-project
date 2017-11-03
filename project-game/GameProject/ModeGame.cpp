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
	Vector3 size;
	for (int nCntZ = 0; nCntZ < 8; nCntZ++)
	{
		for (int nCntX = 0; nCntX < 8; nCntX++)
		{
			pBill = new GameObject( m_pRoot);
			boxRenderer = pBill->AddComponent<MeshBoxRenderer>();
			boxRenderer->LoadTexture("data/TEXTURE/bill02.png");
			size = Vector3( 40.0f, 100.0f, 40.0f);
			pBill->m_pTransform->SetLocalPosition( -680.0f + nCntX * 187.0f, size.y * 0.5f, -680.0f + nCntZ * 187.0f);
			pBill->m_pTransform->SetLocalScale(size);

			pBill = new GameObject( m_pRoot);
			boxRenderer = pBill->AddComponent<MeshBoxRenderer>();
			boxRenderer->LoadTexture("data/TEXTURE/bill02.png");
			size = Vector3( 37.0f, 115.0f, 37.0f);
			pBill->m_pTransform->SetLocalPosition( -680.0f + nCntX * 187.0f, size.y * 0.5f, -635.0f + nCntZ * 187.0f);
			pBill->m_pTransform->SetLocalScale(size);

			pBill = new GameObject( m_pRoot);
			boxRenderer = pBill->AddComponent<MeshBoxRenderer>();
			boxRenderer->LoadTexture("data/TEXTURE/bill02.png");
			size = Vector3( 50.0f, 160.0f, 50.0f);
			pBill->m_pTransform->SetLocalPosition( -635.0f + nCntX * 187.0f, size.y * 0.5f, -680.0f + nCntZ * 187.0f);
			pBill->m_pTransform->SetLocalScale(size);

			pBill = new GameObject( m_pRoot);
			boxRenderer = pBill->AddComponent<MeshBoxRenderer>();
			boxRenderer->LoadTexture("data/TEXTURE/bill02.png");
			size = Vector3( 45.0f, 130.0f, 45.0f);
			pBill->m_pTransform->SetLocalPosition( -635.0f + nCntX * 187.0f, size.y * 0.5f, -630.0f + nCntZ * 187.0f);
			pBill->m_pTransform->SetLocalScale(size);
		}
	}

	auto pPlayer = new GameObject( m_pRoot);
	auto playerModel = pPlayer->AddComponent<PartsAnimator>();
	playerModel->Load( "data/MOTION/player.txt");
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void ModeGame::Update()
{
	//ゲームオブジェクトの更新
	m_pRoot->UpdateAll();
}

