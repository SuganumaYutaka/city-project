/*==============================================================================

    TextureManager.h - �e�N�X�`���Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/3/31
==============================================================================*/
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Texture;
class RenderTexture;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define DEFAULT_TEXTURE_FILE_NAME "data/TEXTURE/default.jpg"		//�f�t�H���g�̃e�N�X�`���̃t�@�C����

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	Texture *Load( std::string FileName);
	RenderTexture *CreateRenderTexture( std::string TextureName, bool isBuckBuffer);

private:
	std::unordered_map< std::string, Texture*> m_mapTexture;		//�e�N�X�`���}�b�v
	Texture* m_DefaultTexture;
};



#endif