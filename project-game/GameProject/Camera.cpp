/*==============================================================================

    Camera.cpp - �J����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "RenderTexture.h"
#include "ShaderManager.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define TEXTURE_WIDTH (1024)
#define TEXTURE_HEIGHT (1024)

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Camera::Camera( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_PosAt = Vector3();
	m_VecUp = Vector3( 0.0f, 1.0f, 0.0f);
	m_fFovY = D3DX_PI / 3.0f;
	m_fAspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	m_fNear = 0.1f;
	m_fFar = 100.0f;
	m_Type = eCameraDefault;

	////�r���[���W�ϊ��s��쐬
	D3DXVECTOR3 PosEye = m_pTransform->GetWorldPosition().ConvertToDX();
	D3DXVECTOR3 PosAt = m_PosAt.ConvertToDX();
	D3DXVECTOR3 VecUp = m_VecUp.ConvertToDX();

	D3DXMatrixLookAtLH( &m_mtxView, &PosEye, &PosAt, &VecUp);

	//�v���W�F�N�V�������W�ϊ��s��쐬
	D3DXMatrixPerspectiveFovLH( 
		&m_mtxProj,								//�v���W�F�N�V�������W�ϊ��s��|�C���^
		m_fFovY,								//��p�i����p�j
		m_fAspect,								//�A�X�y�N�g��
		m_fNear,								//near	�� 0.0f < near
		m_fFar);								//far

	//�r���[�|�[�g�ݒ�
	m_ViewPort = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f};

	//�����_�[�^�[�Q�b�g�i�f�t�H���g�̓o�b�N�o�b�t�@�j
	m_RenderTarget = Manager::GetTextureManager()->CreateRenderTexture( "BuckBuffer", true);

	//�`�悷�郌�C���[�i�f�t�H���g�͂��ׂĕ`��j
	m_listRenderLayer.clear();

	//�J�������}�l�[�W���[�ɒǉ�
	Manager::GetRenderManager()->SetCamera( this);
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void Camera::Uninit( void)
{
	//�}�l�[�W���[����폜
	Manager::GetRenderManager()->ReleaseCamera( this);
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void Camera::Update()
{
	////�r���[���W�ϊ��s��쐬
	D3DXVECTOR3 PosEye = m_pTransform->GetWorldPosition().ConvertToDX();
	D3DXVECTOR3 PosAt = m_PosAt.ConvertToDX();
	D3DXVECTOR3 VecUp = m_VecUp.ConvertToDX();

	D3DXMatrixLookAtLH( &m_mtxView, &PosEye, &PosAt, &VecUp);
}

/*------------------------------------------------------------------------------
	�J�����������擾
------------------------------------------------------------------------------*/
Vector3 Camera::GetCameraVec( void) const
{
	 return ( m_PosAt - m_pTransform->GetWorldPosition()).Normalize();
}

/*------------------------------------------------------------------------------
	�J�������s�ړ�
------------------------------------------------------------------------------*/
void Camera::Move( const Vector3& Value)
{
	m_PosAt += Value;
	m_pTransform->Move( Value);
}

/*------------------------------------------------------------------------------
	�����_�[�^�[�Q�b�g�̐ݒ�
------------------------------------------------------------------------------*/
void Camera::SetRenderTarget(std::string TextureName, bool isBuckBuffer)
{
	m_RenderTarget = Manager::GetTextureManager()->CreateRenderTexture( TextureName, isBuckBuffer);

	if (isBuckBuffer == false)
	{
		//�v���W�F�N�V�������W�ϊ�
		m_fAspect = (float)TEXTURE_WIDTH / TEXTURE_HEIGHT;
		m_fNear = 8.0f;
		m_fFar = 100.0f;
		D3DXMatrixPerspectiveFovLH( &m_mtxProj, m_fFovY, m_fAspect, m_fNear, m_fFar);

		//�r���[�|�[�g�ϊ�
		SetViewPort( 0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 1.0f);
	}
}

/*------------------------------------------------------------------------------
	�J�����̐ݒ�i�r���[�|�[�g�A�����_�[�^�[�Q�b�g�A�[�x�o�b�t�@�j
------------------------------------------------------------------------------*/
void Camera::SetCamera(void)
{
	auto pDevice = Manager::GetDevice();

	//�J�����̐ݒ�
	pDevice->SetViewport( &m_ViewPort);
	pDevice->SetRenderTarget( 0, m_RenderTarget->GetRenderTarget());
	pDevice->SetDepthStencilSurface( m_RenderTarget->GetDepthBuffer());

	//�����_�[�^�[�Q�b�g�̏�����
	if (m_Type == eCameraLight)
	{
		pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 255, 255, 255, 255), 1.0f, 0);
		//pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 0, 0, 0, 255), 1.0f, 0);
	}
	else
	{
		pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 255, 255, 255, 255), 1.0f, 0);
	}

	//�V�F�[�_�[�̐ݒ�
	if (m_Type == eCameraDefault)
	{
		Manager::GetShaderManager()->SetDefault();
	}
	if (m_Type == eCameraLight)
	{
		Manager::GetShaderManager()->SetDepth();
	}
}

/*------------------------------------------------------------------------------
	�j�A�̐ݒ�
------------------------------------------------------------------------------*/
void Camera::SetNear(float Near)
{
	m_fNear = Near;
	D3DXMatrixPerspectiveFovLH( &m_mtxProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
}

/*------------------------------------------------------------------------------
	�t�@�[�̐ݒ�
------------------------------------------------------------------------------*/
void Camera::SetFar(float Far)
{
	m_fFar = Far;
	D3DXMatrixPerspectiveFovLH( &m_mtxProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
}
