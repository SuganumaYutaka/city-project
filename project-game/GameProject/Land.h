/*==============================================================================

    Land.h - 町の自動生成ー土地
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _LAND_H_
#define _LAND_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class MeshPolygonRenderer;
class TrafficLand;
class RoadAttribute;
class CityAttributeManager;
class LandManager;
class Building;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Land
{
public:
	Land( LandManager* manager, GameObject* parent);
	~Land();
	void Delete( void);
	void Init( const std::vector<Vector3>& vertices);
	void OnDeleteBlock( void);

	const std::vector<Vector3>& GetVertices( void) { return m_Vertices;}
	Vector3 CulcCenterPosition( void);

	void LinkAttribute( CityAttributeManager* attributeManager, int blockID);
	void SetTraffic( std::list<RoadAttribute*> roadAttributes);

	int LinkBuilding( Building* building);
	void UnlinkBuilding( Building* building);
	void UnlinkBuilding( int buildingID);
	GameObject* GetGameObject( void){ return m_GameObject;}
	TrafficLand* GetTrafficLand( void){ return m_TrafficLand;}

private:
	LandManager* m_Manager;

	std::vector<Vector3> m_Vertices;
	GameObject* m_GameObject;
	MeshPolygonRenderer* m_Renderer;
	TrafficLand* m_TrafficLand;
	std::vector<Building*> m_Buildings;
	
	CityAttributeManager* m_AttributeManager;
	int m_BlockID;
};

#endif

