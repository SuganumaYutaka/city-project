/*==============================================================================
	
	Main.cpp - ���C������
														Author : Yutaka Suganuma
														Date   : 2017/3/25
==============================================================================*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	���C��
	����
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine
		int nCmdShow
	�߂�l
		(int)msg.wParam
------------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	new Game(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	Game::Ran();

	return 0;
}

