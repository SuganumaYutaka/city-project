/*==============================================================================
	
	CityController.h - ���̎��������[�R���g���[��
														Author : Yutaka Suganuma
														Date   : 2017/12/1
==============================================================================*/
#ifndef _CITY_CONTROLLER_H_
#define _CITY_CONTROLLER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;

namespace HalfEdgeDataStructure
{
	class Model;
}

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class CityController : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	CityController( GameObject* pGameObject);
	void Uninit( void);
	
private:
	void Update();

	HalfEdgeDataStructure::Model* m_Model;

};


#endif

