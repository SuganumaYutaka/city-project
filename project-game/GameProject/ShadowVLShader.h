/*==============================================================================

    ShadowVLShader.h - 影付き頂点ライティングシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/15
==============================================================================*/
#ifndef _SHADOW_VL_SHADER_H_
#define _SHADOW_VL_SHADER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class ShadowVLShader : public Shader
{
public:
	ShadowVLShader();
	~ShadowVLShader();
	void Set( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet);
private:
	D3DXHANDLE m_hTech;
	D3DXHANDLE m_hMtxWVP;
	D3DXHANDLE m_hMtxWIT;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightAmb;
	D3DXHANDLE m_hLightDif;
	D3DXHANDLE m_hMaterialAmb;
	D3DXHANDLE m_hMaterialDif;
	D3DXHANDLE m_hLightDirW;

	D3DXHANDLE m_hShadowBuf;
	D3DXHANDLE m_hMtxLightWVP;
	D3DXHANDLE m_hMtxLightWV;
	D3DXHANDLE m_hFar;

};


#endif#pragma once
