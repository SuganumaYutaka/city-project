/*==============================================================================

    CollisionManager.h - �Փ˔���i�R���C�_�[�j�Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/
#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Collider;
class BoxCollider;
class MeshFieldCollider;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void AddCollider( Collider *pCollider);
	void ReleaseCollider( Collider *pCollider);
	void Sort( void);
	void Collision( void);

	void ClearCollider( void) { m_listCollider.clear(); m_listReleaseCollider.clear();}

private:
	std::list< Collider *> m_listCollider;				//�R���C�_�[���X�g
	std::list< Collider *> m_listReleaseCollider;		//��������R���C�_�[���X�g
	bool IsCollision( std::list< Collider *>::iterator Source, std::list< Collider *>::iterator Dest);

	bool BoxBox( BoxCollider *pSource, BoxCollider *pDest);
	bool BoxMeshField( BoxCollider *pBox, MeshFieldCollider *pMeshField);
	bool CollisionRayBox( D3DXVECTOR3 posWorld, D3DXVECTOR3 dirWorld, D3DXVECTOR3 size, D3DXMATRIX mtxWorld, float &Dist, D3DXVECTOR3 *colPos);
};

#endif

