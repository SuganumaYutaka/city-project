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
	Polygon3DRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);
	void LoadTexture( std::string FileName);
	
	void SetColor( float R, float G, float B, float A) { m_Color = D3DXCOLOR( R, G, B, A); SetVtxBuffer();}
	void SetAlpha( float A) { m_Color.a = A; SetVtxBuffer();}

	void UVScroll( const Vector2 &Value) { m_TextureUV.Scroll( Value); SetVtxBuffer();}
	void UVSetScroll( const Vector2 &Pos = Vector2( 0.0f, 0.0f), const Vector2 &Size = Vector2( 1.0f, 1.0f))
	{ 
		m_TextureUV.SetScroll( Pos, Size);
		SetVtxBuffer();
	}
	void UVAddPart( int nValue = 1) { m_TextureUV.AddPart( nValue); SetVtxBuffer();}
	void UVSetPart( int NumPartX, int NumPartY, int nPart = 0) { m_TextureUV.SetPart( NumPartX, NumPartY, nPart); SetVtxBuffer();}
	int UVGetMaxPart( void) { return m_TextureUV.GetMaxPart(); }

	void SetShader( EShaderType Type);
	
private:
	void SetVtxBuffer( void);

	Material *m_pMaterial;							//�}�e���A��
	D3DXCOLOR m_Color;								//�F�i 0.0f�`1.0f �j
	TextureUV m_TextureUV;							//UV���W
	
};

#endif
