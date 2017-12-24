/*==============================================================================

    BuildingController.h - 建物の自動生成ー建物コントローラ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/24
==============================================================================*/
#ifndef _BUILDING_CONTROLLER_H_
#define _BUILDING_CONTROLLER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class BuildingGeometry;
class BuildingRule;
class RoadAttribute;
class BuildingManager;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BuildingController : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BuildingController( GameObject* pGameObject);
	void Uninit( void);

	bool Init( const std::vector<Vector3>& vertices, BuildingRule* rule, std::list<RoadAttribute*> roads, BuildingManager* manager);

private:
	void Update(void);

	BuildingManager* m_BuildingManager;
	BuildingGeometry* m_Geometry;
	
	
};

#endif

