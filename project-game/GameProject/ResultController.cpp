/*==============================================================================
	
	ResultController.cpp - ���U���g����
														Author : Yutaka Suganuma
														Date   : 2017/7/6
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "ResultController.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "TextureManager.h"

#include "InputKeyboard.h"

#include "ModeTitle.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�R���X�g���N�^
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
	�I������
------------------------------------------------------------------------------*/
void ResultController::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void ResultController::Update()
{
	if ( Manager::GetInputKeyboard()->GetKeyTrigger(DIK_RETURN))
	{
		Manager::SetMode( new ModeTitle);
	}
}

/*------------------------------------------------------------------------------
	�Փˎ��̏���
------------------------------------------------------------------------------*/
void ResultController::OnCollision( Collider *pCollider)
{
	
}
