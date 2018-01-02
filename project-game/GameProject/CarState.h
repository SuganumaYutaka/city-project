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
	eCarStateNeutral = 0
};

/*------------------------------------------------------------------------------
	�N���X��`
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

//�j���[�g����
class CarStateNeutral : public CarState
{
public:
	void Update( void) override;

};


#endif

