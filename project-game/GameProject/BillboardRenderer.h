/*==============================================================================

    BillboardRenderer.h - �r���{�[�h�`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/25
==============================================================================*/
#ifndef _BILLBOARD_RENDERER_H_
#define _BILLBOARD_RENDERER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Renderer.h"

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
class BillboardRenderer : public Renderer
{
public:
	BillboardRenderer( GameObject *pGameObject);
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
	
	
private:
	void SetVtxBuffer( void);
	void SetBillboard( D3DXMATRIX mtxView);

	Material *m_pMaterial;							//�}�e���A��
	D3DXCOLOR m_Color;								//�F�i 0.0f�`1.0f �j
	TextureUV m_TextureUV;							//UV���W
};

#endif

