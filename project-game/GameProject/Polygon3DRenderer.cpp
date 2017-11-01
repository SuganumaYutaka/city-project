/*==============================================================================

    Polygon3DRenderer.cpp - 3Dポリゴン描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/17
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Polygon3DRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Polygon3DRenderer::Polygon3DRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
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
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void Polygon3DRenderer::Uninit( void)
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
void Polygon3DRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void Polygon3DRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

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
void Polygon3DRenderer::SetVtxBuffer( void)
{
	//ロック
	VERTEX_3D* pVtx;		//仮想アドレス用ポインタ
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	//頂点設定
	//座標（Z字）
	pVtx[ 0].Pos = D3DXVECTOR3( -0.5f, 0.0f, +0.5f);
	pVtx[ 1].Pos = D3DXVECTOR3( +0.5f, 0.0f, +0.5f);
	pVtx[ 2].Pos = D3DXVECTOR3( -0.5f, 0.0f, -0.5f);
	pVtx[ 3].Pos = D3DXVECTOR3( +0.5f, 0.0f, -0.5f);
	
	//法線
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

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
void Polygon3DRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	シェーダー設定
------------------------------------------------------------------------------*/
void Polygon3DRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}
