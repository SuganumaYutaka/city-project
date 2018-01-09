/*==============================================================================

    BlockModel.h - ���̎��������[��惂�f��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/12
==============================================================================*/
#ifndef _BLOCK_MODEL_H_
#define _BLOCK_MODEL_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
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
	�N���X��`
------------------------------------------------------------------------------*/
class BlockModel : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BlockModel( GameObject* pGameObject);
	void Uninit( void);

	bool CreateBuilding( BlockAttribute* attribute);

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

	std::list<BuildingController*> m_BuildingControllers;

	bool NarrowLand( Vector3& start, Vector3& end, float value);
	bool MoveLand( Vector3& start, Vector3& end, float value);

	bool SetRoadsFromCorner( HalfEdgeDataStructure::Vertex* corner, HalfEdgeDataStructure::Vertex* next, BlockEdge* blockedge, BlockAttribute* attribute);

	bool CollisionLand( const BlockLand& source, const BlockLand& dest);
};

#endif

