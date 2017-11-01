/*==============================================================================

    Collider.cpp - コライダー（基底クラス）
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Collider.h"
#include "CollisionManager.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Collider::Collider()
{
	 m_ColType = eColNone;
	 m_IsTrigger = true;

	 Manager::GetCollisionManager()->AddCollider( this);
}

/*------------------------------------------------------------------------------
	アクティブ設定
------------------------------------------------------------------------------*/
void Collider::SetActive(bool bEnable)
{
	if (m_bEnable == true && bEnable == false)
	{
		//マネージャーから削除
		Manager::GetCollisionManager()->ReleaseCollider( this);
	}
	else if (m_bEnable == false && bEnable == true)
	{
		//マネージャーに登録
		Manager::GetCollisionManager()->AddCollider( this);
	}

	m_bEnable = bEnable;
}
