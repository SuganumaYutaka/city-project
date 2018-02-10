/*==============================================================================
	
	CarManager.h - ��ʃV�X�e���[�ԊǗ�
														Author : Yutaka Suganuma
														Date   : 2017/12/28
==============================================================================*/
#ifndef _CAR_MANAGER_H_
#define _CAR_MANAGER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Car;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class CarManager
{
private:
	std::vector<Car*> m_Cars;
	
public:
	~CarManager();

	Car* GetCar( int id);
	int GetCarID( Car* land);
	int GetCarCount( void){ return (int)m_Cars.size();}
	const std::vector<Car*>& GetCars( void){ return m_Cars;}
	void RegisterCar( Car* land){ m_Cars.push_back( land); }
	bool UnregisterCar( Car* land);
};


#endif

