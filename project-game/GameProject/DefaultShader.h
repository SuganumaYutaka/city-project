/*==============================================================================

    DefaultShader.h - デフォルトシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/07
==============================================================================*/
#ifndef _DEFAULT_SHADER_H_
#define _DEFAULT_SHADER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class DefaultShader : public Shader
{
public:
	DefaultShader();
	~DefaultShader();
	void Set( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet);
private:
	D3DXHANDLE m_hTech;
	D3DXHANDLE m_hMtxWorld;
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProj;
	D3DXHANDLE m_hTexture;
};


#endif