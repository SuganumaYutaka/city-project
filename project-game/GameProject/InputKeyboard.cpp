/*==============================================================================

    InputKeyboard.cpp - �L�[�{�[�h����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/6
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "InputKeyboard.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define	COUNT_WAIT_REPEAT	(20)	// ���s�[�g�҂�����

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
InputKeyboard::InputKeyboard( HINSTANCE hInst, HWND hWnd)
{
	//�e���[�N�̃N���A
	ZeroMemory(m_aKeyState, sizeof m_aKeyState);
	ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
	ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
	ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
	ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);

	//���͏����̏�����
	Input::Init( hInst, hWnd);

	HRESULT hr;

	//�f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pInput->CreateDevice( GUID_SysKeyboard, &m_pInputDevice, NULL);
	if( FAILED( hr) || m_pInputDevice == NULL)
	{
		assert( false);
		return;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pInputDevice->SetDataFormat( &c_dfDIKeyboard);
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

	//m_pInputDevice�A�N�Z�X�����l��(���͐���J�n)
	m_pInputDevice->Acquire();
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
InputKeyboard::~InputKeyboard()
{

}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void InputKeyboard::Update( HWND hWnd)
{
	HRESULT hr;
	BYTE aKeyState[ NUM_KEY_MAX];

	if( m_pInputDevice == NULL)
	{
		assert( false);
		return;
	}

	//�f�o�C�X����f�[�^���擾
	hr = m_pInputDevice->GetDeviceState( sizeof( aKeyState), aKeyState);
	if( SUCCEEDED( hr))
	{
		for( int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[ nCntKey] = ( m_aKeyState[ nCntKey] ^ aKeyState[ nCntKey]) & aKeyState[ nCntKey];
			m_aKeyStateRelease[ nCntKey] = ( m_aKeyState[ nCntKey] ^ aKeyState[ nCntKey]) & ~aKeyState[ nCntKey];
			m_aKeyStateRepeat[ nCntKey] = m_aKeyStateTrigger[ nCntKey];

			if( aKeyState[ nCntKey])
			{
				m_aKeyStateRepeatCnt[ nCntKey]++;
				if( m_aKeyStateRepeatCnt[ nCntKey] >= COUNT_WAIT_REPEAT)
				{
					m_aKeyStateRepeat[ nCntKey] = aKeyState[ nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCnt[ nCntKey] = 0;
				m_aKeyStateRepeat[ nCntKey] = 0;
			}

			m_aKeyState[ nCntKey] = aKeyState[ nCntKey];
		}
	}
	else
	{
		m_pInputDevice->Acquire();
	}
}

/*------------------------------------------------------------------------------
	�L�[�{�[�h�f�[�^�擾(�v���X)
------------------------------------------------------------------------------*/
BOOL InputKeyboard::GetKeyPress(int nKey)
{
	return ( m_aKeyState[ nKey] & 0x80) ? TRUE : FALSE;
}

/*------------------------------------------------------------------------------
	�L�[�{�[�h�f�[�^�擾(�g���K�[)
------------------------------------------------------------------------------*/
BOOL InputKeyboard::GetKeyTrigger(int nKey)
{
	return ( m_aKeyStateTrigger[ nKey] & 0x80) ? TRUE : FALSE;
}

/*------------------------------------------------------------------------------
	�L�[�{�[�h�f�[�^�擾(�����[�X)
------------------------------------------------------------------------------*/
BOOL InputKeyboard::GetKeyRelease(int nKey)
{
	return ( m_aKeyStateRelease[ nKey] & 0x80) ? TRUE : FALSE;
}

/*------------------------------------------------------------------------------
	�L�[�{�[�h�f�[�^�擾(���s�[�g)
------------------------------------------------------------------------------*/
BOOL InputKeyboard::GetKeyRepeat(int nKey)
{
	return ( m_aKeyStateRepeat[ nKey] & 0x80) ? TRUE : FALSE;
}

/*------------------------------------------------------------------------------
	�L�[�{�[�h�f�[�^�폜(�g���K�[)
------------------------------------------------------------------------------*/
void InputKeyboard::FlushKeyTrigger(int nKey)
{
	m_aKeyStateTrigger[ nKey] = 0;
}

/*------------------------------------------------------------------------------
	�����擾�i2D�j
	����
	void
	�߂�l
	const Vector2
------------------------------------------------------------------------------*/
const Vector2 InputKeyboard::Get2DDirection()
{
	Vector2 Dir;		//�����x�N�g��
	Dir.x += (float)GetKeyPress( DIK_D);
	Dir.y += (float)GetKeyPress( DIK_S);
	Dir.x -= (float)GetKeyPress( DIK_A);
	Dir.y -= (float)GetKeyPress( DIK_W);

	return Dir;
}

/*------------------------------------------------------------------------------
	�����擾�i3D�j
	����
	void
	�߂�l
	const Vector3
------------------------------------------------------------------------------*/
const Vector3 InputKeyboard::Get3DDirection()
{
	Vector3 Dir;		//�����x�N�g��
	Dir.x += (float)GetKeyPress( DIK_D);
	Dir.z += (float)GetKeyPress( DIK_W);
	Dir.x -= (float)GetKeyPress( DIK_A);
	Dir.z -= (float)GetKeyPress( DIK_S);

	return Dir;
}