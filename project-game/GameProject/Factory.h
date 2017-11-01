/*==============================================================================

    Factory.h - �I�u�W�F�N�g����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/5
==============================================================================*/
#ifndef _FACTORY_H_
#define _FACTORY_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;

/*------------------------------------------------------------------------------
	�N���X��`
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

