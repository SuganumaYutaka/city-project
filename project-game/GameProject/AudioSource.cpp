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
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* AudioSource::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<AudioSource>();
}

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

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void AudioSource::Load(Text& text)
{
	//text��ǂݐi�߂�
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "SoundData")
		{
			text.ForwardPositionToNextWord();
			int size = std::stoi( text.GetWord());
			for (int nCnt = 0; nCnt < size; nCnt++)
			{
				text.ForwardPositionToNextWord();
				std::string fileName = text.GetWord();
				text.ForwardPositionToNextWord();
				int nCntLoop = std::stoi(text.GetWord());
				LoadSound( fileName, nCntLoop);
			}
		}

		//text��ǂݐi�߂�
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	�Z�[�u
------------------------------------------------------------------------------*/
void AudioSource::Save(Text& text)
{
	StartSave(text);

	int size = m_vecData.size();
	text += "SoundData " + std::to_string(size) + '\n';
	for (int nCnt = 0; nCnt < size; nCnt++)
	{
		text += m_vecData[nCnt]->GetFileName() + ' ';
		text += std::to_string(m_vecData[nCnt]->GetCntLoop()) + '\n';
	}
	
	EndSave( text);
}
