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
	m_hMtxWVP = m_pEffect->GetParameterByName(0, "g_mtxWVP");
	m_hMtxWIT = m_pEffect->GetParameterByName(0, "g_mtxWIT");
	m_hTexture = m_pEffect->GetParameterByName( 0, "g_texture");
	m_hLightAmb = m_pEffect->GetParameterByName( 0, "g_LightAmb");
	m_hLightDif = m_pEffect->GetParameterByName( 0, "g_LightDif");
	m_hMaterialAmb = m_pEffect->GetParameterByName( 0, "g_MaterialAmb");
	m_hMaterialDif = m_pEffect->GetParameterByName( 0, "g_MaterialDif");
	m_hLightDirW = m_pEffect->GetParameterByName( 0, "g_lightDirW");

	m_hShadowBuf = m_pEffect->GetParameterByName( 0, "g_shadowBuf");
	m_hMtxLightWVP = m_pEffect->GetParameterByName( 0, "g_mtxLightWVP");
	m_hMtxLightWV = m_pEffect->GetParameterByName( 0, "g_mtxLightWV");
	m_hFar = m_pEffect->GetParameterByName( 0, "g_far");
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
void ShadowVLShader::Set(Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//�f�o�C�X�̃|�C���^

	auto mtxWorld = pRenderer->m_pTransform->WorldMatrix();
	
	D3DXMATRIX mtxWVP = mtxWorld * *pCamera->GetViewMatrix() * *pCamera->GetProjectionMatrix();
	D3DXMATRIX mtxWIT;
	D3DXMatrixInverse( &mtxWIT, NULL, &mtxWorld);
	D3DXMatrixTranspose( &mtxWIT, &mtxWIT);

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
	auto mtxLightWVP = pRenderer->m_pTransform->WorldMatrix() * *pLightCamera->GetViewMatrix() * *pLightCamera->GetProjectionMatrix();
	auto mtxLightWV = pRenderer->m_pTransform->WorldMatrix() * *pLightCamera->GetViewMatrix();
	
	//if( !isAlreadySet)
	{
		//���_�錾
		pDevice->SetVertexDeclaration( m_pVertexDec);

		//�e�N�j�b�N�̐ݒ�
		m_pEffect->SetTechnique( m_hTech);
	}

	//�萔���V�F�[�_�ɓ`����
	m_pEffect->SetMatrix( m_hMtxWVP, &mtxWVP);
	m_pEffect->SetMatrix( m_hMtxWIT, &mtxWIT);
	m_pEffect->SetTexture( m_hTexture, pMaterial->GetTexture());
	m_pEffect->SetVector( m_hMaterialAmb, pMaterial->GetAmbient());
	m_pEffect->SetVector( m_hMaterialDif, pMaterial->GetDiffuse());

	m_pEffect->SetTexture( m_hShadowBuf, Manager::GetTextureManager()->Load("shadow")->GetTexture());
	m_pEffect->SetMatrix( m_hMtxLightWVP, &mtxLightWVP);
	m_pEffect->SetMatrix( m_hMtxLightWV, &mtxLightWV);
	m_pEffect->SetFloat( m_hFar, pLightCamera->GetFar());
	
	//���C�g�̎擾
	auto list = Light::GetList();
	for (auto light : list)
	{
		D3DXVECTOR4 lightDir( light->GetDirection().ConvertToDX());
		m_pEffect->SetVector( m_hLightDirW, &lightDir);
		m_pEffect->SetVector( m_hLightAmb, light->GetAmbient());
		m_pEffect->SetVector( m_hLightDif, light->GetDiffuse());
	}

	m_pEffect->CommitChanges();
}


