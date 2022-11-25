/*==============================================================================

    Shader.h - シェーダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/07
==============================================================================*/
#ifndef _SHADER_H_
#define _SHADER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*-----------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Renderer;
class Material;
class Camera;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Shader
{
public:
	virtual ~Shader(){}
	virtual void Set( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet) = 0;

	void Begin(int nPass)
	{
		m_pEffect->Begin( NULL, 0);
		m_pEffect->BeginPass( nPass);
	}

	void End(void)
	{
		m_pEffect->EndPass();
		m_pEffect->End();
	}

protected:
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDec;			//頂点宣言
	LPD3DXEFFECT m_pEffect;								//シェーダ情報

	void Uninit( void);
};


#endif