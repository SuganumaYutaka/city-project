/*==============================================================================

    RenderTexture.h - �����_�[�e�N�X�`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/8
==============================================================================*/
#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Texture.h"

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class RenderTexture : public Texture
{
public:
	RenderTexture( bool isBuckBuffer);
	~RenderTexture();

	LPDIRECT3DSURFACE9 GetRenderTarget( void) { return m_pTexSurface;}
	LPDIRECT3DSURFACE9 GetDepthBuffer( void) { return m_pTexZ;}
	
private:
	LPDIRECT3DSURFACE9 m_pTexSurface;				//�e�N�X�`���T�[�t�F�X
	LPDIRECT3DSURFACE9 m_pTexZ;						//�[�x�o�b�t�@
	bool m_bBuckBuffer;									//�o�b�N�o�b�t�@���ǂ���
};


#endif