/*==============================================================================
	
	Sun.h - 太陽
														Author : Yutaka Suganuma
														Date   : 2017/8/16
==============================================================================*/
#ifndef _SUN_H_
#define _SUN_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Camera;
class Light;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Sun : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	Sun( GameObject* pGameObject);
	void Uninit( void);

	void SetFieldSize( const Vector3& size);

	virtual void Save( Text& text);
	virtual void Load( Text& text);
	
private:
	void Update();
	Camera* m_pCamera;
	Light* m_pLight;
};


#endif

