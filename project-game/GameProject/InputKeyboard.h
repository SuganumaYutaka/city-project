/*==============================================================================

    InputKeyboard.h - �L�[�{�[�h����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/6
==============================================================================*/
#ifndef _INPUT_KEYBOARD_H_
#define _INPUT_KEYBOARD_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Input.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define	NUM_KEY_MAX		(256)	//�L�[�ő吔

/*------------------------------------------------------------------------------
	�N���X��`
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
	BYTE m_aKeyState[ NUM_KEY_MAX];				//�L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE m_aKeyStateTrigger[ NUM_KEY_MAX];		//�g���K�[���[�N
	BYTE m_aKeyStateRelease[ NUM_KEY_MAX];		//�����[�X���[�N
	BYTE m_aKeyStateRepeat[ NUM_KEY_MAX];		//���s�[�g���[�N
	int m_aKeyStateRepeatCnt[ NUM_KEY_MAX];		//���s�[�g�J�E���^
};

#endif

