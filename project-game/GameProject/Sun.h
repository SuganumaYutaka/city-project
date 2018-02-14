/*==============================================================================
	
	Sun.h - ���z
														Author : Yutaka Suganuma
														Date   : 2017/8/16
==============================================================================*/
#ifndef _SUN_H_
#define _SUN_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Camera;
class Light;

/*------------------------------------------------------------------------------
	�N���X��`
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

