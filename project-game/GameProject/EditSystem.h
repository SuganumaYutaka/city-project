/*==============================================================================
	
	EditSystem.h - エディットシステム
														Author : Yutaka Suganuma
														Date   : 2017/11/17
==============================================================================*/
#ifndef _EDIT_SYSTEM_H_
#define _EDIT_SYSTEM_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;

/*------------------------------------------------------------------------------
	クラス定義
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

