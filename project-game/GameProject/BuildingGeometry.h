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
class Land;
class Shape;
class BuildingRule;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BuildingGeometry : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BuildingGeometry( GameObject* pGameObject);
	void Uninit( void);

	bool Init( const std::vector<Vector3>& vertices, BuildingRule* rule);

	Land* GetLand( void) { return m_Land;}
	void AddShape( Shape* shape){ m_Shapes.push_back( shape); }

private:
	void Update(void);

	Land* m_Land;
	std::list< Shape*> m_Shapes;
	BuildingRule* m_Rule;
};

#endif

