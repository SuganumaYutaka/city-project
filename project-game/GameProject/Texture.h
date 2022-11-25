/*==============================================================================

    Texture.h - テクスチャ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/3/31
==============================================================================*/
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Texture
{
public:
	Texture( std::string fileName);
	virtual ~Texture();

	void Set( void);
	LPDIRECT3DTEXTURE9 GetTexture( void) { return m_pTexture;}
	const std::string& GetFileName( void) { return FileName;}

protected:
	Texture(){}
	LPDIRECT3DTEXTURE9 m_pTexture;		//DirectXテクスチャ
	std::string FileName;

};


#endif