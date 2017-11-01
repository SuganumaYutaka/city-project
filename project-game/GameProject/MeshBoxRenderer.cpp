/*==============================================================================

   MeshBoxRenderer.cpp - ボックス描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/14
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "MeshBoxRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define NUM_POLYGON (12)	//ポリゴン数
#define NUM_VERTEX (24)		//頂点数
#define NUM_INDEX (36)		//インデックス数

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
MeshBoxRenderer::MeshBoxRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 0;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//色の設定
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//中心位置・大きさの設定
	m_Center = Vector3( 0.0f, 0.0f, 0.0f);
	m_Size = Vector3( 1.0f, 1.0f, 1.0f);
	
	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX,	//作成したい頂点バッファのサイズ
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

	//頂点バッファ設定
	SetVtxBuffer();

	//インデックスバッファの作成
	HRESULT hr;
	hr = pDevice->CreateIndexBuffer(
		sizeof( WORD) * NUM_INDEX,		 //サイズ（WORD or DWORD）*インデックス数
		D3DUSAGE_WRITEONLY,				 //使用用途フラグ
		D3DFMT_INDEX16,					 //インデックスデータのフォーマット（16 or 32）
		D3DPOOL_MANAGED,				 //メモリの管理方法（おまかせ）
		&m_pIdxBuff,					 //インデックスバッファインターフェイスポインタのアドレス
		NULL);

	if( FAILED( hr))
	{
		//エラー
		assert( false);
		return;
	}

	//インデックスバッファ設定
	SetIdxBuffer();

	//マテリアル
	m_pMaterial = new Material();
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void MeshBoxRenderer::Uninit( void)
{
	Manager::GetRenderManager()->ReleaseRenderer( this);

	//頂点バッファの解放
	if( m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの解放
	if( m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
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
void MeshBoxRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void MeshBoxRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//ワイヤーフレーム設定
	if (m_nLayer == eLayerWireFrame)
	{
		pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//ワイヤーフレーム表示
	}

	//マテリアル（シェーダー）をセット
	m_pMaterial->Set( pCamera, this);

	//頂点情報設定
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_3D));

	//インデックスバッファの設定
	pDevice->SetIndices( m_pIdxBuff);

	//テクニック開始
	m_pMaterial->Begin( m_nPass);

	//プリミティブ（ポリゴン・図形）の描画（インデックス）
	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, NUM_VERTEX, 0,	NUM_POLYGON);

	//テクニック終了
	m_pMaterial->End();

	//ワイヤーフレーム設定
	if (m_nLayer == eLayerWireFrame)
	{
		pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	//中心を戻す
	Vector3 CenterInv = m_Center * -1;
	m_pTransform->Move( CenterInv);
}

/*------------------------------------------------------------------------------
	頂点バッファ設定
------------------------------------------------------------------------------*/
void MeshBoxRenderer::SetVtxBuffer( void)
{
	//頂点バッファをロックして、仮想アドレスを取得する
	VERTEX_3D* pVtx;				//仮想アドレス用ポインタ
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx,0);

	Vector3 Size = m_Size * 0.5f;

	//手前
	//座標（Z字）
	pVtx[ 0].Pos = D3DXVECTOR3( -Size.x,	+Size.y,	-Size.z);
	pVtx[ 1].Pos = D3DXVECTOR3( +Size.x,	+Size.y,	-Size.z);
	pVtx[ 2].Pos = D3DXVECTOR3( -Size.x,	-Size.y,	-Size.z);
	pVtx[ 3].Pos = D3DXVECTOR3( +Size.x,	-Size.y,	-Size.z);
	
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
	pVtx[ 0].Tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[ 1].Tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[ 2].Tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[ 3].Tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//奥
	//座標（Z字）
	pVtx[ 0].Pos = D3DXVECTOR3( +Size.x,	+Size.y,	Size.z);
	pVtx[ 1].Pos = D3DXVECTOR3( -Size.x,	+Size.y,	Size.z);
	pVtx[ 2].Pos = D3DXVECTOR3( +Size.x,	-Size.y,	Size.z);
	pVtx[ 3].Pos = D3DXVECTOR3( -Size.x,	-Size.y,	Size.z);
	
	//法線
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);

	//頂点カラー
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV座標（Z字）
	pVtx[ 0].Tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[ 1].Tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[ 2].Tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[ 3].Tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//左
	//座標（Z字）
	pVtx[ 0].Pos = D3DXVECTOR3( -Size.x,	+Size.y,	+Size.z);
	pVtx[ 1].Pos = D3DXVECTOR3( -Size.x,	+Size.y,	-Size.z);
	pVtx[ 2].Pos = D3DXVECTOR3( -Size.x,	-Size.y,	+Size.z);
	pVtx[ 3].Pos = D3DXVECTOR3( -Size.x,	-Size.y,	-Size.z);
	
	//法線
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( -1.0f, 0.0f, 0.0f);

	//頂点カラー
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV座標（Z字）
	pVtx[ 0].Tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[ 1].Tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[ 2].Tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[ 3].Tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//右
	//座標（Z字）
	pVtx[ 0].Pos = D3DXVECTOR3( +Size.x,	+Size.y,	-Size.z);
	pVtx[ 1].Pos = D3DXVECTOR3( +Size.x,	+Size.y,	+Size.z);
	pVtx[ 2].Pos = D3DXVECTOR3( +Size.x,	-Size.y,	-Size.z);
	pVtx[ 3].Pos = D3DXVECTOR3( +Size.x,	-Size.y,	+Size.z);
	
	//法線
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);

	//頂点カラー
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV座標（Z字）
	pVtx[ 0].Tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[ 1].Tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[ 2].Tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[ 3].Tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//上
	//座標（Z字）
	pVtx[ 0].Pos = D3DXVECTOR3( -Size.x,	+Size.y,	+Size.z);
	pVtx[ 1].Pos = D3DXVECTOR3( +Size.x,	+Size.y,	+Size.z);
	pVtx[ 2].Pos = D3DXVECTOR3( -Size.x,	+Size.y,	-Size.z);
	pVtx[ 3].Pos = D3DXVECTOR3( +Size.x,	+Size.y,	-Size.z);
	
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
	pVtx[ 0].Tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[ 1].Tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[ 2].Tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[ 3].Tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//下
	//座標（Z字）
	pVtx[ 0].Pos = D3DXVECTOR3( +Size.x,	-Size.y,	+Size.z);
	pVtx[ 1].Pos = D3DXVECTOR3( -Size.x,	-Size.y,	+Size.z);
	pVtx[ 2].Pos = D3DXVECTOR3( +Size.x,	-Size.y,	-Size.z);
	pVtx[ 3].Pos = D3DXVECTOR3( -Size.x,	-Size.y,	-Size.z);
	
	//法線
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( 0.0f, -1.0f, 0.0f);

	//頂点カラー
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV座標（Z字）
	pVtx[ 0].Tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[ 1].Tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[ 2].Tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[ 3].Tex = D3DXVECTOR2(1.0f, 1.0f);

	//アンロック
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	インデックスバッファ設定
------------------------------------------------------------------------------*/
void MeshBoxRenderer::SetIdxBuffer(void)
{
	//頂点バッファをロックして、仮想アドレスを取得する
	WORD* pIdx;		//インデックス情報
	m_pIdxBuff->Lock( 0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		pIdx[ 0] = nCnt * 4 + 0;
		pIdx[ 1] = nCnt * 4 + 1;
		pIdx[ 2] = nCnt * 4 + 2;

		pIdx[ 3] = nCnt * 4 + 1;
		pIdx[ 4] = nCnt * 4 + 3;
		pIdx[ 5] = nCnt * 4 + 2;

		if (nCnt >= 5)
		{
			return;
		}

		pIdx += 6;
	}

	//アンロック
	m_pIdxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	中心位置設定
------------------------------------------------------------------------------*/
void MeshBoxRenderer::SetCenter(const Vector3& Center)
{
	m_Center = Center;
}

/*------------------------------------------------------------------------------
	大きさ設定
------------------------------------------------------------------------------*/
void MeshBoxRenderer::SetSize(const Vector3& Size)
{
	m_Size = Size;

	//頂点バッファ設定
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	テクスチャ設定
------------------------------------------------------------------------------*/
void MeshBoxRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	シェーダー設定
------------------------------------------------------------------------------*/
void MeshBoxRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}
