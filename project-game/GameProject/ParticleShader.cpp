/*==============================================================================

    ParticleShader.cpp - パーティクルシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/16
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "ParticleShader.h"
#include "Renderer.h"
#include "ComponentInclude.h"
#include "Material.h"
#include "Texture.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
ParticleShader::ParticleShader()
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//デバイスのポインタ
	
	//頂点宣言格納インターフェイス作成
	D3DVERTEXELEMENT9 g_Dec1[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 16, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_PSIZE, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 0, 36, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration( g_Dec1, &m_pVertexDec);

	//シェーダの読み込み
	HRESULT hr;
	ID3DXBuffer *pError;		//コンパイルエラー情報格納バッファ
	hr = D3DXCreateEffectFromFile(pDevice, "data/SHADER/ParticleShader.cso", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_pEffect, &pError);
	if( FAILED( hr))
	{
		MessageBox( NULL, "シェーダーの読み込みに失敗しました\n", "エラー", MB_OK);
		return;
	}

	//ハンドルの取得
	m_hTech = m_pEffect->GetTechnique(0);
	m_hMtxWVP = m_pEffect->GetParameterByName(0, "g_mtxWVP");
	m_hTime = m_pEffect->GetParameterByName(0, "g_time");
	m_hTexture = m_pEffect->GetParameterByName( 0, "g_texture");
	m_hGravity = m_pEffect->GetParameterByName( 0, "g_gravity");
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
ParticleShader::~ParticleShader()
{
	Uninit();


}

/*------------------------------------------------------------------------------
	シェーダーをセット
------------------------------------------------------------------------------*/
void ParticleShader::Set(Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//デバイスのポインタ

	if( !isAlreadySet)
	{
		//頂点宣言
		pDevice->SetVertexDeclaration( m_pVertexDec);

		//テクニックの設定
		m_pEffect->SetTechnique( m_hTech);
	}

	//定数をシェーダに伝える
	D3DXMATRIX mtxWorld = pRenderer->m_pTransform->WorldMatrix() * *pCamera->GetViewMatrix() * *pCamera->GetProjectionMatrix();
	m_pEffect->SetMatrix( m_hMtxWVP, &mtxWorld);
	float time = (float)timeGetTime();
	m_pEffect->SetFloat( m_hTime, time);
	m_pEffect->SetTexture( m_hTexture, pMaterial->GetTexture());
	m_pEffect->SetFloat( m_hGravity, -0.000003f);	//重力
	m_pEffect->CommitChanges();
}


