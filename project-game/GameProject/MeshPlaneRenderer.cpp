/*==============================================================================

   MeshPlaneRenderer.cpp - 平面描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/24
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "MeshPlaneRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define P_POS_X(P_WIDTH, NUM_FIELD_X) (0.0f + P_WIDTH * NUM_FIELD_X * -0.5f)		//開始地点のX座標
#define P_POS_Y (0.0f)																//開始地点のY座標
#define P_POS_Z(P_HEIGHT, NUM_FIELD_Z) (0.0f + P_HEIGHT * NUM_FIELD_Z * 0.5f)		//開始地点のZ座標
#define P_WIDTH( ALL_WIDTH, NUM_FIELD_X)			( ALL_WIDTH / NUM_FIELD_X)										//ポリゴン一枚あたりの幅
#define P_HEIGHT( ALL_HEIGHT, NUM_FIELD_Z)			( ALL_HEIGHT / NUM_FIELD_Z)										//ポリゴン一枚あたりの高さ
#define NUM_POLYGON( NUM_FIELD_X, NUM_FIELD_Z)		( 2 * NUM_FIELD_X * NUM_FIELD_Z + ( NUM_FIELD_Z - 1) * 4)		//ポリゴン数
#define NUM_VERTEX( NUM_FIELD_X, NUM_FIELD_Z)		( ( NUM_FIELD_X + 1) * ( NUM_FIELD_Z + 1))						//頂点数
#define NUM_INDEX( NUM_FIELD_X, NUM_FIELD_Z)		( ( (NUM_FIELD_X + 1) * 2 + 2) * NUM_FIELD_Z - 2)				//インデックス数

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
MeshPlaneRenderer::MeshPlaneRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerBack;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 1;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//色の設定
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//デフォルト値設定
	m_nNumBlockX = 1;			//横の分割数
	m_nNumBlockZ = 1;			//縦の分割数
	m_fWidth = 1.0f;			//幅
	m_fHeight = 1.0f;			//高さ
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//ポリゴン1枚あたりの幅
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//ポリゴン1枚あたりの高さ
	
	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX( m_nNumBlockX, m_nNumBlockZ),				//作成したい頂点バッファのサイズ
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
		sizeof( WORD) * NUM_INDEX( m_nNumBlockX, m_nNumBlockZ),		 //サイズ（WORD or DWORD）*インデックス数
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
void MeshPlaneRenderer::Uninit( void)
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
void MeshPlaneRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//マテリアル（シェーダー）をセット
	m_pMaterial->Set( pCamera, this);

	//頂点情報設定
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_3D));

	//インデックスバッファの設定
	pDevice->SetIndices( m_pIdxBuff);

	//テクニック開始
	m_pMaterial->Begin( m_nPass);

	//プリミティブ（ポリゴン・図形）の描画
	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,	0, 0, 
		NUM_VERTEX( m_nNumBlockX, m_nNumBlockZ), 0,	NUM_POLYGON( m_nNumBlockX, m_nNumBlockZ));

	//テクニック終了
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	頂点バッファ設定
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetVtxBuffer( void)
{
	//頂点バッファをロックして、仮想アドレスを取得する
	VERTEX_3D* pVtx;				//仮想アドレス用ポインタ
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx,0);

	//変数宣言
	int nCntHeight = 0;		//たてカウンタ
	int nCntWidth = 0;		//よこカウンタ

	//ポリゴンの設定
	for( nCntHeight = 0; nCntHeight < m_nNumBlockZ + 1; nCntHeight++)
	{
		for( nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			//頂点座標の設定
			pVtx[ 0].Pos = D3DXVECTOR3(
				m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
				0.0f,
				m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
			
			//法線の設定
			pVtx[ 0].Normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
			
			//頂点カラーの設定
			pVtx[ 0].Color = m_Color;
			
			//UVデータの設定
			pVtx[ 0].Tex = D3DXVECTOR2( 1.0f * nCntWidth, 1.0f * nCntHeight);
			
			//ポインタをずらす
			pVtx += 1;
		}
	}

	//アンロック
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	インデックスバッファ設定
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetIdxBuffer(void)
{
	//頂点バッファをロックして、仮想アドレスを取得する
	WORD* pIdx;		//インデックス情報
	m_pIdxBuff->Lock( 0, 0, (void**)&pIdx, 0);

	//変数宣言
	int nCntHeight = 0;		//たてカウンタ
	int nCntWidth = 0;		//よこカウンタ

	//インデックス情報の設定
	for( nCntHeight = 0; nCntHeight < m_nNumBlockZ; nCntHeight++)
	{
		//表示するポリゴンのインデックス情報の設定
		for( nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			pIdx[ 0] = ( nCntHeight + 1) * ( m_nNumBlockX + 1) + nCntWidth;
			pIdx[ 1] = ( nCntHeight + 0) * ( m_nNumBlockX + 1) + nCntWidth;

			pIdx += 2;
		}

		//最後は縮退ポリゴン不要
		if( nCntHeight == m_nNumBlockZ - 1)
		{
			break;
		}

		//縮退ポリゴンのインデックス情報の設定
		pIdx[ 0] = nCntHeight * ( m_nNumBlockX + 1) + nCntWidth - 1;
		pIdx[ 1] = ( nCntHeight + 2) * ( m_nNumBlockX + 1);
		
		pIdx += 2;
	}

	//アンロック
	m_pIdxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	分割数を設定（1枚当たりの大きさ変えない）
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetBlockNum( int X, int Z)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//設定
	m_nNumBlockX = X;
	m_nNumBlockZ = Z;
	m_fWidth = m_nNumBlockX * m_fBlockWidth;
	m_fHeight = m_nNumBlockZ * m_fBlockHeight;
	

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
	
	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX( m_nNumBlockX, m_nNumBlockZ),				//作成したい頂点バッファのサイズ
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
		sizeof( WORD) * NUM_INDEX( m_nNumBlockX, m_nNumBlockZ),		 //サイズ（WORD or DWORD）*インデックス数
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
}

/*------------------------------------------------------------------------------
	ポリゴン一枚あたりの幅・高さを設定
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetBlockSize( float Width, float Height)
{
	//設定
	m_fBlockWidth = Width;
	m_fBlockHeight = Height;
	m_fWidth = m_nNumBlockX * m_fBlockWidth;
	m_fHeight = m_nNumBlockZ * m_fBlockHeight;
	
	//頂点バッファ設定
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	分割数を設定
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetNum( int X, int Z)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//設定
	m_nNumBlockX = X;			//横の分割数
	m_nNumBlockZ = Z;			//縦の分割数
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//ポリゴン1枚あたりの幅
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//ポリゴン1枚あたりの高さ

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
	
	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX( m_nNumBlockX, m_nNumBlockZ),				//作成したい頂点バッファのサイズ
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
		sizeof( WORD) * NUM_INDEX( m_nNumBlockX, m_nNumBlockZ),		 //サイズ（WORD or DWORD）*インデックス数
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
}

/*------------------------------------------------------------------------------
	幅・高さを設定
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetSize( float Width, float Height)
{
	//設定
	m_fWidth = Width;
	m_fHeight = Height;
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//ポリゴン1枚あたりの幅
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//ポリゴン1枚あたりの高さ
	
	//頂点バッファ設定
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	テクスチャ設定
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	シェーダー設定
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}
