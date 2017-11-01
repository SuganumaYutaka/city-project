/*==============================================================================

    ShadowVLShader.cpp - �e�t�����_���C�e�B���O�V�F�[�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/15
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "ShadowVLShader.h"
#include "Renderer.h"
#include "ComponentInclude.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"
#include "TextureManager.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
ShadowVLShader::ShadowVLShader()
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//�f�o�C�X�̃|�C���^
	
	//���_�錾�i�[�C���^�[�t�F�C�X�쐬
	D3DVERTEXELEMENT9 g_Dec1[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration( g_Dec1, &m_pVertexDec);

	//�V�F�[�_�̓ǂݍ���
	HRESULT hr;
	ID3DXBuffer *pError;		//�R���p�C���G���[���i�[�o�b�t�@
	hr = D3DXCreateEffectFromFile(pDevice, "data/SHADER/ShadowVLShader.cso", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_pEffect, &pError);
	if( FAILED( hr))
	{
		MessageBox( NULL, "�V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂���\n", "�G���[", MB_OK);
		return;
	}

	//�n���h���̎擾
	m_hTech = m_pEffect->GetTechnique(0);
	m_hMtxWorld = m_pEffect->GetParameterByName(0, "g_mtxWorld");
	m_hMtxWorldInv = m_pEffect->GetParameterByName(0, "g_mtxWorldInv");
	m_hMtxView = m_pEffect->GetParameterByName(0, "g_mtxView");
	m_hMtxProj = m_pEffect->GetParameterByName(0, "g_mtxProj");
	m_hTexture = m_pEffect->GetParameterByName( 0, "g_texture");
	m_hColAmb = m_pEffect->GetParameterByName( 0, "g_colAmb");
	m_hColDif = m_pEffect->GetParameterByName( 0, "g_colDif");
	m_hDirLight = m_pEffect->GetParameterByName( 0, "g_DirLight");
	m_hShadowBuf = m_pEffect->GetParameterByName( 0, "g_shadowBuf");
	m_hMtxLightVP = m_pEffect->GetParameterByName( 0, "g_mtxLightVP");
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
ShadowVLShader::~ShadowVLShader()
{
	Uninit();


}

/*------------------------------------------------------------------------------
	�V�F�[�_�[���Z�b�g
------------------------------------------------------------------------------*/
void ShadowVLShader::Set(Camera* pCamera, Renderer* pRenderer, Material* pMaterial)
{
	//�t�s��̐ݒ�
	D3DXMATRIX mtxWorldInv = pRenderer->m_pTransform->WorldMatrix();
	D3DXMatrixInverse( &mtxWorldInv, NULL, &mtxWorldInv);

	//���C�g�s��̐ݒ�
	Camera* pLightCamera = NULL;
	auto listCamera = Manager::GetRenderManager()->GetCameraList();
	for (auto camera : listCamera)
	{
		if (camera != Manager::GetRenderManager()->GetMainCamera())
		{
			pLightCamera = camera;
			break;
		}
	}
	if (pLightCamera == NULL)
	{
		MessageBox( NULL, "���C�g�J����������܂���\n", "�G���[", MB_OK);
		assert(false);
	}
	auto mtxLightVP = *pLightCamera->GetViewMatrix() * *pLightCamera->GetProjectionMatrix();

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//�f�o�C�X�̃|�C���^

	//���_�錾
	pDevice->SetVertexDeclaration( m_pVertexDec);

	//�e�N�j�b�N�̐ݒ�
	m_pEffect->SetTechnique( m_hTech);

	//�萔���V�F�[�_�ɓ`����
	m_pEffect->SetMatrix( m_hMtxWorld, &pRenderer->m_pTransform->WorldMatrix());
	m_pEffect->SetMatrix( m_hMtxWorldInv, &mtxWorldInv);
	m_pEffect->SetMatrix( m_hMtxView, pCamera->GetViewMatrix());
	m_pEffect->SetMatrix( m_hMtxProj, pCamera->GetProjectionMatrix());
	m_pEffect->SetTexture( m_hTexture, pMaterial->GetTexture());
	m_pEffect->SetVector( m_hColAmb, pMaterial->GetAmbient());
	m_pEffect->SetVector( m_hColDif, pMaterial->GetDiffuse());

	m_pEffect->SetTexture( m_hShadowBuf, Manager::GetTextureManager()->Load("shadow")->GetTexture());
	m_pEffect->SetMatrix( m_hMtxLightVP, &mtxLightVP);
	
	//���C�g�̎擾
	auto list = Light::GetList();
	for (auto light : list)
	{
		m_pEffect->SetVector( m_hDirLight, light->GetDirection());
	}

	m_pEffect->CommitChanges();
}


