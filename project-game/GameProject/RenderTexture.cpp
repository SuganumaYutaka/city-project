/*==============================================================================

    RenderTexture.cpp - レンダーテクスチャ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/8
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "RenderTexture.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
//#define TEXTURE_WIDTH (1024)
//#define TEXTURE_HEIGHT (1024)
#define TEXTURE_WIDTH (2048)
#define TEXTURE_HEIGHT (2048)

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
RenderTexture::RenderTexture( bool isBuckBuffer)
{
	auto pDevice = Manager::GetDevice();

	//バックバッファかどうか
	m_bBuckBuffer = isBuckBuffer;

	//バックバッファの場合
	if (isBuckBuffer == true)
	{
		m_pTexture = NULL;
		pDevice->GetRenderTarget( 0, &m_pTexSurface);
		pDevice->GetDepthStencilSurface( &m_pTexZ);
		return;
	}

	//レンダーテクスチャの作成
	HRESULT hr;
	hr = pDevice->CreateTexture(
		SCREEN_WIDTH, SCREEN_HEIGHT,		//テクスチャサイズ
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
		&m_pTexture, NULL);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "レンダーテクスチャの作成に失敗しました\n", "エラー", MB_OK);
		return;
	}

	//テクスチャサーフェスの取得
	m_pTexture->GetSurfaceLevel( 0, &m_pTexSurface);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "テクスチャサーフェスの取得に失敗しました\n", "エラー", MB_OK);
		return;
	}

	//深度バッファの作成
	hr = pDevice->CreateDepthStencilSurface(
		SCREEN_WIDTH, SCREEN_HEIGHT,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE,
		&m_pTexZ, NULL);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "深度バッファの作成に失敗しました\n", "エラー", MB_OK);
		return;
	}

	//テクスチャサイズの設定
	m_Size.x = SCREEN_WIDTH;
	m_Size.y = SCREEN_HEIGHT;
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
RenderTexture::~RenderTexture()
{
	if( m_bBuckBuffer == false)
	{
		SAFE_RELEASE( m_pTexture);
		SAFE_RELEASE( m_pTexSurface);
		SAFE_RELEASE( m_pTexZ);
	}
}

/*------------------------------------------------------------------------------
	深度バッファ描画用フォーマットに変更
------------------------------------------------------------------------------*/
bool RenderTexture::ChangeDepthRenderFormat( void)
{
	//テクスチャの破棄
	SAFE_RELEASE( m_pTexture);
	SAFE_RELEASE( m_pTexSurface);
	SAFE_RELEASE( m_pTexZ);

	auto pDevice = Manager::GetDevice();

	//レンダーテクスチャの作成
	HRESULT hr;
	hr = pDevice->CreateTexture(
		TEXTURE_WIDTH, TEXTURE_HEIGHT,		//テクスチャサイズ
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT,
		&m_pTexture, NULL);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "レンダーテクスチャの作成に失敗しました\n", "エラー", MB_OK);
		return false;
	}

	//テクスチャサーフェスの取得
	m_pTexture->GetSurfaceLevel( 0, &m_pTexSurface);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "テクスチャサーフェスの取得に失敗しました\n", "エラー", MB_OK);
		return false;
	}

	//深度バッファの作成
	hr = pDevice->CreateDepthStencilSurface(
		TEXTURE_WIDTH, TEXTURE_HEIGHT,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE,
		&m_pTexZ, NULL);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "深度バッファの作成に失敗しました\n", "エラー", MB_OK);
		return false;
	}

	//テクスチャサイズの設定
	m_Size.x = TEXTURE_WIDTH;
	m_Size.y = TEXTURE_HEIGHT;

	return true;
}