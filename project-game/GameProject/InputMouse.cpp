/*==============================================================================

    InputMouse.cpp - �L�[�{�[�h����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/6
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "InputMouse.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define	COUNT_WAIT_REPEAT	(20)	// ���s�[�g�҂�����

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
InputMouse::InputMouse( HINSTANCE hInst, HWND hWnd)
{
	//���͏����̏�����
	Input::Init( hInst, hWnd);

	HRESULT hr;

	//�f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pInput->CreateDevice( GUID_SysMouse, &m_pInputDevice, NULL);
	if( FAILED( hr))
	{
		assert( false);
		return;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pInputDevice->SetDataFormat( &c_dfDIMouse2);
	if( FAILED( hr))
	{
		assert( false);
		return;
	}

	//�������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pInputDevice->SetCooperativeLevel( hWnd, ( DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if( FAILED( hr))
	{
		assert( false);
		return;
	}

	//�f�o�C�X�̐ݒ�
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof( dipdw);
	dipdw.diph.dwHeaderSize = sizeof( dipdw.diph);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_REL;		//���΍��W

	hr = m_pInputDevice->SetProperty( DIPROP_AXISMODE, &dipdw.diph);
	if( FAILED( hr))
	{
		assert( false);
		return;
	}

	//m_pInputDevice�A�N�Z�X�����l��(���͐���J�n)
	m_pInputDevice->Acquire();
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
InputMouse::~InputMouse()
{

}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void InputMouse::Update(HWND hWnd)
{
	HRESULT hr;
	DIMOUSESTATE2 MouseState;

	if( m_pInputDevice == NULL)
	{
		assert( false);
		return;
	}

	//�f�o�C�X����f�[�^���擾
	hr = m_pInputDevice->GetDeviceState( sizeof( MouseState), &MouseState);

	if( SUCCEEDED( hr))
	{
		m_MouseStateTrigger.lX = ((m_MouseState.lX ^ MouseState.lX) & MouseState.lX);
		m_MouseStateTrigger.lY = ((m_MouseState.lY ^ MouseState.lY) & MouseState.lY);
		m_MouseStateTrigger.lZ = ((m_MouseState.lZ ^ MouseState.lZ) & MouseState.lZ);

		m_MouseStateRelease.lX = ((m_MouseState.lX ^ MouseState.lX) & m_MouseState.lX);
		m_MouseStateRelease.lY = ((m_MouseState.lY ^ MouseState.lY) & m_MouseState.lY);
		m_MouseStateRelease.lZ = ((m_MouseState.lZ ^ MouseState.lZ) & m_MouseState.lZ);

		for( int nCntKey = 0; nCntKey < 8; nCntKey++)
		{
			m_MouseStateTrigger.rgbButtons[ nCntKey] = ((m_MouseState.rgbButtons[ nCntKey] ^ MouseState.rgbButtons[ nCntKey]) & MouseState.rgbButtons[ nCntKey]);
			m_MouseStateRelease.rgbButtons[ nCntKey] = ((m_MouseState.rgbButtons[ nCntKey] ^ MouseState.rgbButtons[ nCntKey]) & m_MouseState.rgbButtons[ nCntKey]);
		}

		m_MouseState = MouseState;

		//�X�N���[�����W���擾
		GetCursorPos( &m_posMouseWorld);
		ScreenToClient( hWnd, &m_posMouseWorld);
	}

	else
	{
		//m_pInputDevice�A�N�Z�X�����l��(���͐���J�n)
		m_pInputDevice->Acquire();
	}
}

/*------------------------------------------------------------------------------
	�N���b�N�f�[�^�擾(�v���X)
------------------------------------------------------------------------------*/
BOOL InputMouse::GetClickPress( int nCntButton)
{
	return (m_MouseState.rgbButtons[ nCntButton] & 0x80) ? true: false;
}

/*------------------------------------------------------------------------------
	�N���b�N�f�[�^�擾(�g���K�[)
------------------------------------------------------------------------------*/
BOOL InputMouse::GetClickTrigger( int nCntButton)
{
	return (m_MouseStateTrigger.rgbButtons[ nCntButton] & 0x80) ? true: false;
}

/*------------------------------------------------------------------------------
	�N���b�N�f�[�^�擾(�����[�X)
------------------------------------------------------------------------------*/
BOOL InputMouse::GetClickRelease( int nCntButton)
{
	return (m_MouseStateRelease.rgbButtons[ nCntButton] & 0x80) ? true: false;
}

/*------------------------------------------------------------------------------
	�}�E�X�f�[�^�擾�iX���ړ��ʁj
------------------------------------------------------------------------------*/
LONG InputMouse::GetAxisX( void)
{
	return m_MouseState.lX;
}

/*------------------------------------------------------------------------------
	�}�E�X�f�[�^�擾�iY���ړ��ʁj
------------------------------------------------------------------------------*/
LONG InputMouse::GetAxisY( void)
{
	return m_MouseState.lY;
}

/*------------------------------------------------------------------------------
	�}�E�X�f�[�^�擾�i�z�C�[���j���� : ��O��
------------------------------------------------------------------------------*/
LONG InputMouse::GetWheel( void)
{
	return m_MouseState.lZ;
}

