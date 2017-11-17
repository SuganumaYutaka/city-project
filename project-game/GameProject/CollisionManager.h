/*==============================================================================

    CollisionManager.h - 衝突判定（コライダー）管理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/
#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "RaycastHit.h"
#include <list>

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Collider;
class BoxCollider;
class MeshFieldCollider;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define RAY_LENGTH_INFINITY (-1.0f)

/*------------------------------------------------------------------------------
	クラス定義
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
	std::list< Collider *> m_listCollider;				//コライダーリスト
	std::list< Collider *> m_listReleaseCollider;		//消去するコライダーリスト
	bool IsCollision( std::list< Collider *>::iterator Source, std::list< Collider *>::iterator Dest);

	bool BoxBox( BoxCollider *pSource, BoxCollider *pDest);
	bool BoxMeshField( BoxCollider *pBox, MeshFieldCollider *pMeshField);
	bool RayBox( const Vector3& position, const Vector3& direction, float maxDistance, BoxCollider* pBox, RaycastHit* pOut = NULL);

};

#endif

