/*==============================================================================

    AudioSourc.cpp - サウンド再生
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/18
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "AudioSource.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "SoundManager.h"
#include "SoundData.h"

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* AudioSource::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<AudioSource>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
AudioSource::AudioSource( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_vecData.clear();
}

/*------------------------------------------------------------------------------
	終了処理
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
	サウンド読み込み -1でリピート
------------------------------------------------------------------------------*/
int AudioSource::LoadSound( std::string FileName, int nCntLoop)
{
	m_vecData.push_back( Manager::GetSoundManager()->SetSound(FileName, nCntLoop));
	return m_vecData.size() - 1;
}

/*------------------------------------------------------------------------------
	再生
------------------------------------------------------------------------------*/
void AudioSource::Play(int nSoundIdx)
{
	if (nSoundIdx >= 0 && nSoundIdx < (int)m_vecData.size())
	{
		m_vecData[ nSoundIdx]->Play();
	}
}

/*------------------------------------------------------------------------------
	停止
------------------------------------------------------------------------------*/
void AudioSource::Stop(int nSoundIdx)
{
	if (nSoundIdx >= 0 && nSoundIdx < (int)m_vecData.size())
	{
		m_vecData[ nSoundIdx]->Stop();
	}
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void AudioSource::Load(Text& text)
{
	//textを読み進める
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

		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	セーブ
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
