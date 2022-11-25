/*==============================================================================

    Car.h - 町の自動生成ー交通システムー車
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/
#ifndef _CAR_H_
#define _CAR_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class CarManager;
class CarController;
class TrafficLand;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Car
{
public:
	Car( CarManager* manager, GameObject* parent);
	~Car();
	void Delete( void);
	void Init( TrafficLand* spawnLand, TrafficLand* targetLand);
	GameObject* GetGameObject( void){ return m_GameObject;}

private:
	CarManager* m_Manager;

	GameObject* m_GameObject;
	CarController* m_CarController;
};

#endif

