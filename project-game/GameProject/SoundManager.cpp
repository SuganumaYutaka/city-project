/*==============================================================================

    SoundManager.cpp - �T�E���h�Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/17
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "SoundManager.h"
#include "SoundData.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
SoundManager::SoundManager(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		//CoUninitialize();

		assert(false);
		return;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(m_pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		//CoUninitialize();

		assert(false);
		return;
	}
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
SoundManager::~SoundManager()
{
	//�f�[�^�̉��
	for (auto ite = m_mapData.begin(); ite != m_mapData.end(); ++ite)
	{
		delete ite->second;
	}
	m_mapData.clear();

	//�}�X�^�[�{�C�X�̔j��
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;
	
	if(m_pXAudio2)
	{
		//XAudio2�I�u�W�F�N�g�̉��
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}
	
	//COM���C�u�����̏I������
	//CoUninitialize();
}

/*------------------------------------------------------------------------------
	�T�E���h�ǉ�
------------------------------------------------------------------------------*/
SoundData* SoundManager::SetSound(std::string FileName, int nCntLoop)
{
	//���łɑ��݂��邩
	if (m_mapData.find(FileName) == m_mapData.end())
	{
		//�������ă}�b�v�ɒǉ�
		m_mapData[ FileName] = new SoundData( FileName, nCntLoop, m_pXAudio2);
	}

	return m_mapData[ FileName];
}



