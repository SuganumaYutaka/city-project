/*==============================================================================
	
	TitleController. - �^�C�g������
														Author : Yutaka Suganuma
														Date   : 2017/6/22
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "TitleController.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "TextureManager.h"

#include "InputKeyboard.h"

#include "ModeGame.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�R���X�g���N�^
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
	�I������
------------------------------------------------------------------------------*/
void TitleController::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void TitleController::Update()
{
	if ( Manager::GetInputKeyboard()->GetKeyTrigger(DIK_RETURN))
	{
		Manager::SetMode( new ModeGame);
	}
}

/*------------------------------------------------------------------------------
	�Փˎ��̏���
------------------------------------------------------------------------------*/
void TitleController::OnCollision( Collider *pCollider)
{
	
}

