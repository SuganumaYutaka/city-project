/*==============================================================================

    ParticleShader.h - パーティクルシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/16
==============================================================================*/
#ifndef _PARTICLE_SHADER_H_
#define _PARTICLE_SHADER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class ParticleShader : public Shader
{
public:
	ParticleShader();
	~ParticleShader();
	void Set( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet);
private:
	D3DXHANDLE m_hTech;
	D3DXHANDLE m_hMtxWVP;
	D3DXHANDLE m_hTime;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hGravity;
};


#endif