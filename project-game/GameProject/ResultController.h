/*==============================================================================
	
	ResultController.h - リザルト処理
														Author : Yutaka Suganuma
														Date   : 2017/7/6
==============================================================================*/
#ifndef _RESULT_CONTROLLER_H_
#define _RESULT_CONTROLLER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"


/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class ResultController : public Component
{
public:
	ResultController( GameObject* pGameObject);
	void Uninit( void);

	void OnCollision( Collider *pCollider);

private:
	void Update();

	int nCnt;

};


#endif

