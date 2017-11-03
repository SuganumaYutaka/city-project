/*==============================================================================

    Input.h - ����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/6
==============================================================================*/
#ifndef _INPUT_H_
#define _INPUT_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Input
{
public:
	virtual ~Input();
	virtual void Update(HWND hWnd) = 0;

protected:
	Input();
	void Init( HINSTANCE hInst, HWND hWnd);
	static LPDIRECTINPUT8 m_pInput;			//DirectInput�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8 m_pInputDevice;	//InputDevice�I�u�W�F�N�g
};

#endif
