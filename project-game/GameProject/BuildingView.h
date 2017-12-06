/*==============================================================================

    BuildingView.h - 町の自動生成ー建物ビュー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/3
==============================================================================*/
#ifndef _BUILDING_VIEW_H_
#define _BUILDING_VIEW_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class BlockAttribute;
class MeshPolygonRenderer;
class Building;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BuildingView : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BuildingView( GameObject* pGameObject);
	void Uninit( void);

	void SetBuilding( Building* building, BlockAttribute* attribute);

private:
	void Update(void);
	Building* m_Building;
	BlockAttribute* m_Attribute;
	MeshPolygonRenderer* m_Renderer;
};

#endif

