/*==============================================================================

    BuildingGeometry.h - 建物の自動生成ー建物ジオメトリ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _BUILDING_GEOMETRY_H_
#define _BUILDING_GEOMETRY_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Shape;
class Tile;
class GeometryParameter;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BuildingGeometry : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BuildingGeometry( GameObject* pGameObject);
	void Uninit( void);

	void Init( GeometryParameter* parameter);
	void AddShape( Shape* shape){ m_Shapes.push_back( shape); }
	const std::list< Shape*>& GetShape( void){ return m_Shapes;}
	void ConfirmGeometry(void);

private:
	void Update(void);

	GeometryParameter* m_Parameter;
	std::list< Shape*> m_Shapes;
	
	Tile* SearchStartTile( Shape* shape, int floorCount);
	bool CanLookTile( Tile* tile, Shape* owner);

};

#endif

