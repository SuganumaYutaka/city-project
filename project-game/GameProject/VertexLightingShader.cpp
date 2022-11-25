/*==============================================================================

    VertexLightingShader.cpp - 頂点ライティングシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/14
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "VertexLightingShader.h"
#include "Renderer.h"
#include "ComponentInclude.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
VertexLightingShader::VertexLightingShader()
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//デバイスのポインタ
	
	//頂点宣言格納インターフェイス作成
	D3DVERTEXELEMENT9 g_Dec1[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration( g_Dec1, &m_pVertexDec);

	//シェーダの読み込み
	HRESULT hr;
	ID3DXBuffer *pError;		//コンパイルエラー情報格納バッファ
	hr = D3DXCreateEffectFromFile(pDevice, "data/SHADER/VertexLightingShader.cso", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_pEffect, &pError);
	if( FAILED( hr))
	{
		MessageBox( NULL, "シェーダーの読み込みに失敗しました\n", "エラー", MB_OK);
		return;
	}

	//ハンドルの取得
	m_hTech = m_pEffect->GetTechnique(0);
	m_hMtxWVP = m_pEffect->GetParameterByName(0, "g_mtxWVP");
	m_hMtxWIT = m_pEffect->GetParameterByName(0, "g_mtxWIT");
	m_hTexture = m_pEffect->GetParameterByName( 0, "g_texture");
	m_hLightAmb = m_pEffect->GetParameterByName( 0, "g_LightAmb");
	m_hLightDif = m_pEffect->GetParameterByName( 0, "g_LightDif");
	m_hMaterialAmb = m_pEffect->GetParameterByName( 0, "g_MaterialAmb");
	m_hMaterialDif = m_pEffect->GetParameterByName( 0, "g_MaterialDif");
	m_hLightDirW = m_pEffect->GetParameterByName( 0, "g_lightDirW");

}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
VertexLightingShader::~VertexLightingShader()
{
	Uninit();


}

/*------------------------------------------------------------------------------
	シェーダーをセット
------------------------------------------------------------------------------*/
void VertexLightingShader::Set(Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//デバイスのポインタ

	//if( !isAlreadySet)
	{
		//頂点宣言
		pDevice->SetVertexDeclaration( m_pVertexDec);

		//テクニックの設定
		m_pEffect->SetTechnique( m_hTech);
	}

	auto mtxWorld = pRenderer->m_pTransform->WorldMatrix();
	
	D3DXMATRIX mtxWVP = mtxWorld * *pCamera->GetViewMatrix() * *pCamera->GetProjectionMatrix();
	D3DXMATRIX mtxWIT;
	D3DXMatrixInverse( &mtxWIT, NULL, &mtxWorld);
	D3DXMatrixTranspose( &mtxWIT, &mtxWIT);
	
	//定数をシェーダに伝える
	m_pEffect->SetMatrix( m_hMtxWVP, &mtxWVP);
	m_pEffect->SetMatrix( m_hMtxWIT, &mtxWIT);
	m_pEffect->SetTexture( m_hTexture, pMaterial->GetTexture());
	m_pEffect->SetVector( m_hMaterialAmb, pMaterial->GetAmbient());
	m_pEffect->SetVector( m_hMaterialDif, pMaterial->GetDiffuse());
	
	//ライトの取得
	auto list = Light::GetList();
	for (auto light : list)
	{
		D3DXVECTOR4 lightDir( light->GetDirection().ConvertToDX());
		lightDir.w = 1.0f;
		m_pEffect->SetVector( m_hLightDirW, &lightDir);
		m_pEffect->SetVector( m_hLightAmb, light->GetAmbient());
		m_pEffect->SetVector( m_hLightDif, light->GetDiffuse());
	}

	m_pEffect->CommitChanges();
}


