/*==============================================================================

    PerPixelLightingShader.h - パーピクセルライティングシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/08
==============================================================================*/
#ifndef _PER_PIXEL_LIGHTING_SHADER_H_
#define _PER_PIXEL_LIGHTING_SHADER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class PerPixelLightingShader : public Shader
{
public:
	PerPixelLightingShader();
	~PerPixelLightingShader();
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

};


#endif#pragma once
