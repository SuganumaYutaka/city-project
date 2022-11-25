/*==============================================================================
	
	ResultController.cpp - リザルト処理
														Author : Yutaka Suganuma
														Date   : 2017/7/6
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "ResultController.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "TextureManager.h"

#include "InputKeyboard.h"

#include "ModeTitle.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
ResultController::ResultController( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_pGameObject->m_Tag = "ResultController";

	m_pGameObject->AddComponent<SpriteRenderer>();
	m_pGameObject->GetComponent<Transform>()->SetLocalPosition( Vector3( SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	m_pGameObject->GetComponent<Transform>()->SetLocalScale( Vector3( SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f));
	m_pGameObject->GetComponent<SpriteRenderer>()->LoadTexture( "data/TEXTURE/result.png");

	auto pAudio = m_pGameObject->AddComponent<AudioSource>();
	pAudio->LoadSound( "data/BGM/result.wav", -1);		//BGM
	pAudio->Play(0);

	nCnt = 0;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void ResultController::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void ResultController::Update()
{
	if ( Manager::GetInputKeyboard()->GetKeyTrigger(DIK_RETURN))
	{
		Manager::SetMode( new ModeTitle);
	}
}

/*------------------------------------------------------------------------------
	衝突時の処理
------------------------------------------------------------------------------*/
void ResultController::OnCollision( Collider *pCollider)
{
	
}

