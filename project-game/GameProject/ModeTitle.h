/*==============================================================================

    ModeTitle.h - タイトルモード
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/6/22
==============================================================================*/
#ifndef _MODE_TITLE_H_
#define _MODE_TITLE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Mode.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class ModeTitle : public Mode
{
public:
	ModeTitle();
	~ModeTitle();
	
	void Init( void);
	void Update( void);

};

#endif
