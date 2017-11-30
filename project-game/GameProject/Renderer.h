/*==============================================================================

    Renderer.h - レンダラー（基底クラス）
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/5
==============================================================================*/
#ifndef _RENDERER_H_
#define _RENDERER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"
#include "RenderManager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Camera;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Renderer : public Component
{
public:
	virtual void Uninit( void){}

	virtual void Update( void) = 0;
	virtual void Draw( Camera* pCamera) = 0;
	virtual void SetActive( bool bEnable);
	virtual bool CheckFrustumCulling( Camera* pCamera) { return true;}
	void SetPass( int Pass) { m_nPass = Pass;}

	virtual void Save( Text& text) {Component::Save(text);}
	virtual void Load( Text& text) {Component::Load(text);}

protected:
	Renderer();
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファ
	int m_nPass;								//描画するパス

};

#endif

