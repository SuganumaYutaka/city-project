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
class BuildingRuleFactory;
class BuildingManager;
class CarManager;

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
	void Init( float cityWidth, float cityHeight, int countDivide, int countCar, bool doConfirmGeometry);
	void CreateCars( int countCar);
	void DeleteAllCars( void);

private:
	void Update();

	HalfEdgeDataStructure::Model* m_Model;
	BuildingRuleFactory* m_BuildingRuleFactory;
	BuildingManager* m_BuildingManager;
	CarManager* m_CarManager;

	float m_Width;
	float m_Height;
	int m_CountCar;

};


#endif

