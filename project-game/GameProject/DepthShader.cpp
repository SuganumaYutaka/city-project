/*==============================================================================

    DepthShader.h - デプスシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/15
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "DepthShader.h"
#include "Renderer.h"
#include "ComponentInclude.h"
#include "Material.h"
#include "Texture.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
DepthShader::DepthShader()
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//デバイスのポインタ
	
	//頂点宣言格納インターフェイス作成
	D3DVERTEXELEMENT9 g_Dec1[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration( g_Dec1, &m_pVertexDec);

	//シェーダの読み込み
	HRESULT hr;
	ID3DXBuffer *pError;		//コンパイルエラー情報格納バッファ
	hr = D3DXCreateEffectFromFile(pDevice, "data/SHADER/DepthShader.cso", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_pEffect, &pError);
	if( FAILED( hr))
	{
		MessageBox( NULL, "シェーダーの読み込みに失敗しました\n", "エラー", MB_OK);
		return;
	}

	//ハンドルの取得
	m_hTech = m_pEffect->GetTechnique(0);
	m_hMtxWorld = m_pEffect->GetParameterByName(0, "g_mtxWorld");
	m_hMtxView = m_pEffect->GetParameterByName(0, "g_mtxView");
	m_hMtxProj = m_pEffect->GetParameterByName(0, "g_mtxProj");
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
DepthShader::~DepthShader()
{
	Uninit();


}

/*------------------------------------------------------------------------------
	シェーダーをセット
------------------------------------------------------------------------------*/
void DepthShader::Set(Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet)
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
	m_pEffect->SetMatrix( m_hMtxWorld, &pRenderer->m_pTransform->WorldMatrix());
	m_pEffect->SetMatrix( m_hMtxView, pCamera->GetViewMatrix());
	m_pEffect->SetMatrix( m_hMtxProj, pCamera->GetProjectionMatrix());
	m_pEffect->CommitChanges();
}


