/*==============================================================================

    Factory.h - オブジェクト生成
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/5
==============================================================================*/
#ifndef _FACTORY_H_
#define _FACTORY_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Factory
{
public:
	GameObject* Create(GameObject* pParent)
	{
		return CreateObject( pParent);
	}

	GameObject* Create(GameObject* pParent, const Vector3& Position)
	{
		return CreateObject( pParent, Position);
	}

protected:
	virtual GameObject* CreateObject( GameObject* pParent) = 0;
	virtual GameObject* CreateObject( GameObject* pParent, const Vector3& Position) = 0;
	
};

#endif

