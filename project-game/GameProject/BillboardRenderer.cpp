/*==============================================================================

    BillboardRenderer.cpp - 3Dポリゴン描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/25
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BillboardRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
BillboardRenderer::BillboardRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerBillboard;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 1;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * 4,				//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,					//頂点バッファの使用方法(速さに影響)
		0,									//FVF(頂点フォーマット)
		D3DPOOL_MANAGED,					//メモリの管理(MANAGEDはデバイスにおまかせ)
		&m_pVtxBuff,						//頂点バッファ管理インターフェイス
		NULL)))
	{
		//エラー
		assert( false);
		return;
	}

	//色の設定
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//頂点設定
	SetVtxBuffer();
	
	//マテリアル
	m_pMaterial = new Material();
	m_pMaterial->SetShader( eShaderBillboard);
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void BillboardRenderer::Uninit( void)
{
	Manager::GetRenderManager()->ReleaseRenderer( this);

	//頂点バッファの解放
	if( m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//マテリアルの解放
	if (m_pMaterial != NULL)
	{
		delete m_pMaterial;
		m_pMaterial = NULL;
	}
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void BillboardRenderer::Update( void)
{
	//頂点設定
	//SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void BillboardRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//ビルボード処理
	SetBillboard( *pCamera->GetViewMatrix());

	//マテリアル（シェーダー）をセット
	m_pMaterial->Set( pCamera, this);

	//頂点情報設定
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_3D));

	//テクニック開始
	m_pMaterial->Begin( m_nPass);

	//ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0,	2);

	//テクニック終了
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	頂点バッファ設定
------------------------------------------------------------------------------*/
void BillboardRenderer::SetVtxBuffer( void)
{
	//ロック
	VERTEX_3D* pVtx;		//仮想アドレス用ポインタ
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	//頂点設定
	//座標（Z字）
	pVtx[ 0].Pos = D3DXVECTOR3( -0.5f, +0.5f, 0.0f);
	pVtx[ 1].Pos = D3DXVECTOR3( +0.5f, +0.5f, 0.0f);
	pVtx[ 2].Pos = D3DXVECTOR3( -0.5f, -0.5f, 0.0f);
	pVtx[ 3].Pos = D3DXVECTOR3( +0.5f, -0.5f, 0.0f);
	
	//法線
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);

	//頂点カラー
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV座標（Z字）
	pVtx[ 0].Tex = D3DXVECTOR2( m_TextureUV.UV[ 0].x, m_TextureUV.UV[ 0].y);
	pVtx[ 1].Tex = D3DXVECTOR2( m_TextureUV.UV[ 1].x, m_TextureUV.UV[ 1].y);
	pVtx[ 2].Tex = D3DXVECTOR2( m_TextureUV.UV[ 2].x, m_TextureUV.UV[ 2].y);
	pVtx[ 3].Tex = D3DXVECTOR2( m_TextureUV.UV[ 3].x, m_TextureUV.UV[ 3].y);

	//アンロック
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	テクスチャ設定
------------------------------------------------------------------------------*/
void BillboardRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	ビルボード処理
------------------------------------------------------------------------------*/
void BillboardRenderer::SetBillboard(D3DXMATRIX mtxView)
{
	//転地行列
	D3DXMatrixTranspose( &mtxView, &mtxView);
	mtxView._14 = 0.0f;
	mtxView._24 = 0.0f;
	mtxView._34 = 0.0f;


}


