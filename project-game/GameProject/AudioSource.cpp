/*==============================================================================

    AudioSourc.cpp - �T�E���h�Đ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/18
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "AudioSource.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "SoundManager.h"
#include "SoundData.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
AudioSource::AudioSource( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_vecData.clear();
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void AudioSource::Uninit( void)
{
	for (auto audio : m_vecData)
	{
		audio->Stop();
	}

	m_vecData.clear();
}

/*------------------------------------------------------------------------------
	�T�E���h�ǂݍ��� -1�Ń��s�[�g
------------------------------------------------------------------------------*/
int AudioSource::LoadSound( std::string FileName, int nCntLoop)
{
	m_vecData.push_back( Manager::GetSoundManager()->SetSound(FileName, nCntLoop));
	return m_vecData.size() - 1;
}

/*------------------------------------------------------------------------------
	�Đ�
------------------------------------------------------------------------------*/
void AudioSource::Play(int nSoundIdx)
{
	if (nSoundIdx >= 0 && nSoundIdx < m_vecData.size())
	{
		m_vecData[ nSoundIdx]->Play();
	}
}

/*------------------------------------------------------------------------------
	��~
------------------------------------------------------------------------------*/
void AudioSource::Stop(int nSoundIdx)
{
	if (nSoundIdx >= 0 && nSoundIdx < m_vecData.size())
	{
		m_vecData[ nSoundIdx]->Stop();
	}
}

