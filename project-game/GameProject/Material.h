/*==============================================================================

    Material.h - マテリアル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/5
==============================================================================*/
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "ShaderManager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Texture;
class Renderer;
class Camera;
class Shader;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Material
{
public:
	Material();
	Material( D3DXMATERIAL* pMat);
	~Material();

	void Set( Camera* pCamera, Renderer* pRenderer);

	LPDIRECT3DTEXTURE9 GetTexture( void);
	void SetTexture( std::string FileName);

	void SetShader( EShaderType Type);
	void Begin(int nPass);
	void End(void);

	D3DXVECTOR4* GetAmbient( void) { return &m_Ambient;}
	D3DXVECTOR4* GetDiffuse( void) { return &m_Diffuse;}
	D3DXVECTOR4* GetSpecular( void) { return &m_Specular;}
	D3DXVECTOR4* GetEmissive( void) { return &m_Emissive;}
	
	void SetAmbient( float r, float g, float b, float a) { m_Ambient = D3DXVECTOR4( r, g, b, a);}
	void SetDiffuse( float r, float g, float b, float a) { m_Diffuse = D3DXVECTOR4( r, g, b, a);}
	void SetSpecular( float r, float g, float b, float a) { m_Specular = D3DXVECTOR4( r, g, b, a);}
	void SetEmissive( float r, float g, float b, float a) { m_Emissive = D3DXVECTOR4( r, g, b, a);}

private:
	D3DXVECTOR4 m_Ambient;
	D3DXVECTOR4 m_Diffuse;
	D3DXVECTOR4 m_Specular;
	D3DXVECTOR4 m_Emissive;

	Texture *m_pTexture;			//テクスチャクラスポインタ
	//Shader *m_pShader;			//シェーダーポインタ
	EShaderType m_ShaderType;		//シェーダーのタイプ
};


#endif