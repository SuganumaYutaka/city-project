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
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* Camera::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<Camera>();
}

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
	���C���J�������擾
------------------------------------------------------------------------------*/
Camera* Camera::GetMainCamera()
{
	return Manager::GetRenderManager()->GetMainCamera();
}

/*------------------------------------------------------------------------------
	���C���J������ݒ�
------------------------------------------------------------------------------*/
void Camera::SetMainCamera()
{
	Manager::GetRenderManager()->SetMainCamera( this);
}

/*------------------------------------------------------------------------------
	���C���J�������ǂ���
------------------------------------------------------------------------------*/
bool Camera::IsMainCamera()
{
	if ( this == Manager::GetRenderManager()->GetMainCamera())
	{
		return true;
	}

	return false;
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
		auto size = m_RenderTarget->GetTextureSize();

		//�v���W�F�N�V�������W�ϊ�
		m_fAspect = size.x / size.y;
		D3DXMatrixPerspectiveFovLH( &m_mtxProj, m_fFovY, m_fAspect, m_fNear, m_fFar);

		//�r���[�|�[�g�ϊ�
		SetViewPort( 0, 0, (int)size.x, (int)size.y, 0.0f, 1.0f);
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
	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 255, 255, 255, 255), 1.0f, 0);
	//pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 0, 0, 0, 0), 1.0f, 0);

	//�V�F�[�_�[�̐ݒ�
	if (m_Type == eCameraDefault)
	{
		Manager::GetShaderManager()->SetDefault();
	}
	else if (m_Type == eCameraLight)
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

