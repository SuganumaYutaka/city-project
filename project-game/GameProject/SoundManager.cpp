/*==============================================================================

    SoundManager.cpp - サウンド管理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/17
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "SoundManager.h"
#include "SoundData.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
SoundManager::SoundManager(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		//CoUninitialize();

		assert(false);
		return;
	}
	
	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(m_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		//CoUninitialize();

		assert(false);
		return;
	}
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
SoundManager::~SoundManager()
{
	//データの解放
	for (auto ite = m_mapData.begin(); ite != m_mapData.end(); ++ite)
	{
		delete ite->second;
	}
	m_mapData.clear();

	//マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;
	
	if(m_pXAudio2)
	{
		//XAudio2オブジェクトの解放
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}
	
	//COMライブラリの終了処理
	//CoUninitialize();
}

/*------------------------------------------------------------------------------
	サウンド追加
------------------------------------------------------------------------------*/
SoundData* SoundManager::SetSound(std::string FileName, int nCntLoop)
{
	//すでに存在するか
	if (m_mapData.find(FileName) == m_mapData.end())
	{
		//生成してマップに追加
		m_mapData[ FileName] = new SoundData( FileName, nCntLoop, m_pXAudio2);
	}

	return m_mapData[ FileName];
}



