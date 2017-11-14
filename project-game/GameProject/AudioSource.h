/*==============================================================================

    AudioSourc.h - サウンド再生
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/18
==============================================================================*/
#ifndef _AUDIO_SOURCE_H_
#define _AUDIO_SOURCE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class SoundData;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class AudioSource : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	AudioSource( GameObject *pGameObject);
	void Uninit( void);

	void Play( int nSoundIdx);
	void Stop( int nSoundIdx);

	int LoadSound( std::string FileName, int nCntLoop);

	virtual void Save( Text& text);
	virtual void Load( Text& text);

private:
	std::vector<SoundData*> m_vecData;		//サウンドデータ
};

#endif

