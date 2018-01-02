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
	eCarStateNeutral = 0
};

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class CarState
{
public:
	CarState() : m_CarController( NULL){}
	virtual ~CarState(){}
	virtual void Init( CarController* carController){ SetCarController( carController);}
	virtual void Update( void){}

protected:
	CarController* GetCarController( void){ return m_CarController;}
	void SetCarController( CarController* carController){ m_CarController = carController;}

private:
	CarController* m_CarController;
};

//ニュートラル
class CarStateNeutral : public CarState
{
public:
	void Update( void) override;

};


#endif

