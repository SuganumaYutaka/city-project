/*==============================================================================

   MeshBoxRenderer.h - ボックス描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/14
==============================================================================*/
#ifndef _MESH_BOX_RENDERER_H_
#define _MESH_BOX_RENDERER_H_

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
class MeshBoxRenderer : public Renderer
{
public:
	static Component* Create( GameObject* gameObject);

	MeshBoxRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);
	void LoadTexture( std::string FileName);

	void SetCenter( const Vector3& Center);
	void SetSize( const Vector3& Size);
	void SetWireFrame( void) { m_nLayer = eLayerWireFrame;}
	
	void SetColor( float R, float G, float B, float A) { m_Color = D3DXCOLOR( R, G, B, A); SetVtxBuffer();}
	void SetAlpha( float A) { m_Color.a = A; SetVtxBuffer();}

	void SetShader( EShaderType Type);

	virtual void Save( Text& text);
	virtual void Load( Text& text);
	
private:
	void SetVtxBuffer( void);
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//インデックスバッファ
	void SetIdxBuffer( void);

	Vector3 m_Center;			//中心位置（Transformの位置からの相対的な位置）
	Vector3 m_Size;				//大きさ

	Material *m_pMaterial;							//マテリアル
	D3DXCOLOR m_Color;								//色（ 0.0f～1.0f ）
	
};

#endif

