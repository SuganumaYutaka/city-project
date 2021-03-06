/*==============================================================================

    Mode.cpp - 画面遷移（インターフェイス）
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/6
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Mode.h"
#include "Manager.h"
#include "GameObject.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void Mode::Uninit()
{
	m_pRoot->ReleaseReserve();
}

