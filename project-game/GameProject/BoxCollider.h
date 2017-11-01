/*==============================================================================

    BoxCollider.h - ボックスコライダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/
#ifndef _BOX_COLLIDER_H_
#define _BOX_COLLIDER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Collider.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class MeshBoxRenderer;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BoxCollider : public Collider
{
public:
	BoxCollider( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void OnCollision( Collider *pCollider);
	virtual void SetActive( bool bEnable);

	const Vector3& GetCenter() const { return m_Center;}
	const Vector3& GetSize() const { return m_Size;}

	void SetCenter( const Vector3& Center);
	void SetSize( const Vector3& Size);

private:
	Vector3 m_Center;				//中心位置（Transformの位置からの相対的な位置）
	Vector3 m_Size;					//大きさ

#ifdef _DEBUG
	MeshBoxRenderer *m_pRenderer;	//レンダラー
#endif // _DEBUG


};

#endif

