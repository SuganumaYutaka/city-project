/*==============================================================================

    LandView.h - 町の自動生成ー区画ビュー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/
#ifndef _LAND_VIEW_H_
#define _LAND_VIEW_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class LandAttribute;
class Polygon3DRenderer;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
//点（交差点）
class LandView : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	LandView( GameObject* pGameObject);
	void Uninit( void);

	void SetAttribute( LandAttribute* attribute);

	void UpdateAttribute( void) { m_IsUpdatedAttribute = true;}

private:
	void Update(void);

	LandAttribute* m_Attribute;

	bool m_IsUpdatedAttribute;
};

#endif

