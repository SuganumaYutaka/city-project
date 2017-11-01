/*==============================================================================

   MeshField.h - ���b�V���t�B�[���h
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/31
==============================================================================*/
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class MeshFieldRenderer;
class MeshFieldCollider;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class MeshField : public Component
{
public:
	MeshField( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( void);

	void SetField( int X, int Z, float BlockWidth, float BlockHeight, float *pVertexHeight);
	void SetBlockSize( float Width, float Height);
	void SetSize( float Width, float Height);

private:
	MeshFieldRenderer *m_pMeshFieldRenderer;		//���b�V���t�B�[���h�`��N���X
	MeshFieldCollider *m_pMeshFieldCollider;		//���b�V���t�B�[���h�R���C�_�[

	int m_nNumBlockX;			//���̕�����
	int m_nNumBlockZ;			//�c�̕�����
	float m_fWidth;				//��
	float m_fHeight;			//����
	float m_fBlockWidth;		//�|���S��1��������̕�
	float m_fBlockHeight;		//�|���S��1��������̍���

	float *m_pVertexHeight;		//���_���Ƃ̍���
};

#endif

