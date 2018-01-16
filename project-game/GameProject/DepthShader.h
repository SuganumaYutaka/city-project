/*==============================================================================

    DepthShader.h - デプスシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/15
==============================================================================*/
#ifndef _DEPTH_SHADER_H_
#define _DEPTH_SHADER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class DepthShader : public Shader
{
public:
	DepthShader();
	~DepthShader();
	void Set( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet);
private:
	D3DXHANDLE m_hTech;
	D3DXHANDLE m_hMtxWorld;
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProj;
	D3DXHANDLE m_hFar;
};


#endif