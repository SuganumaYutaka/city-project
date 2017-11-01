/*==============================================================================

    Texture.h - �e�N�X�`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/3/31
==============================================================================*/
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Texture
{
public:
	Texture( std::string FileName);
	virtual ~Texture();

	void Set( void);
	LPDIRECT3DTEXTURE9 GetTexture( void) { return m_pTexture;}

protected:
	Texture(){}
	LPDIRECT3DTEXTURE9 m_pTexture;		//DirectX�e�N�X�`��

};


#endif