/*------------------------------------------------------------------------------
	�v���W�F�N�V�����s��̐ݒ�iPerspective�j
------------------------------------------------------------------------------*/
void Camera::SetPerspective( float FovY, float Aspect, float Near, float Far)
{
	m_fFovY = FovY;
	m_fAspect = Aspect;
	m_fNear = Near;
	m_fFar = Far;

	D3DXMatrixPerspectiveFovLH( &m_mtxProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
}

/*------------------------------------------------------------------------------
	�v���W�F�N�V�����s��̐ݒ�iPerspective�j
------------------------------------------------------------------------------*/
void Camera::SetOrtho( float Width, float Height, float Near, float Far)
{
	m_fNear = Near;
	m_fFar = Far;
	D3DXMatrixOrthoLH( &m_mtxProj, Width, Height, Near, Far);
}

/*------------------------------------------------------------------------------
	�X�N���[�����W���烌�C�i�����j���Z�o
------------------------------------------------------------------------------*/
Ray Camera::CalcScreenPointToRay(Vector2 screenPoint)
{
	//�X�N���[�����W���烏�[���h���W�ɕϊ�
	Vector3 nearPos = CalcScreenPointToWorldPosition( screenPoint, 0.0f);		//�J��������ł��߂����W
	Vector3 farPos = CalcScreenPointToWorldPosition( screenPoint, 1.0f);		//�J��������ł��������W

	//�J�[�\���̐����Z�o
	Ray ray;
	ray.Direction = (farPos - nearPos).Normalize();		//�J�[�\���̐���
	ray.Position = m_pTransform->GetWorldPosition();

	return ray;
}

/*------------------------------------------------------------------------------
	�X�N���[�����W���烏�[���h���W���Z�o
------------------------------------------------------------------------------*/
Vector3 Camera::CalcScreenPointToWorldPosition( Vector2 screenPoint, float positionZ)
{
	//�ϐ��錾
	D3DXMATRIX mtxWorld;				//���[���h���W�ϊ��s��
	D3DXVECTOR3 worldPos;				//���[���h���W

	//�t�s���ݒ�
	D3DXMATRIX mtxViewInv;		//�r���[���W�ϊ��s��̋t�s��
	D3DXMATRIX mtxProjInv;		//�v���W�F�N�V�������W�ϊ��s��̋t�s��
	D3DXMatrixInverse( &mtxViewInv, NULL, &m_mtxView);
	D3DXMatrixInverse( &mtxProjInv, NULL, &m_mtxProj);
	
	//�r���[�|�[�g�i�E�����j
	D3DXMATRIX VP, InvViewport;
	D3DXMatrixIdentity( &VP );
	VP._11 = SCREEN_WIDTH / 2.0f;
	VP._22 = -SCREEN_HEIGHT / 2.0f;
	VP._41 = SCREEN_WIDTH / 2.0f;
	VP._42 = SCREEN_HEIGHT / 2.0f;
	D3DXMatrixInverse( &InvViewport, NULL, &VP );

	//�t�s��������Ă������[���h���W�ϊ��s��ɕϊ�
	D3DXMatrixIdentity( &mtxWorld);		//�P�ʍs��
	mtxWorld = mtxWorld * InvViewport * mtxProjInv * mtxViewInv;
	
	//���[���h���W���Z�o
	D3DXVec3TransformCoord( &worldPos, &D3DXVECTOR3( screenPoint.x, screenPoint.y, positionZ), &mtxWorld);

	return worldPos;
}

/*------------------------------------------------------------------------------
	���C�g�J�����p�̐ݒ�
------------------------------------------------------------------------------*/
bool Camera::ChangeLightCameraFormat(Vector3 fieldSize)
{
	//�J�����̎�ނ̐ݒ�
	SetType( eCameraLight);

	//�`�惌�C���[�̐ݒ�
	SetRenderLayer( eLayerBack);
	SetRenderLayer( eLayerDefault);
	
	//�����_�[�^�[�Q�b�g�̐ݒ�
	SetRenderTarget( "shadow", false);
	m_RenderTarget->ChangeDepthRenderFormat();

	//Ortho�̐ݒ�
	float length = max( fieldSize.x, fieldSize.z);
	SetOrtho( length, length, 0.1f, length);

	//�r���[�|�[�g�̐ݒ�
	auto textureSize = m_RenderTarget->GetTextureSize();
	SetViewPort( 0, 0, (int)textureSize.x, (int)textureSize.y, 0.0f, 1.0f);

	//far�̐ݒ�
	m_fFar = length * 2.0f;

	return true;
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void Camera::Load(Text& text)
{
	//text��ǂݐi�߂�
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "PosAt")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_PosAt.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}
		else if (text.GetWord() == "VecUp")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_VecUp.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}
		else if (text.GetWord() == "FovY")
		{
			text.ForwardPositionToNextWord();
			m_fFovY = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Aspect")
		{
			text.ForwardPositionToNextWord();
			m_fAspect = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Near")
		{
			text.ForwardPositionToNextWord();
			m_fNear = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Far")
		{
			text.ForwardPositionToNextWord();
			m_fFar = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "RenderLayer")
		{
			text.ForwardPositionToNextWord();
			int size = std::stoi(text.GetWord());
			for( int nCnt = 0; nCnt < size; nCnt++)
			{
				text.ForwardPositionToNextWord();
				m_listRenderLayer.push_back(std::stoi(text.GetWord()));
			}
		}

		//text��ǂݐi�߂�
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

	//�r���[���W�ϊ��s��쐬
	D3DXVECTOR3 PosEye = m_pTransform->GetWorldPosition().ConvertToDX();
	D3DXVECTOR3 PosAt = m_PosAt.ConvertToDX();
	D3DXVECTOR3 VecUp = m_VecUp.ConvertToDX();
	D3DXMatrixLookAtLH( &m_mtxView, &PosEye, &PosAt, &VecUp);

	//�v���W�F�N�V�������W�ϊ��s��쐬
	SetPerspective( m_fFovY, m_fAspect, m_fNear, m_fFar);
}

/*------------------------------------------------------------------------------
	�Z�[�u
------------------------------------------------------------------------------*/
void Camera::Save(Text& text)
{
	StartSave(text);

	text += "PosAt " + m_PosAt.ConvertToString() + '\n';
	text += "VecUp " + m_VecUp.ConvertToString() + '\n';
	text += "FovY " + std::to_string(m_fFovY) + '\n';
	text += "Aspect " + std::to_string(m_fAspect) + '\n';
	text += "Near " + std::to_string(m_fNear) + '\n';
	text += "Far " + std::to_string(m_fFar) + '\n';

	int size = m_listRenderLayer.size();
	if( size > 0)
	{
		text += "RenderLayer " + std::to_string(size) + '\n';
		for( auto ite = m_listRenderLayer.begin(); ite != m_listRenderLayer.end(); ++ite)
		{
			text += std::to_string(*ite) + '\n';
		}
	}

	EndSave( text);
}

