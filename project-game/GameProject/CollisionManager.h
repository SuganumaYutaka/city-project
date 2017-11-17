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
#include "RaycastHit.h"
#include <list>

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Collider;
class BoxCollider;
class MeshFieldCollider;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define RAY_LENGTH_INFINITY (-1.0f)

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

	bool Raycast(const Ray& ray, float maxDistance  = RAY_LENGTH_INFINITY, RaycastHit* pOut = NULL);
	bool Raycast(const Vector3& position, const Vector3& direction, float maxDistance  = RAY_LENGTH_INFINITY, RaycastHit* pOut = NULL);
	std::list<RaycastHit> RaycastAll( const Vector3& position, const Vector3& direction, float maxDistance = RAY_LENGTH_INFINITY);

private:
	std::list< Collider *> m_listCollider;				//�R���C�_�[���X�g
	std::list< Collider *> m_listReleaseCollider;		//��������R���C�_�[���X�g
	bool IsCollision( std::list< Collider *>::iterator Source, std::list< Collider *>::iterator Dest);

	bool BoxBox( BoxCollider *pSource, BoxCollider *pDest);
	bool BoxMeshField( BoxCollider *pBox, MeshFieldCollider *pMeshField);
	bool RayBox( const Vector3& position, const Vector3& direction, float maxDistance, BoxCollider* pBox, RaycastHit* pOut = NULL);

};

#endif

