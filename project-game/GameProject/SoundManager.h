/*==============================================================================

    SoundManager.h - �T�E���h�Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/17
==============================================================================*/
#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class SoundData;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class SoundManager
{
public:
	SoundManager( HWND hWnd);
	~SoundManager();

	SoundData* SetSound( std::string FileName, int nCntLoop);

private:
	std::unordered_map< std::string, SoundData *> m_mapData;		//�T�E���h�f�[�^�}�b�v
	IXAudio2 *m_pXAudio2;											//XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;						//�}�X�^�[�{�C�X
};

#endif