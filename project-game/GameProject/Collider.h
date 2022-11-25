/*==============================================================================

    Collider.h - コライダー（基底クラス）
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	列挙型定義
------------------------------------------------------------------------------*/
enum E_COLTYPE
{
	eColNone = 0,				//設定なし
	eColBox,					//ボックス
	eColMeshField,				//メッシュフィールド
	eColRay,					//直線
	eColNum						//コライダーの種類の総数
};

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Collider : public Component
{
public:
	virtual void Uninit( void){}

	virtual void Update( void) = 0;
	virtual void Draw( void) {}
	virtual void SetActive( bool bEnable);
	
	const E_COLTYPE GetColType() const { return m_ColType;}

	bool m_IsTrigger;			//トリガー用か
	Vector3 m_OldPos;			//前の位置

	virtual void Save( Text& text) {Component::Save(text);}
	virtual void Load( Text& text) {Component::Load(text);}

protected:
	Collider();
	E_COLTYPE m_ColType;		//コライダーの種類
	
};

#endif

