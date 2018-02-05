/*==============================================================================

    LandCreateRule.h - 町の自動生成ー土地生成ルールの関数オブジェクト
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/4
==============================================================================*/
#ifndef _LAND_CREATE_RULE_H_
#define _LAND_CREATE_RULE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class BlockAttribute;
class RoadAttribute;
class Land;

namespace HalfEdgeDataStructure
{
	class Vertex;
}

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class LandCreateRule
{
public:
	std::vector<Land*> operator()( BlockAttribute* attribute, GameObject* parent);

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

	bool NarrowLand( Vector3& start, Vector3& end, float value);
	bool MoveLand( Vector3& start, Vector3& end, float value);

	bool SetRoadsFromCorner( HalfEdgeDataStructure::Vertex* corner, HalfEdgeDataStructure::Vertex* next, BlockEdge* blockedge, BlockAttribute* attribute);

	bool CollisionLand( const BlockLand& source, const BlockLand& dest);
};

#endif

