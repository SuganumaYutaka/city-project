/*==============================================================================

    MeshFieldCollider.h - ���b�V���t�B�[���h�R���C�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/31
==============================================================================*/
#ifndef _MESH_FIELD_COLLIDER_H_
#define _MESH_FIELD_COLLIDER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Collider.h"

/*------------------------------------------------------------------------------
	�\���̒�`
------------------------------------------------------------------------------*/
struct POLYGON
{
	int nVtxIndex[ 3];		//���_�̃C���f�b�N�X�ԍ�
	Vector3 Normal;			//�ʖ@��
};

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class MeshFieldCollider : public Collider
{
public:
	static Component* Create( GameObject* gameObject);

	MeshFieldCollider( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void OnCollision( Collider *pCollider);

	void SetField( int X, int Z, float BlockWidth, float BlockHeight, float *pVertexHeight);
	void SetBlockSize( float Width, float Height);
	void SetSize( float Width, float Height);

	float GetHeight( const Vector3& Pos);
	
	virtual void Save( Text& text);
	virtual void Load( Text& text);

private:
	int m_nNumBlockX;			//���̕�����
	int m_nNumBlockZ;			//�c�̕�����
	float m_fWidth;				//��
	float m_fHeight;			//����
	float m_fBlockWidth;		//�|���S��1��������̕�
	float m_fBlockHeight;		//�|���S��1��������̍���

	Vector3 *m_pVtxPos;			//���_
	POLYGON *m_pPolygon;		//�|���S��

	int GetIndexPolygon( const Vector3& Pos);
	Vector3 GetPolygonNormal( int nIndex);
	bool IsPointInPolygon( int nIndex, const Vector3& Pos);
	
};

#endif

