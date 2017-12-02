/*==============================================================================

    RoadView.h - 町の自動生成ー道路ビュー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/
#ifndef _ROAD_VIEW_H_
#define _ROAD_VIEW_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class RoadAttribute;
class Polygon3DRenderer;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
//点（交差点）
class RoadView : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	RoadView( GameObject* pGameObject);
	void Uninit( void);

	void SetAttribute( RoadAttribute* attribute);

	void UpdateAttribute( void) { m_IsUpdatedAttribute = true;}

private:
	void Update(void);

	RoadAttribute* m_Attribute;
	Polygon3DRenderer* m_Renderer;

	bool m_IsUpdatedAttribute;
};

#endif

