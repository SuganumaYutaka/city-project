/*==============================================================================

    ModeResult.h - ���U���g���[�h
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/6
==============================================================================*/
#ifndef _MODE_RESULT_H_
#define _MODE_RESULT_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Mode.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class ModeResult : public Mode
{
public:
	ModeResult();
	~ModeResult();
	
	void Init( void);
	void Update( void);

};

#endif