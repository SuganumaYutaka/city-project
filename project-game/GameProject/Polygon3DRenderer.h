/*==============================================================================

    Polygon3DRenderer.h - 3D�|���S���`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/17
==============================================================================*/
#ifndef _POLYGON_3D_RENDERER_H_
#define _POLYGON_3D_RENDERER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Renderer.h"
#include "ShaderManager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Material;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Polygon3DRenderer : public Renderer
{
public:
	static Component* Create( GameObject* gameObject);

	Polygon3DRenderer( GameObject *pGameObject);
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

	virtual bool CheckFrustumCulling( Camera* pCamera);

private:
	void SetVtxBuffer( void);

	Material *m_pMaterial;							//�}�e���A��
	D3DXCOLOR m_Color;								//�F�i 0.0f�`1.0f �j
	TextureUV m_TextureUV;							//UV���W
	
	std::vector<D3DXVECTOR3> m_Vertices;
};

#endif

