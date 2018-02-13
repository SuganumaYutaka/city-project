/*==============================================================================

    MeshPolygonRenderer.h - 多角形ポリゴン描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/3
==============================================================================*/
#ifndef _MESH_POLYGON_RENDERER_H_
#define _MESH_POLYGON_RENDERER_H_

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
class MeshPolygonRenderer : public Renderer
{
public:
	static Component* Create( GameObject* gameObject);

	MeshPolygonRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);
	void LoadTexture( std::string FileName);
	
	void SetColor( float R, float G, float B, float A) { m_Color = D3DXCOLOR( R, G, B, A); SetVtxBuffer();}
	void SetAlpha( float A) { m_Color.a = A; SetVtxBuffer();}
	Material* GetMaterial( void){ return m_pMaterial;}
	void SetVertices( const std::vector<Vector3>& vertices);

	void SetShader( EShaderType Type);
	
	virtual void Save( Text& text);
	virtual void Load( Text& text);

	virtual bool CheckFrustumCulling( Camera* pCamera);

private:
	Material *m_pMaterial;							//マテリアル
	D3DXCOLOR m_Color;								//色（ 0.0f～1.0f ）
	
	std::vector<D3DXVECTOR3> m_Vertices;

	void SetVtxBuffer( void);
};

#endif

