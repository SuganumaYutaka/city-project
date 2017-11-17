/*==============================================================================

    InputMouse.h - マウス入力
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/23
==============================================================================*/
#ifndef _INPUT_MOUSE_H_
#define _INPUT_MOUSE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Game.h"
#include "Input.h"
#include "Vector2.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
//#define	NUM_KEY_MAX		(256)	//キー最大数

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class InputMouse : public Input
{
public:
	InputMouse( HINSTANCE hInst, HWND hWnd);
	~InputMouse();
	void Update( HWND hWnd);

	BOOL GetClickPress( int nCntButton);
	BOOL GetClickTrigger( int nCntButton);
	BOOL GetClickRelease( int nCntButton);
	
	LONG GetAxisX( void);
	LONG GetAxisY( void);
	LONG GetWheel( void);
	Vector2 GetScreenPosition(void) { return Vector2(m_posMouseWorld.x, m_posMouseWorld.y);}

	//const Vector2 Get2DDirection();
	//const Vector3 Get3DDirection();

private:
	DIMOUSESTATE2 m_MouseState;					//マウスの状態
	DIMOUSESTATE2 m_MouseStateTrigger;			//マウスの状態
	DIMOUSESTATE2 m_MouseStateRelease;			//マウスの状態
	POINT m_posMouseWorld;						//マウスの位置

};

#endif

#pragma once
