/*==============================================================================

    ParticleRenderer.h - �p�[�e�B�N���`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/16
==============================================================================*/
#ifndef _PARTICLE_RENDERER_H_
#define _PARTICLE_RENDERER_H_

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
	�\���̒�`
------------------------------------------------------------------------------*/
//���_�f�[�^
typedef struct
{
	D3DXVECTOR3 Pos;		//�ʒu
	D3DCOLOR Color;			//���_�J���[
	float Size;				//�T�C�Y
	D3DXVECTOR3 Dir;		//�����x�ƕ���
	float Life;				//�p�����ԁi�I���ニ�[�v�j
	float Start;			//�J�n���ԁiLife����ǂꂭ�炢��O���j
}VERTEX_PARTICLE;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class ParticleRenderer : public Renderer
{
public:
	static Component* Create( GameObject* gameObject);

	ParticleRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);
	void LoadTexture( std::string FileName);
	
	void SetShader( EShaderType Type);

	void SetColor( float R, float G, float B, float A) { m_Color = D3DXCOLOR( R, G, B, A); m_bNeedUpdateVtx = true;}
	void SetAlpha( float Center, float Range) { m_Color.a = Center; m_AlphaRange = Range; m_bNeedUpdateVtx = true;}
	void SetPosRange( Vector3 Range) { m_PosRange = Range; m_bNeedUpdateVtx = true;}
	void SetSize( float Center, float Range) { m_SizeCenter = Center; m_SizeRange = Range; m_bNeedUpdateVtx = true;}
	void SetDirection( Vector3 Center, Vector3 Range) { m_DirCenter = Center; m_DirRange = Range; m_bNeedUpdateVtx = true;}
	void SetSpeed( float Center, float Range) { m_SpeedCenter = Center; m_SpeedRange = Range; m_bNeedUpdateVtx = true;}
	void SetLife( float Center, float Range) { m_LifeCenter = Center; m_LifeRenge = Range; m_bNeedUpdateVtx = true;}
	void SetStartRange( float Range) { m_StartRange = Range; m_bNeedUpdateVtx = true;}

	void SetNumParticle( int NumParticle);

	virtual void Save( Text& text);
	virtual void Load( Text& text);
	
private:
	void SetVtxBuffer( void);

	Material*	m_pMaterial;			//�}�e���A��
	D3DXCOLOR	m_Color;			//�F�i0.0f�`1.0f�j
	float		m_AlphaRange;		//�A���t�@�l�i�͈́j
	Vector3		m_PosRange;			//�ʒu�i�͈́j
	float		m_SizeCenter;		//�T�C�Y�i�����l�j
	float		m_SizeRange;		//�T�C�Y�i�͈́j
	Vector3		m_DirCenter;		//�����i�����l�j
	Vector3		m_DirRange;			//�����i�͈́j
	float		m_SpeedCenter;		//�����x�i�����l�j
	float		m_SpeedRange;		//�����x�i�͈́j
	float		m_LifeCenter;		//�p�����ԁi�����l�j
	float		m_LifeRenge;		//�p�����ԁi�͈́j
	float		m_StartRange;		//�J�n���ԁi�͈́j

	int m_nNumParticle;				//�p�[�e�B�N���̐�
	bool m_bNeedUpdateVtx;			//���_�o�b�t�@�̍X�V���K�v��
};

#endif

