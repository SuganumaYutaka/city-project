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

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Collider;
class BoxCollider;
class MeshFieldCollider;

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

private:
	std::list< Collider *> m_listCollider;				//コライダーリスト
	std::list< Collider *> m_listReleaseCollider;		//消去するコライダーリスト
	bool IsCollision( std::list< Collider *>::iterator Source, std::list< Collider *>::iterator Dest);

	bool BoxBox( BoxCollider *pSource, BoxCollider *pDest);
	bool BoxMeshField( BoxCollider *pBox, MeshFieldCollider *pMeshField);
	bool CollisionRayBox( D3DXVECTOR3 posWorld, D3DXVECTOR3 dirWorld, D3DXVECTOR3 size, D3DXMATRIX mtxWorld, float &Dist, D3DXVECTOR3 *colPos);
};

#endif

