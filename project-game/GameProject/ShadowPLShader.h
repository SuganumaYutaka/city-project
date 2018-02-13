/*==============================================================================

    ShadowPLShader.h - 影付きパーピクセルライティングシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/13
==============================================================================*/
#ifndef _SHADOW_PL_SHADER_H_
#define _SHADOW_PL_SHADER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class ShadowPLShader : public Shader
{
public:
	ShadowPLShader();
	~ShadowPLShader();
	void Set( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet);
private:
	D3DXHANDLE m_hTech;
	D3DXHANDLE m_hMtxWorld;
	D3DXHANDLE m_hMtxWVP;
	D3DXHANDLE m_hMtxWIT;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightAmb;
	D3DXHANDLE m_hLightDif;
	D3DXHANDLE m_hLightSpe;
	D3DXHANDLE m_hMaterialAmb;
	D3DXHANDLE m_hMaterialDif;
	D3DXHANDLE m_hMaterialSpe;
	D3DXHANDLE m_hLightDirW;
	D3DXHANDLE m_hPosEyeW;
	D3DXHANDLE m_hSpeclarPower;

	D3DXHANDLE m_hShadowBuf;
	D3DXHANDLE m_hMtxLightWVP;
	D3DXHANDLE m_hMtxLightWV;
	D3DXHANDLE m_hFar;

};


#endif#pragma once
