/*==============================================================================
	
	Manager.h - ウィンドウの設定・ゲームループ処理
														Author : Yutaka Suganuma
														Date   : 2017/3/28
==============================================================================*/
#ifndef _GAME_H_
#define _GAME_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include <Windows.h>

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Manager;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define SCREEN_WIDTH	(1200)				//ウインドウの幅
#define SCREEN_HEIGHT	(675)				//ウインドウ高さ

#define SAFE_RELEASE(p) { if(p){ (p)->Release(); (p) = NULL;}}		//セーフリリースマクロ

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Game
{
public:
	Game(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	~Game();
	static void Ran(void);
	static bool IsRan(void);
	static void Finish(void);

	static int GetFPS( void) {return m_This->m_nFPS;}

private:
	static Game *m_This;				//thisポインタ
	static bool m_bRan;					//ゲームループフラグ
	HINSTANCE m_hInstance;				//インスタンス
	HWND m_hWnd;						//ウインドウのハンドル
	BOOL m_bWindow;						//フルスクリーンフラグ
	MSG m_Msg;							//メッセージ
	int m_nFPS;							//FPS
	Manager *m_pManager;				//マネージャーポインタ

	void Init( void);
	void Uninit( void);
	void Update( void);
	void Draw( void);
};




#endif