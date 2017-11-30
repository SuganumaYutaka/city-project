/*==============================================================================

    VertexLightingShader.h - 頂点ライティングシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/14
==============================================================================*/
#ifndef _VERTEX_LIGHTING_SHADER_H_
#define _VERTEX_LIGHTING_SHADER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	クラス定義
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
	D3DXHANDLE m_hLightAmb;
	D3DXHANDLE m_hLightDif;
	D3DXHANDLE m_hMaterialAmb;
	D3DXHANDLE m_hMaterialDif;
	D3DXHANDLE m_hDirLight;
};


#endif#pragma once
