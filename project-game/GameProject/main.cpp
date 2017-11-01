/*==============================================================================
	
	Main.cpp - ÉÅÉCÉìèàóù
														Author : Yutaka Suganuma
														Date   : 2017/3/25
==============================================================================*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	ÉÅÉCÉì
	à¯êî
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine
		int nCmdShow
	ñﬂÇËíl
		(int)msg.wParam
------------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	new Game(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	Game::Ran();

	return 0;
}

