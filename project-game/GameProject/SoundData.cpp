/*==============================================================================

    SoundData.cpp - �T�E���h�f�[�^
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/18
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "SoundData.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
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

	//�o�b�t�@�̃N���A
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	//�T�E���h�f�[�^�t�@�C���̐���
	hFile = CreateFile(FileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		assert(false);
		return;
	}

	//�t�@�C���|�C���^��擪�Ɉړ�
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		assert(false);
		return;
	}
	
	//WAVE�t�@�C���̃`�F�b�N
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
	
	//�t�H�[�}�b�g�`�F�b�N
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

	//�I�[�f�B�I�f�[�^�ǂݍ���
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
	
	// �\�[�X�{�C�X�̐���
	hr = pXAudio2->CreateSourceVoice(&m_pSourceVoice, &(wfx.Format));
	if(FAILED(hr))
	{
		assert(false);
		return;
	}

	//�o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_SizeAudio;
	buffer.pAudioData = m_pDataAudio;
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = m_nCntLoop;

	//�I�[�f�B�I�o�b�t�@�̓o�^
	m_pSourceVoice->SubmitSourceBuffer(&buffer);

	//���[�v�񐔐ݒ�
	m_nCntLoop = nCntLoop;

	FileName = fileName;
}


/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
SoundData::~SoundData()
{
	//��~
	m_pSourceVoice->Stop(0);
	
	//�\�[�X�{�C�X�̔j��
	m_pSourceVoice->DestroyVoice();
	m_pSourceVoice = NULL;
	
	//�I�[�f�B�I�f�[�^�̉��
	free(m_pDataAudio);
	m_pDataAudio = NULL;
}

/*------------------------------------------------------------------------------
	�Đ�
------------------------------------------------------------------------------*/
void SoundData::Play(void)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	//�o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_SizeAudio;
	buffer.pAudioData = m_pDataAudio;
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = m_nCntLoop;

	//��Ԏ擾
	m_pSourceVoice->GetState(&xa2state);
	
	// �Đ���
	if(xa2state.BuffersQueued != 0)
	{
		//�ꎞ��~
		m_pSourceVoice->Stop(0);

		//�I�[�f�B�I�o�b�t�@�̍폜
		m_pSourceVoice->FlushSourceBuffers();
	}

	//�I�[�f�B�I�o�b�t�@�̓o�^
	m_pSourceVoice->SubmitSourceBuffer(&buffer);

	//�Đ�
	m_pSourceVoice->Start(0);
}

/*------------------------------------------------------------------------------
	��~
------------------------------------------------------------------------------*/
void SoundData::Stop(void)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	m_pSourceVoice->GetState(&xa2state);
	
	//�Đ���
	if(xa2state.BuffersQueued != 0)
	{
		//�ꎞ��~
		m_pSourceVoice->Stop(0);

		//�I�[�f�B�I�o�b�t�@�̍폜
		m_pSourceVoice->FlushSourceBuffers();
	}
}

/*------------------------------------------------------------------------------
	�`�����N�̃`�F�b�N
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
	
	//�t�@�C���|�C���^��擪�Ɉړ�
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		//�`�����N�̓ǂݍ���
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		//�`�����N�f�[�^�̓ǂݍ���
		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;

			//�t�@�C���^�C�v�̓ǂݍ���
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			//�t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
	�`�����N�f�[�^�̓ǂݍ���
------------------------------------------------------------------------------*/
HRESULT SoundData::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	//�t�@�C���|�C���^���w��ʒu�܂ňړ�
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	//�f�[�^�̓ǂݍ���
	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

