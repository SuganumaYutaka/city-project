/*==============================================================================

    SoundData.h - �T�E���h�f�[�^
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/18
==============================================================================*/
#ifndef _SOUND_DATA_H_
#define _SOUND_DATA_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class SoundData
{
public:
	SoundData( std::string FileName, int nCntLoop, IXAudio2 *pXAudio2);
	~SoundData();

	void Play( void);
	void Stop(void);
	
private:
	int m_nCntLoop;							//���[�v�J�E���g�i-1�Ŗ������[�v�@0�ň��@1�ȏ�Ŏw���jIXAudio2SourceVoice *g_apSourceVoice;		//�\�[�X�{�C�X
	IXAudio2SourceVoice *m_pSourceVoice;	//�\�[�X�{�C�X
	BYTE *m_pDataAudio;						//�I�[�f�B�I�f�[�^
	DWORD m_SizeAudio;						//�I�[�f�B�I�f�[�^�T�C�Y
	
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
};


#endif