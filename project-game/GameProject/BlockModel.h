/*==============================================================================

    BlockModel.h - 町の自動生成ー区画モデル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/12
==============================================================================*/
#ifndef _BLOCK_MODEL_H_
#define _BLOCK_MODEL_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class Building;
class BlockAttribute;
class BuildingController;
class BuildingRuleFactory;
class RoadAttribute;

namespace HalfEdgeDataStructure
{
	class Vertex;
}

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BlockModel : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BlockModel( GameObject* pGameObject);
	void Uninit( void);

	bool CreateBuilding( BlockAttribute* attribute);

private:
	//建物の生成用構造体定義
	//辺（連続する頂点）
	typedef struct
	{
		std::vector<Vector3> vertices;
		Vector3 vector;
		
		std::list<std::pair<RoadAttribute*, float>> roads;

	}BlockEdge;

	//土地（連続する頂点）
	typedef struct
	{
		std::vector<Vector3> vertices;
		bool canCreateBuilding;

		std::list<RoadAttribute*> roads;
	}BlockLand;

	std::list<BuildingController*> m_BuildingControllers;

	bool NarrowLand( Vector3& start, Vector3& end, float value);
	bool MoveLand( Vector3& start, Vector3& end, float value);

	bool SetRoadsFromCorner( HalfEdgeDataStructure::Vertex* corner, HalfEdgeDataStructure::Vertex* next, BlockEdge* blockedge, BlockAttribute* attribute);

	bool CollisionLand( const BlockLand& source, const BlockLand& dest);
};

#endif

