/*==============================================================================

    ModeTitle.cpp - �^�C�g�����[�h
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/6/22
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "ModeTitle.h"
#include "GameObject.h"
#include "Component.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "ComponentInclude.h"

#include "TitleController.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
ModeTitle::ModeTitle()
{
	//�������Ȃ�
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
ModeTitle::~ModeTitle()
{
	
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void ModeTitle::Init()
{
	//�m�[�h�̐擪
	m_pRoot = new GameObject( NULL);
	
	//�I�u�W�F�N�g����
	GameObject *pObj;

	//�^�C�g��
	pObj = new GameObject(m_pRoot);
	pObj->AddComponent<TitleController>();

	//�J����
	GameObject *pCamera = new GameObject( m_pRoot);
	pCamera->AddComponent<Camera>();
	Manager::GetRenderManager()->SetMainCamera( pCamera->GetComponent<Camera>());	//���C���J�����ɐݒ�
	pCamera->GetComponent<Transform>()->SetLocalPosition( Vector3( 0.0f, 2.5f, -2.5f));
	pCamera->AddComponent<CameraController>();

	//���C�g
	GameObject *pLight = new GameObject( m_pRoot);
	pLight->AddComponent<Light>();
	pLight->GetComponent<Transform>()->SetLocalPosition( Vector3( 2.0f, 1.5f, -1.0f));

	//���͎w��
	pObj = new GameObject( m_pRoot);
	auto renderer = pObj->AddComponent<SpriteRenderer>();
	renderer->LoadTexture( "data/TEXTURE/press_enter.png");
	pObj->m_pTransform->SetLocalPosition( Vector3( SCREEN_WIDTH * 0.5f, 600.0f, 0.0f));
	pObj->m_pTransform->SetLocalScale( Vector3( 500.0f, 75.0f, 0.0f));
	
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void ModeTitle::Update()
{
	//�Q�[���I�u�W�F�N�g�̍X�V
	m_pRoot->UpdateAll();
}

