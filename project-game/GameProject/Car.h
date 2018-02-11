/*==============================================================================

    Car.h - ���̎��������[��ʃV�X�e���[��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/
#ifndef _CAR_H_
#define _CAR_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;
class CarManager;
class CarController;
class TrafficLand;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Car
{
public:
	Car( CarManager* manager, GameObject* parent);
	~Car();
	void Delete( void);
	void Init( TrafficLand* spawnLand, TrafficLand* targetLand);

private:
	CarManager* m_Manager;

	GameObject* m_GameObject;
	CarController* m_CarController;
};

#endif
