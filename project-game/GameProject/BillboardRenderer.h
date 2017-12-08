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
	static Component* Create( GameObject* gameObject);

	BillboardRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);
	void LoadTexture( std::string FileName);

	void SetColor( float R, float G, float B, float A) { m_Color = D3DXCOLOR( R, G, B, A); SetVtxBuffer();}
	void SetAlpha( float A) { m_Color.a = A; SetVtxBuffer();}

	void ScrollUV( const Vector2& value) { m_TextureUV.Scroll( value); }
	void SetUV( const TextureUV& texUV) { m_TextureUV = texUV;}
	
	virtual void Save( Text& text);
	virtual void Load( Text& text);
	
private:
	void SetVtxBuffer( void);
	void SetBillboard( D3DXMATRIX mtxView);

	Material *m_pMaterial;							//�}�e���A��
	D3DXCOLOR m_Color;								//�F�i 0.0f�`1.0f �j
	TextureUV m_TextureUV;							//UV���W
};

#endif

