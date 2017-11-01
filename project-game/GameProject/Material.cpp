/*==============================================================================

    Material.cpp - �}�e���A��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/5
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Material.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	�O���[�o���ϐ�
------------------------------------------------------------------------------*/
const D3DXVECTOR4 DefAmb( 0.3f, 0.3f, 0.3f, 1.0f);
const D3DXVECTOR4 DefDif( 1.0f, 1.0f, 1.0f, 1.0f);
const D3DXVECTOR4 DefSpe( 1.0f, 1.0f, 1.0f, 1.0f);
const D3DXVECTOR4 DefEmi( 0.5f, 0.5f, 0.5f, 0.5f);

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Material::Material()
{
	m_Ambient = DefAmb;
	m_Diffuse = DefDif;
	m_Specular = DefSpe;
	m_Emissive = DefEmi;

	m_pTexture = Manager::GetTextureManager()->Load( DEFAULT_TEXTURE_FILE_NAME);

	m_ShaderType = eShaderDefault;
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
	����
		D3DXMATERIAL* pMat
------------------------------------------------------------------------------*/
Material::Material( D3DXMATERIAL* pMat)
{
	m_Ambient = D3DXVECTOR4( pMat->MatD3D.Ambient.r, pMat->MatD3D.Ambient.g, pMat->MatD3D.Ambient.b, pMat->MatD3D.Ambient.a);
	m_Diffuse = D3DXVECTOR4( pMat->MatD3D.Diffuse.r, pMat->MatD3D.Diffuse.g, pMat->MatD3D.Diffuse.b, pMat->MatD3D.Diffuse.a);
	m_Specular = D3DXVECTOR4( pMat->MatD3D.Specular.r, pMat->MatD3D.Specular.g, pMat->MatD3D.Specular.b, pMat->MatD3D.Specular.a);
	m_Emissive = D3DXVECTOR4( pMat->MatD3D.Emissive.r, pMat->MatD3D.Emissive.g, pMat->MatD3D.Emissive.b, pMat->MatD3D.Emissive.a);

	m_Ambient = DefAmb;
	m_Specular = DefSpe;

	if( pMat->pTextureFilename != NULL)
	{
		m_pTexture = Manager::GetTextureManager()->Load( pMat->pTextureFilename);
	}
	else
	{
		m_pTexture = Manager::GetTextureManager()->Load( DEFAULT_TEXTURE_FILE_NAME);
	}

	m_ShaderType = eShaderDefault;
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Material::~Material()
{
	
}

/*------------------------------------------------------------------------------
	�Z�b�g
------------------------------------------------------------------------------*/
void Material::Set( Camera* pCamera, Renderer* pRenderer)
{
	auto pShader = Manager::GetShaderManager()->Load( m_ShaderType);
	pShader->Set( pCamera, pRenderer, this);
}

/*------------------------------------------------------------------------------
	�e�N�X�`�������n��
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 Material::GetTexture(void)
{
	return m_pTexture->GetTexture();
}

/*------------------------------------------------------------------------------
	�e�N�X�`���ݒ�
------------------------------------------------------------------------------*/
void Material::SetTexture(std::string FileName)
{
	m_pTexture = Manager::GetTextureManager()->Load( FileName);
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�ݒ�
------------------------------------------------------------------------------*/
void Material::SetShader(EShaderType Type)
{
	m_ShaderType = Type;
}

/*------------------------------------------------------------------------------
	�e�N�j�b�N�J�n
------------------------------------------------------------------------------*/
void Material::Begin(int nPass)
{
	Manager::GetShaderManager()->Load( m_ShaderType)->Begin( nPass);
}

/*------------------------------------------------------------------------------
	�e�N�j�b�N�I��
------------------------------------------------------------------------------*/
void Material::End(void)
{
	Manager::GetShaderManager()->Load( m_ShaderType)->End();
}

