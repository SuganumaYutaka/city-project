/*==============================================================================
	
	CarState.h - 交通システムー車の状態
														Author : Yutaka Suganuma
														Date   : 2017/12/30
==============================================================================*/
#ifndef _CAR_STATE_H_
#define _CAR_STATE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class CarController;

/*------------------------------------------------------------------------------
	列挙型定義
------------------------------------------------------------------------------*/
enum ECarState
{
	eCarStateNeutral = 0,
	eCarStateMoveOnRoad,
	eCarStateMoveTowardRoad,
	eCarStateNearNextJunction
};

enum ECarJunctionSelect
{
	eCarJunctionGoStraight = 0,
	eCarJunctionTurnRight,
	eCarJunctionTurnLeft
};

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class CarState
{
public:
	CarState() : m_CarController( NULL){}
	virtual ~CarState(){}
	virtual void Init( void){}
	virtual void Update( void){}

protected:
	CarController* m_CarController;
	
};

//ニュートラル
class CarStateNeutral : public CarState
{
public:
	CarStateNeutral( CarController* carController){ m_CarController = carController; }
	void Update( void) override;

};

//道路上を走行
class CarStateMoveOnRoad : public CarState
{
public:
	CarStateMoveOnRoad( CarController* carController){ m_CarController = carController; }
	void Init( void) override;
	void Update( void) override;

};

//道路に合流するために走行
class CarStateMoveTowardRoad : public CarState
{
public:
	CarStateMoveTowardRoad( CarController* carController){ m_CarController = carController; }
	void Init( void) override;
	void Update( void) override;

};

//次の交差点に接近した状態
class CarStateNearNextJunction : public CarState
{
public:
	CarStateNearNextJunction( CarController* carController);
	~CarStateNearNextJunction();
	void Init( void) override;
	void Update( void) override;

private:
	ECarJunctionSelect m_JunctionSelect;
	Random* m_Random;
};


#endif

