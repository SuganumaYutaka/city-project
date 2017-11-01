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
	if (nSoundIdx >= 0 && nSoundIdx < m_vecData.size())
	{
		m_vecData[ nSoundIdx]->Play();
	}
}

/*------------------------------------------------------------------------------
	停止
------------------------------------------------------------------------------*/
void AudioSource::Stop(int nSoundIdx)
{
	if (nSoundIdx >= 0 && nSoundIdx < m_vecData.size())
	{
		m_vecData[ nSoundIdx]->Stop();
	}
}

