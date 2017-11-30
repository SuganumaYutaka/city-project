/*==============================================================================

    Renderer.h - �����_���[�i���N���X�j
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/5
==============================================================================*/
#ifndef _RENDERER_H_
#define _RENDERER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"
#include "RenderManager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Camera;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Renderer : public Component
{
public:
	virtual void Uninit( void){}

	virtual void Update( void) = 0;
	virtual void Draw( Camera* pCamera) = 0;
	virtual void SetActive( bool bEnable);
	virtual bool CheckFrustumCulling( Camera* pCamera) { return true;}
	void SetPass( int Pass) { m_nPass = Pass;}

	virtual void Save( Text& text) {Component::Save(text);}
	virtual void Load( Text& text) {Component::Load(text);}

protected:
	Renderer();
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//���_�o�b�t�@
	int m_nPass;								//�`�悷��p�X

};

#endif

