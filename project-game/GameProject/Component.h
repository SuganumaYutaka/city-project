/*==============================================================================

    Component.h - コンポーネント（基底クラス）
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/27
==============================================================================*/
#ifndef _COMPONENT_H_
#define _COMPONENT_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class Transform;
class Collider;

/*------------------------------------------------------------------------------
	列挙型定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Component
{
public:
	Component( GameObject *pGameObject);
	~Component();

	GameObject *GetGameObject() { return m_pGameObject;}	//所有者情報を渡す
	Transform *GetTransform() { return m_pTransform;}

	virtual void Update( void) {}
	virtual void Uninit( void) {}
	
	virtual void OnCollision( Collider *pCollider) {}
	//virtual void OnCollisionEnter( Collider *pCollider) {}
	//virtual void OnCollisionExit( Collider *pCollider) {}

	int m_nLayer;					//レイヤー

	GameObject *m_pGameObject;		//所有者（親）
	Transform *m_pTransform;		//所有者の位置・回転・スケール

	void ReleaseReserve( void );
	static void ReleaseList( void);

	virtual void SetActive( bool bEnable) { m_bEnable = bEnable;}
	bool GetActive( void) const { return m_bEnable;}

protected:
	Component();
	bool m_bEnable;					//実行可能か
	
private:
	static std::list<Component*> m_listReleaseComponent;	//消去リスト
	bool m_bRelease;										//消去予約済みか
};

#endif

