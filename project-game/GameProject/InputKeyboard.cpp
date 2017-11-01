/*==============================================================================

    InputKeyboard.cpp - キーボード入力
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/6
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "InputKeyboard.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define	COUNT_WAIT_REPEAT	(20)	// リピート待ち時間

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
InputKeyboard::InputKeyboard( HINSTANCE hInst, HWND hWnd)
{
	//各ワークのクリア
	ZeroMemory(m_aKeyState, sizeof m_aKeyState);
	ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
	ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
	ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
	ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);

	//入力処理の初期化
	Input::Init( hInst, hWnd);

	HRESULT hr;

	//デバイスオブジェクトを作成
	hr = m_pInput->CreateDevice( GUID_SysKeyboard, &m_pInputDevice, NULL);
	if( FAILED( hr) || m_pInputDevice == NULL)
	{
		assert( false);
		return;
	}

	//データフォーマットを設定
	hr = m_pInputDevice->SetDataFormat( &c_dfDIKeyboard);
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

	//m_pInputDeviceアクセス権を獲得(入力制御開始)
	m_pInputDevice->Acquire();
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
InputKeyboard::~InputKeyboard()
{

}

/*------------------------------------------------------------------------------
	更新
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

	//デバイスからデータを取得
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
	キーボードデータ取得(プレス)
------------------------------------------------------------------------------*/
BOOL InputKeyboard::GetKeyPress(int nKey)
{
	return ( m_aKeyState[ nKey] & 0x80) ? TRUE : FALSE;
}

/*------------------------------------------------------------------------------
	キーボードデータ取得(トリガー)
------------------------------------------------------------------------------*/
BOOL InputKeyboard::GetKeyTrigger(int nKey)
{
	return ( m_aKeyStateTrigger[ nKey] & 0x80) ? TRUE : FALSE;
}

/*------------------------------------------------------------------------------
	キーボードデータ取得(リリース)
------------------------------------------------------------------------------*/
BOOL InputKeyboard::GetKeyRelease(int nKey)
{
	return ( m_aKeyStateRelease[ nKey] & 0x80) ? TRUE : FALSE;
}

/*------------------------------------------------------------------------------
	キーボードデータ取得(リピート)
------------------------------------------------------------------------------*/
BOOL InputKeyboard::GetKeyRepeat(int nKey)
{
	return ( m_aKeyStateRepeat[ nKey] & 0x80) ? TRUE : FALSE;
}

/*------------------------------------------------------------------------------
	キーボードデータ削除(トリガー)
------------------------------------------------------------------------------*/
void InputKeyboard::FlushKeyTrigger(int nKey)
{
	m_aKeyStateTrigger[ nKey] = 0;
}

/*------------------------------------------------------------------------------
	方向取得（2D）
	引数
	void
	戻り値
	const Vector2
------------------------------------------------------------------------------*/
const Vector2 InputKeyboard::Get2DDirection()
{
	Vector2 Dir;		//方向ベクトル
	Dir.x += (float)GetKeyPress( DIK_D);
	Dir.y += (float)GetKeyPress( DIK_S);
	Dir.x -= (float)GetKeyPress( DIK_A);
	Dir.y -= (float)GetKeyPress( DIK_W);

	return Dir;
}

/*------------------------------------------------------------------------------
	方向取得（3D）
	引数
	void
	戻り値
	const Vector3
------------------------------------------------------------------------------*/
const Vector3 InputKeyboard::Get3DDirection()
{
	Vector3 Dir;		//方向ベクトル
	Dir.x += (float)GetKeyPress( DIK_D);
	Dir.z += (float)GetKeyPress( DIK_W);
	Dir.x -= (float)GetKeyPress( DIK_A);
	Dir.z -= (float)GetKeyPress( DIK_S);

	return Dir;
}