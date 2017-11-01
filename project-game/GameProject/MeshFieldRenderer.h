/*==============================================================================

   MeshFieldRenderer.h - メッシュフィールド描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/24
==============================================================================*/
#ifndef _MESH_FIELD_RENDERER_H_
#define _MESH_FIELD_RENDERER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Renderer.h"
#include "MeshField.h"
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
class MeshFieldRenderer : public Renderer
{
public:
	MeshFieldRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);
	void LoadTexture( std::string FileName);

	void SetField( int X, int Z, float BlockWidth, float BlockHeight, float *pVertexHeight);
	void SetBlockSize( float Width, float Height);
	void SetSize( float Width, float Height);

	void SetColor( float R, float G, float B, float A) { m_Color = D3DXCOLOR( R, G, B, A); SetVtxBuffer();}
	void SetAlpha( float A) { m_Color.a = A; SetVtxBuffer();}

	void SetShader( EShaderType Type);
	
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
	float *m_pVertexHeight;		//頂点ごとの高さ

	Material *m_pMaterial;							//マテリアル
	D3DXCOLOR m_Color;								//色（ 0.0f〜1.0f ）

};

#endif

