/*==============================================================================

    SoundData.cpp - サウンドデータ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/18
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "SoundData.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
SoundData::SoundData(std::string fileName, int nCntLoop, IXAudio2 *pXAudio2)
{
	HRESULT hr;
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	//バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	//サウンドデータファイルの生成
	hFile = CreateFile(FileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		assert(false);
		return;
	}

	//ファイルポインタを先頭に移動
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		assert(false);
		return;
	}
	
	//WAVEファイルのチェック
	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if(FAILED(hr))
	{
		assert(false);
		return;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if(FAILED(hr))
	{
		assert(false);
		return;
	}
	if(dwFiletype != 'EVAW')
	{
		assert(false);
		return;
	}
	
	//フォーマットチェック
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if(FAILED(hr))
	{
		assert(false);
		return;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if(FAILED(hr))
	{
		assert(false);
		return;
	}

	//オーディオデータ読み込み
	hr = CheckChunk(hFile, 'atad', &m_SizeAudio, &dwChunkPosition);
	if(FAILED(hr))
	{
		assert(false);
		return;
	}
	m_pDataAudio = (BYTE*)malloc(m_SizeAudio);
	hr = ReadChunkData(hFile, m_pDataAudio, m_SizeAudio, dwChunkPosition);
	if(FAILED(hr))
	{
		assert(false);
		return;
	}
	
	// ソースボイスの生成
	hr = pXAudio2->CreateSourceVoice(&m_pSourceVoice, &(wfx.Format));
	if(FAILED(hr))
	{
		assert(false);
		return;
	}

	//バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_SizeAudio;
	buffer.pAudioData = m_pDataAudio;
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = m_nCntLoop;

	//オーディオバッファの登録
	m_pSourceVoice->SubmitSourceBuffer(&buffer);

	//ループ回数設定
	m_nCntLoop = nCntLoop;

	FileName = fileName;
}


/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
SoundData::~SoundData()
{
	//停止
	m_pSourceVoice->Stop(0);
	
	//ソースボイスの破棄
	m_pSourceVoice->DestroyVoice();
	m_pSourceVoice = NULL;
	
	//オーディオデータの解放
	free(m_pDataAudio);
	m_pDataAudio = NULL;
}

/*------------------------------------------------------------------------------
	再生
------------------------------------------------------------------------------*/
void SoundData::Play(void)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	//バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_SizeAudio;
	buffer.pAudioData = m_pDataAudio;
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = m_nCntLoop;

	//状態取得
	m_pSourceVoice->GetState(&xa2state);
	
	// 再生中
	if(xa2state.BuffersQueued != 0)
	{
		//一時停止
		m_pSourceVoice->Stop(0);

		//オーディオバッファの削除
		m_pSourceVoice->FlushSourceBuffers();
	}

	//オーディオバッファの登録
	m_pSourceVoice->SubmitSourceBuffer(&buffer);

	//再生
	m_pSourceVoice->Start(0);
}

/*------------------------------------------------------------------------------
	停止
------------------------------------------------------------------------------*/
void SoundData::Stop(void)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_pSourceVoice->GetState(&xa2state);
	
	//再生中
	if(xa2state.BuffersQueued != 0)
	{
		//一時停止
		m_pSourceVoice->Stop(0);

		//オーディオバッファの削除
		m_pSourceVoice->FlushSourceBuffers();
	}
}

/*------------------------------------------------------------------------------
	チャンクのチェック
------------------------------------------------------------------------------*/
HRESULT SoundData::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	//ファイルポインタを先頭に移動
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		//チャンクの読み込み
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		//チャンクデータの読み込み
		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;

			//ファイルタイプの読み込み
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			//ファイルポインタをチャンクデータ分移動
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

/*------------------------------------------------------------------------------
	チャンクデータの読み込み
------------------------------------------------------------------------------*/
HRESULT SoundData::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	//ファイルポインタを指定位置まで移動
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	//データの読み込み
	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

