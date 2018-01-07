/*==============================================================================

    BillboardShader.h - ビルボードシェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/09
==============================================================================*/
#ifndef _BILLBOARD_SHADER_H_
#define _BILLBOARD_SHADER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BillboardShader : public Shader
{
public:
	BillboardShader();
	~BillboardShader();
	void Set( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet);
private:
	D3DXHANDLE m_hTech;
	D3DXHANDLE m_hMtxPos;
	D3DXHANDLE m_hMtxScale;
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxViewInverse;
	D3DXHANDLE m_hMtxProj;
	D3DXHANDLE m_hTexture;
};


#endif
