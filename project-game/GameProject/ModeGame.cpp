/*==============================================================================

    ModeGame.cpp - �Q�[�����[�h
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/6/8
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "ModeGame.h"
#include "GameObject.h"
#include "Component.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "ComponentInclude.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
ModeGame::ModeGame()
{
	//�������Ȃ�
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
ModeGame::~ModeGame()
{

}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void ModeGame::Init()
{
	//�m�[�h�̐擪
	m_pRoot = new GameObject(NULL);
	
	//�I�u�W�F�N�g����

	//�J����
	GameObject *pCamera = new GameObject( m_pRoot);
	auto cameraComponent = pCamera->AddComponent<Camera>();
	pCamera->m_pTransform->SetLocalPosition( Vector3( 0.0f, 1.0f, -2.5f));
	Manager::GetRenderManager()->SetMainCamera( pCamera->GetComponent<Camera>());	//���C���J�����ɐݒ�
	cameraComponent->SetFar( 1200.0f);
	pCamera->AddComponent<CameraController>();

	//���z
	auto pSun = new GameObject( m_pRoot);
	pSun->AddComponent<Sun>();

	//�X�J�C�h�[��
	auto pSky = new GameObject( m_pRoot);
	auto meshDome = pSky->AddComponent<MeshDomeRenderer>();
	meshDome->m_nLayer = eLayerSky;
	meshDome->SetShader( eShaderSky);
	meshDome->LoadTexture( "data/TEXTURE/sky00.jpg");
	meshDome->SetSize( 1000.0f, 1000.0f);

	//�t�B�[���h
	auto pField = new GameObject( m_pRoot);
	auto meshField = pField->AddComponent<MeshField>();
	meshField->LoadTexture( "data/TEXTURE/field01.png");
	meshField->SetSize( 1500.0f, 1500.0f);
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void ModeGame::Update()
{
	//�Q�[���I�u�W�F�N�g�̍X�V
	m_pRoot->UpdateAll();
}

