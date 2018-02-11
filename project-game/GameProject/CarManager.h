/*==============================================================================
	
	CarManager.h - 交通システムー車管理
														Author : Yutaka Suganuma
														Date   : 2017/12/28
==============================================================================*/
#ifndef _CAR_MANAGER_H_
#define _CAR_MANAGER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Car;
class GameObject;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class CarManager
{
private:
	std::vector<Car*> m_Cars;
	GameObject* m_GameObject;

public:
	CarManager( GameObject* parent);
	~CarManager();
	void DeleteGameObject( void);

	Car* CreateCar( void);
	Car* GetCar( int id);
	int GetCarID( Car* land);
	int GetCarCount( void){ return (int)m_Cars.size();}
	const std::vector<Car*>& GetCars( void){ return m_Cars;}
	void RegisterCar( Car* land){ m_Cars.push_back( land); }
	bool UnregisterCar( Car* land);
};


#endif

