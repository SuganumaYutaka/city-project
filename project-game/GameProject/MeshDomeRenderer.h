/*==============================================================================

   MeshDomeRenderer.h - ���b�V���h�[���`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/26
==============================================================================*/
#ifndef _MESH_DOME_RENDERER_H_
#define _MESH_DOME_RENDERER_H_

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
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//�C���f�b�N�X�o�b�t�@
	void SetIdxBuffer( void);
	int m_nNumBlockX;						//���̕�����
	int m_nNumBlockY;						//�c�̕�����
	float m_fRadius;						//���a
	float m_fHeight;						//����
	float m_fBlockAngle;					//�|���S��1��������̊p�x
	float m_fBlockHeight;					//�|���S��1��������̍���

	Material *m_pMaterial;							//�}�e���A��
	D3DXCOLOR m_Color;								//�F�i 0.0f�`1.0f �j
	
};

#endif

