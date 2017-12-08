/*==============================================================================

    PerPixelLightingShader.cpp - パーピクセルライティングシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/08
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "PerPixelLightingShader.h"
#include "Renderer.h"
#include "ComponentInclude.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
PerPixelLightingShader::PerPixelLightingShader()
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
	hr = D3DXCreateEffectFromFile(pDevice, "data/SHADER/PerPixelLightingShader.cso", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_pEffect, &pError);
	if( FAILED( hr))
	{
		MessageBox( NULL, "シェーダーの読み込みに失敗しました\n", "エラー", MB_OK);
		return;
	}

	//ハンドルの取得
	m_hTech = m_pEffect->GetTechnique(0);
	m_hMtxWorld = m_pEffect->GetParameterByName(0, "g_mtxWorld");
	m_hMtxWorldInv = m_pEffect->GetParameterByName(0, "g_mtxWorldInv");
	m_hMtxView = m_pEffect->GetParameterByName(0, "g_mtxView");
	m_hMtxProj = m_pEffect->GetParameterByName(0, "g_mtxProj");
	m_hTexture = m_pEffect->GetParameterByName( 0, "g_texture");

	m_hLightAmb = m_pEffect->GetParameterByName( 0, "g_LightAmb");
	m_hLightDif = m_pEffect->GetParameterByName( 0, "g_LightDif");
	m_hLightSpe = m_pEffect->GetParameterByName( 0, "g_LightSpe");
	m_hMaterialAmb = m_pEffect->GetParameterByName( 0, "g_MaterialAmb");
	m_hMaterialDif = m_pEffect->GetParameterByName( 0, "g_MaterialDif");
	m_hMaterialSpe = m_pEffect->GetParameterByName( 0, "g_MaterialSpe");
	m_hPosLight = m_pEffect->GetParameterByName( 0, "g_PosLight");
	m_hDirLight = m_pEffect->GetParameterByName( 0, "g_DirLight");
	m_hPosEye = m_pEffect->GetParameterByName( 0, "g_PosEye");
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
PerPixelLightingShader::~PerPixelLightingShader()
{
	Uninit();


}

/*------------------------------------------------------------------------------
	シェーダーをセット
------------------------------------------------------------------------------*/
void PerPixelLightingShader::Set(Camera* pCamera, Renderer* pRenderer, Material* pMaterial)
{
	D3DXMATRIX mtxWorldInv = pRenderer->m_pTransform->WorldMatrix();
	D3DXMatrixInverse( &mtxWorldInv, NULL, &mtxWorldInv);

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//デバイスのポインタ

	//頂点宣言
	pDevice->SetVertexDeclaration( m_pVertexDec);

	//テクニックの設定
	m_pEffect->SetTechnique( m_hTech);

	//定数をシェーダに伝える
	m_pEffect->SetMatrix( m_hMtxWorld, &pRenderer->m_pTransform->WorldMatrix());
	m_pEffect->SetMatrix( m_hMtxWorldInv, &mtxWorldInv);
	m_pEffect->SetMatrix( m_hMtxView, pCamera->GetViewMatrix());
	m_pEffect->SetMatrix( m_hMtxProj, pCamera->GetProjectionMatrix());
	m_pEffect->SetTexture( m_hTexture, pMaterial->GetTexture());
	m_pEffect->SetVector( m_hMaterialAmb, pMaterial->GetAmbient());
	m_pEffect->SetVector( m_hMaterialDif, pMaterial->GetDiffuse());
	m_pEffect->SetVector( m_hMaterialSpe, pMaterial->GetSpecular());
	
	//ライトの取得
	auto list = Light::GetList();
	for (auto light : list)
	{
		D3DXVECTOR4 lightPos( light->GetPosition().ConvertToDX());
		m_pEffect->SetVector( m_hDirLight, &lightPos);
		D3DXVECTOR4 lightDir( light->GetDirection().ConvertToDX());
		m_pEffect->SetVector( m_hDirLight, &lightDir);
		
		m_pEffect->SetVector( m_hLightAmb, light->GetAmbient());
		m_pEffect->SetVector( m_hLightDif, light->GetDiffuse());
		m_pEffect->SetVector( m_hLightSpe, light->GetSpecular());
		
		//今は一つだけ
		break;
	}

	//カメラの設定
	auto pos = pCamera->m_pTransform->GetWorldPosition();
	D3DXVECTOR4 PosEye( pos.x, pos.y, pos.z, 0.0f);
	m_pEffect->SetVector( m_hPosEye, &PosEye);

	m_pEffect->CommitChanges();
}


