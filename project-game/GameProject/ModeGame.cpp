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

#ifdef _DEBUG
	//m_pRoot->AddComponent<EditSystem>();
#endif // _DEBUG

	//�J����
	GameObject *pCamera = new GameObject( m_pRoot);
	auto cameraComponent = pCamera->AddComponent<Camera>();
	//pCamera->m_pTransform->SetLocalPosition( Vector3( 0.0f, 400.0f, -70.0f));
	pCamera->m_pTransform->SetLocalPosition( Vector3( 0.0f, 5.0f, -5.0f));
	cameraComponent->SetFar( 2000.0f);
	pCamera->AddComponent<CameraController>();

	//�X�J�C�h�[��
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
	�X�V
------------------------------------------------------------------------------*/
void ModeGame::Update()
{
	//�Q�[���I�u�W�F�N�g�̍X�V
	m_pRoot->UpdateAll();
}

