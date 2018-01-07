/*==============================================================================

    SpriteShader.h - 2D�|���S���V�F�[�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/11
==============================================================================*/
#ifndef _SPRITE_SHADER_H_
#define _SPRITE_SHADER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class SpriteShader : public Shader
{
public:
	SpriteShader();
	~SpriteShader();
	void Set( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet);
private:
	D3DXHANDLE m_hTech;
	D3DXHANDLE m_hTexture;
};


#endif
