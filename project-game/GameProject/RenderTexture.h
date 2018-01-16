/*==============================================================================

    RenderTexture.h - レンダーテクスチャ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/8
==============================================================================*/
#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Texture.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class RenderTexture : public Texture
{
public:
	RenderTexture( bool isBuckBuffer);
	~RenderTexture();

	bool ChangeDepthRenderFormat( void);

	LPDIRECT3DSURFACE9 GetRenderTarget( void) { return m_pTexSurface;}
	LPDIRECT3DSURFACE9 GetDepthBuffer( void) { return m_pTexZ;}
	const Vector2& GetTextureSize( void){ return m_Size;}
	
private:
	LPDIRECT3DSURFACE9 m_pTexSurface;				//テクスチャサーフェス
	LPDIRECT3DSURFACE9 m_pTexZ;						//深度バッファ
	bool m_bBuckBuffer;								//バックバッファかどうか

	Vector2 m_Size;
};


#endif