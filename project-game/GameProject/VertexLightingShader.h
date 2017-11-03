/*==============================================================================

    VertexLightingShader.h - ���_���C�e�B���O�V�F�[�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/14
==============================================================================*/
#ifndef _VERTEX_LIGHTING_SHADER_H_
#define _VERTEX_LIGHTING_SHADER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class VertexLightingShader : public Shader
{
public:
	VertexLightingShader();
	~VertexLightingShader();
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
	D3DXHANDLE m_hDirLight;
};


#endif#pragma once