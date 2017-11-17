/*==============================================================================

    InputMouse.h - �}�E�X����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/23
==============================================================================*/
#ifndef _INPUT_MOUSE_H_
#define _INPUT_MOUSE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Game.h"
#include "Input.h"
#include "Vector2.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
//#define	NUM_KEY_MAX		(256)	//�L�[�ő吔

/*------------------------------------------------------------------------------
	�N���X��`
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
	DIMOUSESTATE2 m_MouseState;					//�}�E�X�̏��
	DIMOUSESTATE2 m_MouseStateTrigger;			//�}�E�X�̏��
	DIMOUSESTATE2 m_MouseStateRelease;			//�}�E�X�̏��
	POINT m_posMouseWorld;						//�}�E�X�̈ʒu

};

#endif

#pragma once
