/*==============================================================================

    InputMouse.cpp - キーボード入力
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/6
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "InputMouse.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define	COUNT_WAIT_REPEAT	(20)	// リピート待ち時間

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
InputMouse::InputMouse( HINSTANCE hInst, HWND hWnd)
{
	//入力処理の初期化
	Input::Init( hInst, hWnd);

	HRESULT hr;

	//デバイスオブジェクトを作成
	hr = m_pInput->CreateDevice( GUID_SysMouse, &m_pInputDevice, NULL);
	if( FAILED( hr))
	{
		assert( false);
		return;
	}

	//データフォーマットを設定
	hr = m_pInputDevice->SetDataFormat( &c_dfDIMouse2);
	if( FAILED( hr))
	{
		assert( false);
		return;
	}

	//協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pInputDevice->SetCooperativeLevel( hWnd, ( DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if( FAILED( hr))
	{
		assert( false);
		return;
	}

	//デバイスの設定
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof( dipdw);
	dipdw.diph.dwHeaderSize = sizeof( dipdw.diph);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_REL;		//相対座標

	hr = m_pInputDevice->SetProperty( DIPROP_AXISMODE, &dipdw.diph);
	if( FAILED( hr))
	{
		assert( false);
		return;
	}

	//m_pInputDeviceアクセス権を獲得(入力制御開始)
	m_pInputDevice->Acquire();
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
InputMouse::~InputMouse()
{

}

/*------------------------------------------------------------------------------
	更新
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

	//デバイスからデータを取得
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

		//スクリーン座標を取得
		GetCursorPos( &m_posMouseWorld);
		ScreenToClient( hWnd, &m_posMouseWorld);
	}

	else
	{
		//m_pInputDeviceアクセス権を獲得(入力制御開始)
		m_pInputDevice->Acquire();
	}
}

/*------------------------------------------------------------------------------
	クリックデータ取得(プレス)
------------------------------------------------------------------------------*/
BOOL InputMouse::GetClickPress( int nCntButton)
{
	return (m_MouseState.rgbButtons[ nCntButton] & 0x80) ? true: false;
}

/*------------------------------------------------------------------------------
	クリックデータ取得(トリガー)
------------------------------------------------------------------------------*/
BOOL InputMouse::GetClickTrigger( int nCntButton)
{
	return (m_MouseStateTrigger.rgbButtons[ nCntButton] & 0x80) ? true: false;
}

/*------------------------------------------------------------------------------
	クリックデータ取得(リリース)
------------------------------------------------------------------------------*/
BOOL InputMouse::GetClickRelease( int nCntButton)
{
	return (m_MouseStateRelease.rgbButtons[ nCntButton] & 0x80) ? true: false;
}

/*------------------------------------------------------------------------------
	マウスデータ取得（X軸移動量）
------------------------------------------------------------------------------*/
LONG InputMouse::GetAxisX( void)
{
	return m_MouseState.lX;
}

/*------------------------------------------------------------------------------
	マウスデータ取得（Y軸移動量）
------------------------------------------------------------------------------*/
LONG InputMouse::GetAxisY( void)
{
	return m_MouseState.lY;
}

/*------------------------------------------------------------------------------
	マウスデータ取得（ホイール）奥が : 手前が
------------------------------------------------------------------------------*/
LONG InputMouse::GetWheel( void)
{
	return m_MouseState.lZ;
}

