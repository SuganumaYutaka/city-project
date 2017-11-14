/*==============================================================================

    Material.cpp - マテリアル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/5
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Material.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	グローバル変数
------------------------------------------------------------------------------*/
const D3DXVECTOR4 DefAmb( 0.3f, 0.3f, 0.3f, 1.0f);
const D3DXVECTOR4 DefDif( 1.0f, 1.0f, 1.0f, 1.0f);
const D3DXVECTOR4 DefSpe( 1.0f, 1.0f, 1.0f, 1.0f);
const D3DXVECTOR4 DefEmi( 0.5f, 0.5f, 0.5f, 0.5f);

/*------------------------------------------------------------------------------
	コンストラクタ
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
	コンストラクタ
	引数
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
	デストラクタ
------------------------------------------------------------------------------*/
Material::~Material()
{
	
}

/*------------------------------------------------------------------------------
	セット
------------------------------------------------------------------------------*/
void Material::Set( Camera* pCamera, Renderer* pRenderer)
{
	auto pShader = Manager::GetShaderManager()->Load( m_ShaderType);
	pShader->Set( pCamera, pRenderer, this);
}

/*------------------------------------------------------------------------------
	テクスチャ引き渡し
------------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 Material::GetTexture(void)
{
	return m_pTexture->GetTexture();
}

/*------------------------------------------------------------------------------
	テクスチャ設定
------------------------------------------------------------------------------*/
void Material::SetTexture(std::string FileName)
{
	m_pTexture = Manager::GetTextureManager()->Load( FileName);
}

/*------------------------------------------------------------------------------
	テクスチャ名引き渡し
------------------------------------------------------------------------------*/
std::string Material::GetTextureName(void)
{
	return m_pTexture->GetFileName();
}

/*------------------------------------------------------------------------------
	シェーダー設定
------------------------------------------------------------------------------*/
void Material::SetShader(EShaderType Type)
{
	m_ShaderType = Type;
}

/*------------------------------------------------------------------------------
	テクニック開始
------------------------------------------------------------------------------*/
void Material::Begin(int nPass)
{
	Manager::GetShaderManager()->Load( m_ShaderType)->Begin( nPass);
}

/*------------------------------------------------------------------------------
	テクニック終了
------------------------------------------------------------------------------*/
void Material::End(void)
{
	Manager::GetShaderManager()->Load( m_ShaderType)->End();
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void Material::Load(Text& text)
{
	//textを読み進める
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndMaterial")
	{
		if (text.GetWord() == "Ambient")
		{
			text.ForwardPositionToNextWord();
			
			m_Ambient.x = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Ambient.y = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Ambient.z = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Ambient.w = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Diffuse")
		{
			text.ForwardPositionToNextWord();
			
			m_Diffuse.x = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Diffuse.y = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Diffuse.z = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Diffuse.w = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Specular")
		{
			text.ForwardPositionToNextWord();
			
			m_Specular.x = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Specular.y = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Specular.z = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Specular.w = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Emissive")
		{
			text.ForwardPositionToNextWord();
			
			m_Emissive.x = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Emissive.y = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Emissive.z = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Emissive.w = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Texture")
		{
			text.ForwardPositionToNextWord();
			
			SetTexture( text.GetWord());
		}

		else if (text.GetWord() == "ShaderType")
		{
			text.ForwardPositionToNextWord();
			
			m_ShaderType = (EShaderType)std::stoi( text.GetWord());
		}

		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	セーブ
------------------------------------------------------------------------------*/
void Material::Save(Text& text)
{
	text += "Material\n";

	text += "Ambient "
		+ std::to_string(m_Ambient.x) + ' '
		+ std::to_string(m_Ambient.y) + ' '
		+ std::to_string(m_Ambient.z) + ' '
		+ std::to_string(m_Ambient.w) + '\n';
	text += "Diffuse "
		+ std::to_string(m_Diffuse.x) + ' '
		+ std::to_string(m_Diffuse.y) + ' '
		+ std::to_string(m_Diffuse.z) + ' '
		+ std::to_string(m_Diffuse.w) + '\n';
	text += "Specular "
		+ std::to_string(m_Specular.x) + ' '
		+ std::to_string(m_Specular.y) + ' '
		+ std::to_string(m_Specular.z) + ' '
		+ std::to_string(m_Specular.w) + '\n';
	text += "Emissive "
		+ std::to_string(m_Emissive.x) + ' '
		+ std::to_string(m_Emissive.y) + ' '
		+ std::to_string(m_Emissive.z) + ' '
		+ std::to_string(m_Emissive.w) + '\n';
	text += "Texture " + m_pTexture->GetFileName() + '\n';
	text += "ShaderType " + std::to_string((int)m_ShaderType) + '\n';

	text += "EndMaterial\n";
}