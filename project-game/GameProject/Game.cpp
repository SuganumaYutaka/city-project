/*==============================================================================
	
	Game.cpp - ウィンドウの設定・ゲームループ処理
														Author : Yutaka Suganuma
														Date   : 2017/3/28
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include <Windows.h>
#include <assert.h>
#include "Manager.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define CLASS_NAME		"Game"				//ウインドウクラス名
#define WINDOW_NAME		"フレーム設計"		//ウインドウ（タイトル）名

/*------------------------------------------------------------------------------
	静的メンバ変数宣言
------------------------------------------------------------------------------*/
Game *Game::m_This = NULL;
bool Game::m_bRan = false;

/*------------------------------------------------------------------------------
	プロトタイプ宣言
------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*------------------------------------------------------------------------------
	コンストラクタ
	引数
		HINSTANCE hInstance
		HINSTANCE hPrevInstance
		LPSTR lpCmdLine
		int nCmdShow
------------------------------------------------------------------------------*/
Game::Game(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//変数を使用しない場合の警告文を表示させない
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//カスタムウインドウの定義
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);			//WNDCLASSEXのメモリサイズ（バイト）を設定
	wcex.style = CS_CLASSDC;					//表示するウインドウのスタイルを設定
	wcex.lpfnWndProc = WndProc;						//ウインドウプロシージャのアドレス（関数名）を指定
	wcex.cbClsExtra = 0;							//通常は使用しないので"0"を指定
	wcex.cbWndExtra = 0;							//通常は使用しないので"0"を指定	
	wcex.hInstance = hInstance;					//WinMainのパラメータのインスタンスハンドル
	wcex.hIcon = NULL;							//○使用するアイコンを指定（Windowsがもっている）
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);	//マウスカーソルを指定
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//○ウインドウのクライアント領域の背景色を設定
	wcex.lpszMenuName = NULL;							//Windowにつけるメニューを設定
	wcex.lpszClassName = CLASS_NAME;					//ウインドウクラスの名前
	wcex.hIconSm = NULL;							//○小さいアイコンが設定された場合の情報を記述

													//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//スクリーンを正しいサイズに
	DWORD style = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);		//ウインドウスタイル
	RECT cr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&cr, style, false);
	int ww = cr.right - cr.left;
	int wh = cr.bottom - cr.top;

	//デスクトップの中心に表示
	RECT dr;
	GetWindowRect(GetDesktopWindow(), &dr);

	int dw = dr.right - dr.left;
	int dh = dr.bottom - dr.top;

	int wx = ww > dw ? 0 : (dw - ww) / 2;
	int wy = wh > dh ? 0 : (dh - wh) / 2;

	//ウインドウを作成
	m_hWnd = CreateWindowEx(0,		//拡張ウインドウスタイル
		CLASS_NAME,					//ウインドウクラスの名前
		WINDOW_NAME,				//ウインドウの名前
		style,						//☆ウインドウスタイル
		wx,							//ウインドウの左上Ｘ座標
		wy,							//ウインドウの左上Ｙ座標
		ww,							//ウインドウの幅（フレーム含める）
		wh,							//ウインドウの高さ（フレーム含める）
		NULL,						//親ウインドウのハンドル
		NULL,						//メニューハンドルまたは子ウインドウID
		hInstance,					//インスタンスハンドル
		NULL);						//ウインドウ作成データ

	//ウインドウを作成（フルスクリーン）
	//m_hWnd = CreateWindowEx(0,		//拡張ウインドウスタイル
	//	CLASS_NAME,					//ウインドウクラスの名前
	//	WINDOW_NAME,				//ウインドウの名前
	//	WS_POPUP,						//☆ウインドウスタイル
	//	CW_USEDEFAULT,
	//	CW_USEDEFAULT,
	//	dw,
	//	dh,
	//	NULL,						//親ウインドウのハンドル
	//	NULL,						//メニューハンドルまたは子ウインドウID
	//	hInstance,					//インスタンスハンドル
	//	NULL);						//ウインドウ作成データ

									//ウインドウの表示
	ShowWindow(m_hWnd, nCmdShow);	//指定されたウインドウの表示状態を設定（ウインドウを表示）
	UpdateWindow(m_hWnd);			//ウインドウの状態を直ちに反映

									//thisポインタの設定
	m_This = this;

	//インスタンスの設定
	m_hInstance = hInstance;

	//フラグの設定
	m_bWindow = TRUE;

	//ゲームループフラグ設定
	m_bRan = true;
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Game::~Game()
{

}

