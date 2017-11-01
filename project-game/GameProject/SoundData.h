/*==============================================================================

    SoundData.h - サウンドデータ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/18
==============================================================================*/
#ifndef _SOUND_DATA_H_
#define _SOUND_DATA_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class SoundData
{
public:
	SoundData( std::string FileName, int nCntLoop, IXAudio2 *pXAudio2);
	~SoundData();

	void Play( void);
	void Stop(void);
	
private:
	int m_nCntLoop;							//ループカウント（-1で無限ループ　0で一回　1以上で指定回）IXAudio2SourceVoice *g_apSourceVoice;		//ソースボイス
	IXAudio2SourceVoice *m_pSourceVoice;	//ソースボイス
	BYTE *m_pDataAudio;						//オーディオデータ
	DWORD m_SizeAudio;						//オーディオデータサイズ
	
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
};


#endif