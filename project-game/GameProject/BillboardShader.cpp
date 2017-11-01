/*==============================================================================

    BillboardShader.h - ビルボードシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/09
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BillboardShader.h"
#include "Renderer.h"
#include "ComponentInclude.h"
#include "Material.h"
#include "Texture.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
BillboardShader::BillboardShader()
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
	hr = D3DXCreateEffectFromFile(pDevice, "data/SHADER/BillboardShader.cso", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_pEffect, &pError);
	if( FAILED( hr))
	{
		MessageBox( NULL, "シェーダーの読み込みに失敗しました\n", "エラー", MB_OK);
		return;
	}

	//ハンドルの取得
	m_hTech = m_pEffect->GetTechnique(0);
	m_hMtxPos = m_pEffect->GetParameterByName(0, "g_mtxPos");
	m_hMtxScale = m_pEffect->GetParameterByName(0, "g_mtxScale");
	m_hMtxView = m_pEffect->GetParameterByName(0, "g_mtxView");
	m_hMtxViewInverse = m_pEffect->GetParameterByName(0, "g_mtxViewInverse");
	m_hMtxProj = m_pEffect->GetParameterByName(0, "g_mtxProj");
	m_hTexture = m_pEffect->GetParameterByName( 0, "g_texture");
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
BillboardShader::~BillboardShader()
{
	Uninit();


}

/*------------------------------------------------------------------------------
	シェーダーをセット
------------------------------------------------------------------------------*/
void BillboardShader::Set(Camera* pCamera, Renderer* pRenderer, Material* pMaterial)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//デバイスのポインタ

	//ビルボード処理
	D3DXMATRIX mtxPos, mtxScale, mtxViewInverse;
	Vector3 Pos = pRenderer->m_pTransform->GetWorldPosition();
	Vector3 Scale = pRenderer->m_pTransform->GetWorldScale();
	D3DXMatrixTranslation( &mtxPos, Pos.x, Pos.y, Pos.z);
	D3DXMatrixScaling( &mtxScale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixTranspose( &mtxViewInverse, pCamera->GetViewMatrix());		//転地
	mtxViewInverse._14 = 0.0f;
	mtxViewInverse._24 = 0.0f;
	mtxViewInverse._34 = 0.0f;

	//頂点宣言
	pDevice->SetVertexDeclaration( m_pVertexDec);

	//テクニックの設定
	m_pEffect->SetTechnique( m_hTech);

	//定数をシェーダに伝える
	m_pEffect->SetMatrix( m_hMtxPos, &mtxPos);
	m_pEffect->SetMatrix( m_hMtxScale, &mtxScale);
	m_pEffect->SetMatrix( m_hMtxView, pCamera->GetViewMatrix());
	m_pEffect->SetMatrix( m_hMtxViewInverse, &mtxViewInverse);
	m_pEffect->SetMatrix( m_hMtxProj, pCamera->GetProjectionMatrix());
	m_pEffect->SetTexture( m_hTexture, pMaterial->GetTexture());
	m_pEffect->CommitChanges();
}


