/*==============================================================================

   MeshDomeRenderer.h - メッシュドーム描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/26
==============================================================================*/
#ifndef _MESH_DOME_RENDERER_H_
#define _MESH_DOME_RENDERER_H_

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
class MeshDomeRenderer : public Renderer
{
public:
	static Component* Create( GameObject* gameObject);

	MeshDomeRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);
	void LoadTexture( std::string FileName);

	void SetColor( float R, float G, float B, float A) { m_Color = D3DXCOLOR( R, G, B, A); SetVtxBuffer();}
	void SetAlpha( float A) { m_Color.a = A; SetVtxBuffer();}
	void SetShader( EShaderType Type);

	void SetDome( int X, int Y, float Radius, float Height);
	void SetBlockSize( float BlockAngle, float BlockHeight);
	void SetSize( float Radius, float Height);

	virtual void Save( Text& text);
	virtual void Load( Text& text);
	
private:
	void SetVtxBuffer( void);
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//インデックスバッファ
	void SetIdxBuffer( void);
	int m_nNumBlockX;						//横の分割数
	int m_nNumBlockY;						//縦の分割数
	float m_fRadius;						//半径
	float m_fHeight;						//高さ
	float m_fBlockAngle;					//ポリゴン1枚あたりの角度
	float m_fBlockHeight;					//ポリゴン1枚あたりの高さ

	Material *m_pMaterial;							//マテリアル
	D3DXCOLOR m_Color;								//色（ 0.0f～1.0f ）
	
};

#endif

