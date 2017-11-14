/*==============================================================================

   MeshFieldRenderer.h - ���b�V���t�B�[���h�`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/24
==============================================================================*/
#ifndef _MESH_FIELD_RENDERER_H_
#define _MESH_FIELD_RENDERER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Renderer.h"
#include "MeshField.h"
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
class MeshFieldRenderer : public Renderer
{
public:
	static Component* Create( GameObject* gameObject);

	MeshFieldRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);
	void LoadTexture( std::string FileName);
	std::string GetTextureName();

	void SetField( int X, int Z, float BlockWidth, float BlockHeight, float *pVertexHeight);
	void SetBlockSize( float Width, float Height);
	void SetSize( float Width, float Height);

	void SetColor( float R, float G, float B, float A) { m_Color = D3DXCOLOR( R, G, B, A); SetVtxBuffer();}
	void SetAlpha( float A) { m_Color.a = A; SetVtxBuffer();}

	void SetShader( EShaderType Type);

	virtual void Save( Text& text);
	virtual void Load( Text& text);
	
private:
	void SetVtxBuffer( void);
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//�C���f�b�N�X�o�b�t�@
	void SetIdxBuffer( void);
	int m_nNumBlockX;			//���̕�����
	int m_nNumBlockZ;			//�c�̕�����
	float m_fWidth;				//��
	float m_fHeight;			//����
	float m_fBlockWidth;		//�|���S��1��������̕�
	float m_fBlockHeight;		//�|���S��1��������̍���
	float *m_pVertexHeight;		//���_���Ƃ̍���

	Material *m_pMaterial;							//�}�e���A��
	D3DXCOLOR m_Color;								//�F�i 0.0f�`1.0f �j

};

#endif

