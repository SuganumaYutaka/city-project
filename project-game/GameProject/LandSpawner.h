/*==============================================================================

    LandSpawner.h - ���̎��������[�y�n�����֐��I�u�W�F�N�g
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/
#ifndef _LAND_SPAWNER_H_
#define _LAND_SPAWNER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;
class BlockAttribute;
class RoadAttribute;
class Land;
class LandManager;
class LandParameter;
class CityAttributeManager;

namespace HalfEdgeDataStructure
{
	class Vertex;
}

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class LandSpawner
{
public:
	std::vector<LandParameter*> operator()( BlockAttribute* attribute, CityAttributeManager* attributeManager);

private:
	//�����̐����p�\���̒�`
	//�Ӂi�A�����钸�_�j
	typedef struct
	{
		std::vector<Vector3> vertices;
		Vector3 vector;
		
		std::list<std::pair<RoadAttribute*, float>> roads;

	}BlockEdge;

	//�y�n�i�A�����钸�_�j
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

