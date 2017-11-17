/*==============================================================================
	
	EditSystem.h - �G�f�B�b�g�V�X�e��
														Author : Yutaka Suganuma
														Date   : 2017/11/17
==============================================================================*/
#ifndef _EDIT_SYSTEM_H_
#define _EDIT_SYSTEM_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class EditSystem : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	EditSystem( GameObject* pGameObject);
	void Uninit( void);
	
private:
	void Update();
	GameObject* SelectedGameObject;

	static EditSystem* Instance;
};


#endif