/*------------------------------------------------------------------------------
	ウインドウプロシージャ
	（CALLBACK : Win32API関数を呼び出す時の規約）
	引数
		hWnd	: ウインドウのハンドル
		uMsg	: メッセージの識別子
		wParam	: メッセージの最初のパラメータ
		lParam	: メッセージの2番目のパラメータ（補足情報）

	戻り値
		DefWindowProc( hWnd, uMsg, wParam, lParam)
------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//メッセージ
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		//キーの取得
		switch (wParam)
		{
		case VK_ESCAPE:
			//終了メッセージ
			UINT nID;
			nID = MessageBox(NULL, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON2);
			if (nID == IDYES)
			{
				Game::Finish();
				//DestroyWindow( hWnd);
			}
			break;

		default:
			break;
		}
		break;

		//メニュー処理
	case WM_COMMAND:
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*------------------------------------------------------------------------------
	ゲームループ処理
------------------------------------------------------------------------------*/
void Game::Ran(void)
{
	//ゲームループが設定されているか
	assert(IsRan());

	//fps設定用
	DWORD dwFrameCount = 0;		//
	DWORD dwCurrentTime = 0;	//現在の時間
	DWORD dwExecLastTime = 0;	//前のフレームの実行終了時の時間
	DWORD dwFPSLastTime = 0;	//

	//分解能を設定
	timeBeginPeriod(1);

	//各カウンターの初期化
	dwFrameCount = 0;
	dwCurrentTime = 0;
	dwExecLastTime =				//時間を完全に一致させるためまとめる
		dwFPSLastTime = timeGetTime();	//システム時刻をミリ秒単位で取得

	//初期化処理
	m_This->Init();

	//メッセージループ
	for (;;)
	{
		if (PeekMessage(&m_This->m_Msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			//Windowsの処理
			if (m_This->m_Msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&m_This->m_Msg);		//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&m_This->m_Msg);		//ウインドウプロシージャへメッセージを送出
			}
		}
		else if (!IsRan())
		{
			break;
		}
		else {
			//fps処理
			dwCurrentTime = timeGetTime();		//システム時刻を取得

												//0.5秒ごとに実行
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
#ifdef _DEBUG
				m_This->m_nFPS = ( dwFrameCount * 1000) / ( dwCurrentTime - dwFPSLastTime);	//秒間の平均fpsを取得
#endif	//_DEBUG

				dwFPSLastTime = dwCurrentTime;	//fps計測時間を現在の時間に
				dwFrameCount = 0;		//フレームカウンタリセット
			}

			//60fpsで実行
			if (dwCurrentTime - dwExecLastTime >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;		//(前回の)処理した時間更新

													//更新処理
				m_This->Update();

				//描画処理
				m_This->Draw();

				//フレームカウンタ
				dwFrameCount++;
			}
		}
	}

	//終了処理
	m_This->Uninit();

	//ゲームループ終了
	delete m_This;
}

/*------------------------------------------------------------------------------
	ゲームループが機能しているか
------------------------------------------------------------------------------*/
bool Game::IsRan(void)
{
	return (m_bRan);
}

/*------------------------------------------------------------------------------
	ゲームループ終了命令
------------------------------------------------------------------------------*/
void Game::Finish(void)
{
	//ゲームループが設定されているか
	assert(IsRan());

	//フラグの設定
	m_bRan = false;
}

/*------------------------------------------------------------------------------
	初期化処理
------------------------------------------------------------------------------*/
void Game::Init(void)
{
	//マネージャー生成
	m_pManager = new Manager( m_hInstance, m_hWnd, m_bWindow);
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void Game::Uninit(void)
{
	//マネージャーの破棄
	if( m_pManager != NULL)
	{
		delete m_pManager;
		m_pManager = NULL;
	}
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void Game::Update(void)
{
	m_pManager->Update();
}

/*------------------------------------------------------------------------------
	描画処理
------------------------------------------------------------------------------*/
void Game::Draw(void)
{
	m_pManager->Draw();
}
