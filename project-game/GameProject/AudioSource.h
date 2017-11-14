/*==============================================================================

    AudioSourc.h - �T�E���h�Đ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/18
==============================================================================*/
#ifndef _AUDIO_SOURCE_H_
#define _AUDIO_SOURCE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class SoundData;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
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
	std::vector<SoundData*> m_vecData;		//�T�E���h�f�[�^
};

#endif

