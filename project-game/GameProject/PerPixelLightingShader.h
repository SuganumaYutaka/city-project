/*==============================================================================

    PerPixelLightingShader.h - �p�[�s�N�Z�����C�e�B���O�V�F�[�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/08
==============================================================================*/
#ifndef _PER_PIXEL_LIGHTING_SHADER_H_
#define _PER_PIXEL_LIGHTING_SHADER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class PerPixelLightingShader : public Shader
{
public:
	PerPixelLightingShader();
	~PerPixelLightingShader();
	void Set( Camera* pCamera, Renderer* pRenderer, Material* pMaterial);
private:
	D3DXHANDLE m_hTech;
	D3DXHANDLE m_hMtxWorld;
	D3DXHANDLE m_hMtxWorldInv;
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProj;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hColAmb;
	D3DXHANDLE m_hColDif;
	D3DXHANDLE m_hColSpe;
	D3DXHANDLE m_hPosLight;
	D3DXHANDLE m_hDirLight;
	D3DXHANDLE m_hPosEye;
};


#endif#pragma once
