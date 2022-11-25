/*==============================================================================

    SpriteRenderer.h - 2Dポリゴン描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/5
==============================================================================*/
#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Renderer.h"
#include "ShaderManager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Material;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class SpriteRenderer : public Renderer
{
public:
	static Component* Create( GameObject* gameObject);

	SpriteRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);
	void LoadTexture( std::string FileName);

	void SetColor( float R, float G, float B, float A) { m_Color = D3DXCOLOR( R, G, B, A); SetVtxBuffer();}
	void SetAlpha( float A) { m_Color.a = A; SetVtxBuffer();}

	void ScrollUV( const Vector2& value) { m_TextureUV.Scroll( value); }
	void SetUV( const TextureUV& texUV) { m_TextureUV = texUV;}

	void SetShader( EShaderType Type);

	virtual void Save( Text& text);
	virtual void Load( Text& text);
	
private:
	void SetVtxBuffer( void);

	float m_fRotation;								//回転量
	Material *m_pMaterial;							//マテリアル
	D3DXCOLOR m_Color;								//色（ 0.0f〜1.0f ）
	TextureUV m_TextureUV;							//UV座標
};

#endif

