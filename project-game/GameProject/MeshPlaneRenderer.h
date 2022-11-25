/*==============================================================================

   MeshPlaneRenderer.h - 平面描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/24
==============================================================================*/
#ifndef _MESH_PLANE_RENDERER_H_
#define _MESH_PLANE_RENDERER_H_

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
class MeshPlaneRenderer : public Renderer
{
public:
	static Component* Create( GameObject* gameObject);

	MeshPlaneRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);
	void LoadTexture( std::string FileName);

	void SetBlockNum( int X, int Z);
	void SetBlockSize( float Width, float Height);
	void SetNum( int X, int Z);
	void SetSize( float Width, float Height);

	void SetColor( float R, float G, float B, float A) { m_Color = D3DXCOLOR( R, G, B, A); SetVtxBuffer();}
	void SetAlpha( float A) { m_Color.a = A; SetVtxBuffer();}

	void SetShader( EShaderType Type);
	
	virtual void Save( Text& text);
	virtual void Load( Text& text);

private:
	void SetVtxBuffer( void);
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//インデックスバッファ
	void SetIdxBuffer( void);
	int m_nNumBlockX;			//横の分割数
	int m_nNumBlockZ;			//縦の分割数
	float m_fWidth;				//幅
	float m_fHeight;			//高さ
	float m_fBlockWidth;		//ポリゴン1枚あたりの幅
	float m_fBlockHeight;		//ポリゴン1枚あたりの高さ

	Material *m_pMaterial;							//マテリアル
	D3DXCOLOR m_Color;								//色（ 0.0f〜1.0f ）

};

#endif

