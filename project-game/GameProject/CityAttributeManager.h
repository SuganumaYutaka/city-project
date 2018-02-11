/*==============================================================================

    CityAttributeManager.h - 町の自動生成ー属性情報管理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/4
==============================================================================*/
#ifndef _CITY_ATTRIBUTE_MANAGER_H_
#define _CITY_ATTRIBUTE_MANAGER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class JunctionAttribute;
class RoadAttribute;
class BlockAttribute;
class GameObject;

namespace HalfEdgeDataStructure
{
	class Model;
}

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class CityAttributeManager
{
private:
	std::vector<JunctionAttribute*> m_Junctions;
	std::vector<RoadAttribute*> m_Roads;
	std::vector<BlockAttribute*> m_Blocks;

	GameObject* m_GameObject;

public:
	CityAttributeManager( GameObject* parent);
	~CityAttributeManager();
	void DeleteGameObject( void);

	JunctionAttribute* CreateJunctionAttribute( HalfEdgeDataStructure::Model* model, int modelID);
	JunctionAttribute* GetJunction( int id);
	int GetJunctionID( JunctionAttribute* junction);
	int GetJunctionCount( void){ return (int)m_Junctions.size();}
	const std::vector<JunctionAttribute*>& GetJunctions( void){ return m_Junctions;}
	void RegisterJunction( JunctionAttribute* junction){ m_Junctions.push_back( junction); }
	bool UnregisterJunction( JunctionAttribute* junction);

	RoadAttribute* CreateRoadAttribute( HalfEdgeDataStructure::Model* model, int modelID);
	RoadAttribute* GetRoad( int id);
	int GetRoadID( RoadAttribute* road);
	int GetRoadCount( void){ return (int)m_Roads.size();}
	const std::vector<RoadAttribute*>& GetRoads( void){ return m_Roads;}
	void RegisterRoad( RoadAttribute* road){ m_Roads.push_back( road); }
	bool UnregisterRoad( RoadAttribute* road);

	BlockAttribute* CreateBlockAttribute( HalfEdgeDataStructure::Model* model, int modelID);
	BlockAttribute* GetBlock( int id);
	int GetBlockID( BlockAttribute* block);
	int GetBlockCount( void){ return (int)m_Blocks.size();}
	const std::vector<BlockAttribute*>& GetBlocks( void){ return m_Blocks;}
	void RegisterBlock( BlockAttribute* block){ m_Blocks.push_back( block); }
	bool UnregisterBlock( BlockAttribute* block);

};


#endif

