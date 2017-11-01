/*==============================================================================

    InputKeyboard.h - キーボード入力
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/6
==============================================================================*/
#ifndef _INPUT_KEYBOARD_H_
#define _INPUT_KEYBOARD_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Input.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define	NUM_KEY_MAX		(256)	//キー最大数

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class InputKeyboard : public Input
{
public:
	InputKeyboard( HINSTANCE hInst, HWND hWnd);
	~InputKeyboard();
	void Update( HWND hWnd);

	BOOL GetKeyPress( int nKey);
	BOOL GetKeyTrigger( int nKey);
	BOOL GetKeyRelease( int nKey);
	BOOL GetKeyRepeat( int nKey);
	void FlushKeyTrigger( int nKey);

	const Vector2 Get2DDirection();
	const Vector3 Get3DDirection();

private:
	BYTE m_aKeyState[ NUM_KEY_MAX];				//キーボードの状態を受け取るワーク
	BYTE m_aKeyStateTrigger[ NUM_KEY_MAX];		//トリガーワーク
	BYTE m_aKeyStateRelease[ NUM_KEY_MAX];		//リリースワーク
	BYTE m_aKeyStateRepeat[ NUM_KEY_MAX];		//リピートワーク
	int m_aKeyStateRepeatCnt[ NUM_KEY_MAX];		//リピートカウンタ
};

#endif

