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
	pCamera->m_pTransform->SetLocalPosition( Vector3( 0.0f, 100.0f, -150.0f));
	//pCamera->m_pTransform->SetLocalPosition( Vector3( 0.0f, 5.0f, -5.0f));
	cameraComponent->SetFar( 2000.0f);
	pCamera->AddComponent<CameraController>();

	//���C�g
	auto cameraObj = new GameObject( m_pRoot);
	cameraObj->AddComponent<Light>();
	cameraObj->m_pTransform->SetWorldRotationLookDirection( Vector3( 0.0f, -1.0f, 1.0f));

	//�X�J�C�h�[��
	auto pSky = new GameObject( m_pRoot);
	auto meshDome = pSky->AddComponent<MeshDomeRenderer>();
	meshDome->SetDome( 30, 10, 220.0f, 220.0f);
	meshDome->m_nLayer = eLayerSky;
	meshDome->SetShader( eShaderSky);
	meshDome->LoadTexture( "data/TEXTURE/sky01.jpg");
	
	//��
	auto groundObject = new GameObject( m_pRoot);
	auto grountRanderer = groundObject->AddComponent<Polygon3DRenderer>();
	grountRanderer->LoadTexture("data/TEXTURE/black.jpg");
	groundObject->m_pTransform->SetLocalPositionY( -5.0f);
	groundObject->m_pTransform->SetLocalScale( 500.0f, 1.0f, 500.0f);

	//City
	auto cityObject = new GameObject( m_pRoot);
	auto cityController = cityObject->AddComponent<CityController>();

	auto carObject = new GameObject( m_pRoot);
	//auto carRenderer = carObject->AddComponent<XModelRenderer>();
	//carRenderer->LoadXModel( "data/MODEL/car.x");
	
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void ModeGame::Update()
{
	//�Q�[���I�u�W�F�N�g�̍X�V
	m_pRoot->UpdateAll();
}

