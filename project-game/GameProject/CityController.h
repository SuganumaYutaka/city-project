/*==============================================================================
	
	CityController.h - 町の自動生成ーコントローラ
														Author : Yutaka Suganuma
														Date   : 2017/12/1
==============================================================================*/
#ifndef _CITY_CONTROLLER_H_
#define _CITY_CONTROLLER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
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
	クラス定義
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

