/*==============================================================================

   MeshBoxRenderer.h - �{�b�N�X�`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/14
==============================================================================*/
#ifndef _MESH_BOX_RENDERER_H_
#define _MESH_BOX_RENDERER_H_

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
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//�C���f�b�N�X�o�b�t�@
	void SetIdxBuffer( void);

	Vector3 m_Center;			//���S�ʒu�iTransform�̈ʒu����̑��ΓI�Ȉʒu�j
	Vector3 m_Size;				//�傫��

	Material *m_pMaterial;							//�}�e���A��
	D3DXCOLOR m_Color;								//�F�i 0.0f�`1.0f �j
	
};

#endif

