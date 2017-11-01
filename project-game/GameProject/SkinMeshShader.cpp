/*==============================================================================

    SkinMeshShader.cpp - スキンメッシュシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/9/21
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "SkinMeshShader.h"
#include "Renderer.h"
#include "ComponentInclude.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define MAX_CLUSTER (58)		//クラスターの最大数（シェーダーに渡すため固定長）

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
SkinMeshShader::SkinMeshShader()
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//デバイスのポインタ
	
	//頂点宣言格納インターフェイス作成
	D3DVERTEXELEMENT9 g_Dec1[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,  0},
		{ 0, 48, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration( g_Dec1, &m_pVertexDec);

	//シェーダの読み込み
	HRESULT hr;
	ID3DXBuffer *pError;		//コンパイルエラー情報格納バッファ
	hr = D3DXCreateEffectFromFile(pDevice, "data/SHADER/SkinMeshShader.cso", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_pEffect, &pError);
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
	m_hColAmb = m_pEffect->GetParameterByName( 0, "g_colAmb");
	m_hColDif = m_pEffect->GetParameterByName( 0, "g_colDif");
	m_hDirLight = m_pEffect->GetParameterByName( 0, "g_DirLight");
	m_hClusters = m_pEffect->GetParameterByName( 0, "g_mtxClusters");
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
SkinMeshShader::~SkinMeshShader()
{
	Uninit();


}

/*------------------------------------------------------------------------------
	シェーダーをセット
------------------------------------------------------------------------------*/
void SkinMeshShader::Set(Camera* pCamera, Renderer* pRenderer, Material* pMaterial)
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
	m_pEffect->SetVector( m_hColAmb, pMaterial->GetAmbient());
	m_pEffect->SetVector( m_hColDif, pMaterial->GetDiffuse());
	
	//ライトの取得
	auto list = Light::GetList();
	for (auto light : list)
	{
		m_pEffect->SetVector( m_hDirLight, light->GetDirection());
	}

	//クラスター情報を送る
	SkinMeshRenderer* pMesh = pRenderer->m_pGameObject->GetComponent<SkinMeshRenderer>();
	if (pMesh == NULL)
	{
		MessageBox( NULL, "現在FbxMeshRendererのみがSkinMeshShaderを利用できます", "エラー", MB_OK);
		return;
	}

	//アニメーション情報があるとき
	int numAnim = pMesh->GetAnimation().size();
	if ( numAnim > 0)
	{
		int state = pMesh->GetSkinMeshModel()->GetAnimationState();	//現在のアニメーション
		auto anim = pMesh->GetAnimation( state);
	
		//現在のアニメーションの現在のフレームの姿勢行列を渡す
		HRESULT hr = m_pEffect->SetMatrixArray( m_hClusters, 
			anim.vecMatrix[pMesh->GetSkinMeshModel()->GetFrame() % anim.AllFrame].data(), 58);
		if (hr == D3DERR_INVALIDCALL)
		{
			MessageBox( NULL, "クラスターデータを送れません", "エラー", MB_OK);
		}
	}
	else
	{
		MessageBox( NULL, "アニメーション情報がありません", "エラー", MB_OK);
	}

	m_pEffect->CommitChanges();
}


