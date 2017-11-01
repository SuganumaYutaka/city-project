/*==============================================================================
	
	TitleController. - タイトル処理
														Author : Yutaka Suganuma
														Date   : 2017/6/22
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "TitleController.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "TextureManager.h"

#include "InputKeyboard.h"

#include "ModeGame.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
TitleController::TitleController( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_pGameObject->m_Tag = "TitleController";

	m_pGameObject->AddComponent<SpriteRenderer>();
	m_pGameObject->GetComponent<Transform>()->SetLocalPosition( Vector3( SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	m_pGameObject->GetComponent<Transform>()->SetLocalScale( Vector3( SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f));
	m_pGameObject->GetComponent<SpriteRenderer>()->LoadTexture( "data/TEXTURE/title.png");

	auto pAudio = m_pGameObject->AddComponent<AudioSource>();
	pAudio->LoadSound( "data/BGM/title.wav", -1);		//BGM
	pAudio->Play(0);

	nCnt = 0;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void TitleController::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void TitleController::Update()
{
	if ( Manager::GetInputKeyboard()->GetKeyTrigger(DIK_RETURN))
	{
		Manager::SetMode( new ModeGame);
	}
}

/*------------------------------------------------------------------------------
	衝突時の処理
------------------------------------------------------------------------------*/
void TitleController::OnCollision( Collider *pCollider)
{
	
}

