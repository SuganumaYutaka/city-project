/*==============================================================================
	
	CarState.h - ��ʃV�X�e���[�Ԃ̏��
														Author : Yutaka Suganuma
														Date   : 2017/12/30
==============================================================================*/
#ifndef _CAR_STATE_H_
#define _CAR_STATE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class CarController;

/*------------------------------------------------------------------------------
	�񋓌^��`
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
	�N���X��`
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

//�j���[�g����
class CarStateNeutral : public CarState
{
public:
	CarStateNeutral( CarController* carController){ m_CarController = carController; }
	void Update( void) override;

};

//���H��𑖍s
class CarStateMoveOnRoad : public CarState
{
public:
	CarStateMoveOnRoad( CarController* carController){ m_CarController = carController; }
	void Init( void) override;
	void Update( void) override;

};

//���H�ɍ������邽�߂ɑ��s
class CarStateMoveTowardRoad : public CarState
{
public:
	CarStateMoveTowardRoad( CarController* carController){ m_CarController = carController; }
	void Init( void) override;
	void Update( void) override;

};

//���̌����_�ɐڋ߂������
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

