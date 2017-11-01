/*==============================================================================
	
	Main.cpp - メイン処理
														Author : Yutaka Suganuma
														Date   : 2017/3/25
==============================================================================*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	メイン
	引数
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine
		int nCmdShow
	戻り値
		(int)msg.wParam
------------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	new Game(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	Game::Ran();

	return 0;
}

