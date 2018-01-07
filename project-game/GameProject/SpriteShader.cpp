/*==============================================================================

    SpriteShader.cpp - 2D�|���S���V�F�[�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/11
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "SpriteShader.h"
#include "Renderer.h"
#include "ComponentInclude.h"
#include "Material.h"
#include "Texture.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
SpriteShader::SpriteShader()
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//�f�o�C�X�̃|�C���^
	
	//���_�錾�i�[�C���^�[�t�F�C�X�쐬
	D3DVERTEXELEMENT9 g_Dec1[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
		{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration( g_Dec1, &m_pVertexDec);

	//�V�F�[�_�̓ǂݍ���
	HRESULT hr;
	ID3DXBuffer *pError;		//�R���p�C���G���[���i�[�o�b�t�@
	hr = D3DXCreateEffectFromFile(pDevice, "data/SHADER/SpriteShader.cso", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_pEffect, &pError);
	if( FAILED( hr))
	{
		MessageBox( NULL, "�V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂���\n", "�G���[", MB_OK);
		return;
	}

	//�n���h���̎擾
	m_hTech = m_pEffect->GetTechnique(0);
	m_hTexture = m_pEffect->GetParameterByName( 0, "g_texture");
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
SpriteShader::~SpriteShader()
{
	Uninit();


}

/*------------------------------------------------------------------------------
	�V�F�[�_�[���Z�b�g
------------------------------------------------------------------------------*/
void SpriteShader::Set(Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//�f�o�C�X�̃|�C���^

	if( !isAlreadySet)
	{
		//���_�錾
		pDevice->SetVertexDeclaration( m_pVertexDec);

		//�e�N�j�b�N�̐ݒ�
		m_pEffect->SetTechnique( m_hTech);
	}

	//�萔���V�F�[�_�ɓ`����
	m_pEffect->SetTexture( m_hTexture, pMaterial->GetTexture());
	m_pEffect->CommitChanges();
}


