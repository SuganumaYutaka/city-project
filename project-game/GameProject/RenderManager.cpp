/*==============================================================================

    RenderManager.cpp - 描画（レンダリング）管理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/4
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "RenderManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "DebugRenderer.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"

/*------------------------------------------------------------------------------
	コンストラクタ
	引数
		HWND hWnd
		BOOL bWindow
------------------------------------------------------------------------------*/
RenderManager::RenderManager( HWND hWnd, BOOL bWindow)
{
	//Direct3Dインターフェイスの取得
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION);
	if( m_pD3D == NULL){
		MessageBox( NULL, "main.cpp\nDirect3Dインターフェイスの取得に失敗しました\n", "エラー", MB_OK);
		assert( false);
		return;
	}
	if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm))){
		MessageBox( NULL, "main.cpp\nDirect3Dインターフェイスの取得に失敗しました\n", "エラー", MB_OK);
		assert( false);
		return;
	}

	//デバイスのプレゼントパラメーターの設定（デバイスを作るための設定を設定）
	ZeroMemory( &d3dpp, sizeof( d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//ステンシルバッファ（3年次にここを編集する
	d3dpp.Windowed = TRUE;					//フルスクリーンにする際変更
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//☆Direct3Dデバイスの作成
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice))){
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice))){
			if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice))){
				MessageBox( NULL, "main.cpp\nDirect3Dデバイスの作成に失敗しました\n", "エラー", MB_OK);
				assert( false);
				return;
			}
		}
	}

	//レンダーステート（描画設定）
	//裏面カリング設定
	m_pDevice->SetRenderState( D3DRS_CULLMODE,			//裏面カリング設定
									D3DCULL_CCW);			//逆時計回りが裏面
	//塗りつぶし設定
	m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);			//デフォルト
	//m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//ワイヤーフレーム表示

	//深度バッファを可能に
	m_pDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE);

	//デバッグ表示
#ifdef _DEBUG
	m_pDebugRenderer = new DebugRenderer( m_pDevice);
#endif // _DEBUG

	m_pMainCamera = NULL;
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
RenderManager::~RenderManager()
{
	//デバッグ表示
#ifdef _DEBUG
	if (m_pDebugRenderer != NULL)
	{
		delete m_pDebugRenderer;
		m_pDebugRenderer = NULL;
	}
#endif // _DEBUG

	//Direct3Dデバイスの解放
	if( m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//Direct3Dインターフェイスの解放
	if( m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

/*------------------------------------------------------------------------------
	レンダラーリストに追加
	引数
		Renderer *pRenderer			レンダラー
------------------------------------------------------------------------------*/
void RenderManager::AddRenderer( Renderer *pRenderer)
{
	m_listRenderer.push_back( pRenderer);
}

/*------------------------------------------------------------------------------
	レンダラーリストから削除
	引数
		Renderer *pRenderer			レンダラー
------------------------------------------------------------------------------*/
void RenderManager::ReleaseRenderer( Renderer *pRenderer)
{
	m_listReleaseRenderer.push_back( pRenderer);
}

/*------------------------------------------------------------------------------
	レンダラーリストをソート
------------------------------------------------------------------------------*/
void RenderManager::Sort()
{
	
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void RenderManager::Draw()
{
	//レンダラーリストから削除
	for (auto Renderer : m_listReleaseRenderer)
	{
		for (auto ite = m_listRenderer.begin(); ite != m_listRenderer.end(); ++ite)
		{
			if( *ite == Renderer)
			{
				m_listRenderer.erase(ite);
				break;
			}
		}
	}
	m_listReleaseRenderer.clear();

	//ソート
	Sort();

	//メインカメラがあるかチェック
	if (m_pMainCamera == NULL)
	{
		MessageBox( NULL, "メインカメラがありません\n", "エラー", MB_OK);
		return;
	}

	//カメラ一つにつき一回描画
	for (auto pCamera : m_listCamera)
	{
		//カメラがアクティブか
		if (pCamera->GetActive() == false)
		{
			continue;
		}

		//メインカメラは最後に描画
		if (pCamera == m_pMainCamera)
		{
			continue;
		}

		//カメラのセット（ビューポート、レンダーターゲット、深度バッファ）
		pCamera->SetCamera();

		//描画の開始
		if( SUCCEEDED( m_pDevice->BeginScene()))
		{
			//描画するレイヤー情報取得
			auto List = pCamera->GetRenderLayer();

			//すべて描画
			if (List.size() == 0)
			{
				//レイヤーごとに描画
				for (int nCntLayer = 0; nCntLayer < eLayerNum; nCntLayer++)
				{
					//登録されたレンダラーを描画
					for (Renderer* pRenderer : m_listRenderer)
					{
						if (pRenderer->m_nLayer == nCntLayer)
						{
							if( pRenderer->GetActive() == true)
							{
								pRenderer->Draw( pCamera);
							}
						}
					}
				}
			}

			//特定のレイヤーのみ描画
			else
			{
				//登録されたレイヤーの順に描画
				for (auto nLayer : List)
				{
					//登録されたレンダラーを描画
					for (Renderer* pRenderer : m_listRenderer)
					{
						if (pRenderer->m_nLayer == nLayer)
						{
							if( pRenderer->GetActive() == true)
							{
								pRenderer->Draw( pCamera);
							}
						}
					}
				}
			}

			//描画の終了
			m_pDevice->EndScene();	//１フレームに１回まで
		}
	}

	//メインカメラの描画
	//カメラのセット（ビューポート、レンダーターゲット、深度バッファ）
	m_pMainCamera->SetCamera();
	
	//画面(バックバッファ)のクリア
	m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 255, 150, 0, 255), 1.0f, 0);

	//描画の開始
	if( SUCCEEDED( m_pDevice->BeginScene()))
	{
		//レイヤーごとに描画
		for (int nCntLayer = 0; nCntLayer < eLayerNum; nCntLayer++)
		{
			//登録されたレンダラーを描画
			for (Renderer* pRenderer : m_listRenderer)
			{
				if (pRenderer->m_nLayer == nCntLayer)
				{
					if( pRenderer->GetActive() == true)
					{
						pRenderer->Draw( m_pMainCamera);
					}
				}
			}
		}

		//デバッグ表示
#ifdef _DEBUG
		m_pDebugRenderer->DrawText();
		DebugRenderer::ClearText();
#endif // _DEBUG

		ImGui::Render();

		//描画の終了
		m_pDevice->EndScene();	//１フレームに１回まで
	}

	//フリップ（バックバッファとフロントバッファを入れ替え）
	m_pDevice->Present( NULL, NULL, NULL, NULL);
}


