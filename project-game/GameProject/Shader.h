/*==============================================================================

    Shader.h - �V�F�[�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/07
==============================================================================*/
#ifndef _SHADER_H_
#define _SHADER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*-----------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Renderer;
class Material;
class Camera;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Shader
{
public:
	virtual ~Shader(){}
	virtual void Set( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet) = 0;

	void Begin(int nPass)
	{
		m_pEffect->Begin( NULL, 0);
		m_pEffect->BeginPass( nPass);
	}

	void End(void)
	{
		m_pEffect->EndPass();
		m_pEffect->End();
	}

protected:
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDec;			//���_�錾
	LPD3DXEFFECT m_pEffect;								//�V�F�[�_���

	void Uninit( void);
};


#endif