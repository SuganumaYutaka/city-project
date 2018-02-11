/*==============================================================================

    LandSpawner.cpp - ���̎��������[�y�n�����֐��I�u�W�F�N�g
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "LandSpawner.h"
#include "GameObject.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"
#include "CityAttribute.h"
#include "Land.h"
#include "LandParameter.h"

#include "BuildingController.h"
#include "BuildingRuleFactory.h"
#include "BuildingManager.h"
#include "CityAttributeManager.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define DEFAULT_LAND_SIZE (10.0f)			//�f�t�H���g�̓y�n�T�C�Y
#define DISTANCE_OF_LANDS (0.5f)			//�y�n���m�̊Ԋu

/*------------------------------------------------------------------------------
	�y�n�̐���
	���@�l�p�`�̋��̂ݑΉ�
------------------------------------------------------------------------------*/
std::vector<LandParameter*> LandSpawner::operator()( BlockAttribute* attribute, CityAttributeManager* attributeManager)
{
	std::vector<LandParameter*> lands;

	//��悩��p�ɂ�����4���_�𒊏o
	std::vector<Vertex*> corners;
	auto face = attribute->GetFace();
	auto halfEdge = face->GetHalfEdge();
	for (;;)
	{
		//���ς�0�O�と�p�Ƃ���
		float dot = Vector3::Dot(halfEdge->GetVector().Normalize(), halfEdge->GetNext()->GetVector().Normalize());
		if (dot < 0.85f)
		{
			corners.push_back( halfEdge->GetEnd());
		}

		halfEdge = halfEdge->GetNext();
		if (halfEdge == face->GetHalfEdge())
		{
			break;
		}
	}

	//4���_�ȊO�̏ꍇ�͖��Ή�
	if (corners.size() != 4)
	{
		return lands;
	}

	//4���_����4�ӂ𐶐�
	std::vector<BlockEdge> edges;
	edges.resize( 4);
	for(int nCnt = 0; nCnt < 4; nCnt++)
	{
		//�ӂ̊J�n�ʒu�𓹘H���̔��������ړ�����
		Vector3 position = corners[nCnt]->GetPosition();
		Vector3 vector;
		if (nCnt == 3)
		{
			vector = corners[ 0]->GetPosition() - corners[ 3]->GetPosition();
		}
		else
		{
			vector = corners[ nCnt + 1]->GetPosition() - corners[ nCnt]->GetPosition();
		}
		Vector3 vectorVertical = Vector3::Cross( Vector3(0.0f, 1.0f, 0.0f), vector);
		position += vector.Normalize() * DEFAULT_ROAD_WIDTH * 0.5f;
		position += vectorVertical.Normalize() * DEFAULT_ROAD_WIDTH * 0.5f;

		//�ӂ̊J�n�ʒu�̐ݒ�
		edges[ nCnt].vertices.push_back( position);
	}
	for(int nCnt = 0; nCnt < 4; nCnt++)
	{
		//�ӂ̓��H���̐ݒ�
		if (nCnt != 3)
		{
			SetRoadsFromCorner( corners[nCnt], corners[nCnt + 1], &edges[nCnt], attribute);
		}
		else
		{
			SetRoadsFromCorner( corners[ nCnt], corners[ 0], &edges[nCnt], attribute);
		}
	}
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//�ӂ̃x�N�g���̐ݒ�
		Vector3 nextPosition;
		if (nCnt != 3)
		{
			nextPosition = edges[ nCnt + 1].vertices[0];
		}
		else
		{
			nextPosition = edges[ 0].vertices[0];
		}
		edges[ nCnt].vector = nextPosition - edges[ nCnt].vertices[0];
	
		//�ӂɗאڂ���y�n���̐ݒ�
		int numLand = (int)( edges[ nCnt].vector.Length() / DEFAULT_LAND_SIZE);

		//�ӂ�y�n���Ŋ������x�N�g���i�ƂȂ�̓_�ւ̃x�N�g���j��ݒ�
		auto vector  = edges[ nCnt].vector / (float)numLand;

		//�ӂ𓙕�����_��ݒ�
		for (int i = 0; i < numLand - 1; i++)
		{
			edges[ nCnt].vertices.push_back( edges[ nCnt].vertices.back() + vector);
		}
	}

	//�y�n��ݒ�
	std::vector<BlockLand> preLands;
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//3�_����ʂ��쐬���邽�߂ɁA�O�̕ӂ̍Ō�̓_��ݒ�
		Vector3 pointFourth;
		if (nCnt != 0)
		{
			pointFourth = edges[ nCnt - 1].vertices.back();
		}
		else
		{
			pointFourth = edges[ 3].vertices.back();
		}

		//�y�n�𐶐�
		int numVertex = edges[nCnt].vertices.size();
		for (int i = 0; i < numVertex - 1; i++)
		{
			//4�ڂ̒��_�i�o�^����3�Ԗځj��ݒ�
			Vector3 pointThird = pointFourth + edges[ nCnt].vector / (float)( numVertex);
			
			//�y�n�𐶐�
			BlockLand land;
			land.vertices.resize(4);
			land.vertices[0] = edges[nCnt].vertices[i];
			land.vertices[1] = edges[nCnt].vertices[i + 1];
			land.vertices[2] = pointThird;
			land.vertices[3] = pointFourth;
			land.canCreateBuilding = true;

			//�y�n�ɗאڂ��铹�H����ݒ�
			if( edges[ nCnt].roads.size() > 0)
			{
				Vector3 midVertex = ( land.vertices[0] + land.vertices[1]) * 0.5f;
				float lengthFromCorner = Vector3::Distance( midVertex, edges[ nCnt].vertices[ 0]);
				float lengthEdge = edges[ nCnt].vector.Length();
				float length = lengthFromCorner / lengthEdge;
				for (auto road : edges[nCnt].roads)
				{
					if (length < road.second)
					{
						land.roads.push_back( road.first);
					}
				}
			
				//�p�n�ɂ͎�O�̕ӂ̍Ō�̓��H�����ݒ�
				if (i == 0)
				{
					if (nCnt != 0)
					{
						auto road = edges[ nCnt - 1].roads.back();
						land.roads.push_back( road.first);
					}
					else
					{
						auto road = edges[ 3].roads.back();
						land.roads.push_back( road.first);
					}
				}
			}
			//�y�n��ǉ�
			preLands.push_back( land);

			//���̐����ɂ�pointThird�𗘗p����
			pointFourth = pointThird;
		}
	}

	//�y�n�����߂�
	float roadWidthHalf = DEFAULT_ROAD_WIDTH * 0.5f;
	for (BlockLand& land : preLands)
	{
		if (!land.canCreateBuilding)
		{
			//continue;
		}

		//�y�n�̍������Ԋu�����߂�
		if ( !NarrowLand(land.vertices[1], land.vertices[0], DISTANCE_OF_LANDS * 1.0f))
		{
			land.canCreateBuilding = false;
			continue;
		}
		if ( !NarrowLand(land.vertices[2], land.vertices[3], DISTANCE_OF_LANDS * 1.0f))
		{
			land.canCreateBuilding = false;
			continue;
		}
	}

	//�y�n�̏Փ˔���
	for (auto ite1 = preLands.begin(); ite1 != preLands.end(); ++ite1)
	{
		for (auto ite2 = preLands.begin(); ite2 != preLands.end(); ++ite2)
		{
			if (ite1 != ite2 && (*ite1).canCreateBuilding && (*ite2).canCreateBuilding)
			{
				if (CollisionLand(*ite1, *ite2))
				{
					(*ite1).canCreateBuilding = false;
				}
			}
		}
	}

	//�y�n�𐶐�
	for (BlockLand& preland : preLands)
	{
		if (preland.canCreateBuilding)
		{
			auto land = new LandParameter();
			for( auto vertex : preland.vertices)
			{
				land->m_Vertices.push_back( vertex);
			}

			for( auto road : preland.roads)
			{
				land->m_RoadIDs.push_back( attributeManager->GetRoadID( road));
			}
			lands.push_back( land);

			////�y�n�̐���
			//Land* land = new Land( manager, parent);
			//land->Init( preland.vertices);
			//lands.push_back( land);

			////���ƃ����N
			//attribute->LinkLand( land);
			//
			////��ʃV�X�e����ݒ�
			//land->SetTraffic( preland.roads);
		}
	}

	//�����p�̒��ԃf�[�^�̉��
	for (BlockEdge& edge : edges)
	{
		edge.vertices.clear();
		edge.vertices.shrink_to_fit();
		edge.roads.clear();
	}
	for (BlockLand& land : preLands)
	{
		land.vertices.clear();
		land.vertices.shrink_to_fit();
		land.roads.clear();
	}

	return lands;
}

/*------------------------------------------------------------------------------
	�y�n�����߂�
------------------------------------------------------------------------------*/
bool LandSpawner::NarrowLand(Vector3& start, Vector3& end, float value)
{
	Vector3 vector;
	vector = end - start;
	if (vector.Length() < value)
	{
		return false;
	}
	vector = vector.Normalize();
	start += vector * value;

	return true;
}

/*------------------------------------------------------------------------------
	�y�n���ړ�������
------------------------------------------------------------------------------*/
bool LandSpawner::MoveLand(Vector3& start, Vector3& end, float value)
{
	Vector3 vector;
	vector = end - start;
	if (vector.Length() < value)
	{
		return false;
	}
	vector = vector.Normalize();
	start += vector * value;
	end += vector * value;

	return true;
}

/*------------------------------------------------------------------------------
	���̊p���瓹�H����ݒ聨BlockEdge�ɕۑ�
------------------------------------------------------------------------------*/
bool LandSpawner::SetRoadsFromCorner(Vertex* corner, Vertex* next, BlockEdge* blockedge, BlockAttribute* attribute)
{
	float blockedgeLength = Vector3::Distance( next->GetPosition(), corner->GetPosition());

	//�͂��߂�Halfedge��ݒ�
	HalfEdge* firstHalfedge = corner->SearchHalfEdgeOnFace( attribute->GetFace());
	if (!firstHalfedge)
	{
		return false;
	}
	firstHalfedge = firstHalfedge->GetNext();
	if (!firstHalfedge)
	{
		return false;
	}
	
	//�����ɉ����ĕӂɐ�߂銄�����Z�o
	float length = firstHalfedge->GetVector().Length();
	
	//Edge��ݒ�
	blockedge->roads.push_back( std::make_pair( (RoadAttribute*)( firstHalfedge->GetEdge()->GetAttribute()), length / blockedgeLength));

	//Edge����݂̂̂Ƃ�����I��
	if (firstHalfedge->GetEnd() == next)
	{
		return true;
	}

	//Halfedge��next�ɓ��B����܂Őݒ�
	auto halfEdge = firstHalfedge->GetNext();
	for (;;)
	{
		length += halfEdge->GetVector().Length();
		blockedge->roads.push_back( std::make_pair( (RoadAttribute*)( halfEdge->GetEdge()->GetAttribute()), length / blockedgeLength));

		if (halfEdge->GetEnd() == next)
		{
			return true;
		}
		halfEdge = halfEdge->GetNext();
	}

	return false;
}

/*------------------------------------------------------------------------------
	�y�n�Ɠy�n�̏Փ˔���
------------------------------------------------------------------------------*/
bool LandSpawner::CollisionLand(const BlockLand& source, const BlockLand& dest)
{
	//���钸�_�����ׂĂ̕ӂɑ΂��ĉE���ɂ���i�O�ςŔ���j���Փ˂���
	for (auto& vertex : source.vertices)
	{
		Vector3 Vec01 = dest.vertices[1] - dest.vertices[0];
		Vector3 Vec0P = vertex - dest.vertices[0];
		Vector3 Cross0 = Vector3::Cross(Vec01, Vec0P);
		if (Cross0.y < 0.0f)
		{
			continue;
		}

		Vector3 Vec12 = dest.vertices[2] - dest.vertices[1];
		Vector3 Vec1P = vertex - dest.vertices[1];
		Vector3 Cross1 = Vector3::Cross(Vec12, Vec1P);
		if (Cross1.y < 0.0f)
		{
			continue;
		}

		Vector3 Vec23 = dest.vertices[3] - dest.vertices[2];
		Vector3 Vec2P = vertex - dest.vertices[2];
		Vector3 Cross2 = Vector3::Cross(Vec23, Vec2P);
		if (Cross2.y < 0.0f)
		{
			continue;
		}

		Vector3 Vec30 = dest.vertices[0] - dest.vertices[3];
		Vector3 Vec3P = vertex - dest.vertices[3];
		Vector3 Cross3 = Vector3::Cross(Vec30, Vec3P);
		if (Cross3.y < 0.0f)
		{
			continue;
		}

		//�Փ˂���
		return true;
	}

	for (auto& vertex : dest.vertices)
	{
		Vector3 Vec01 = source.vertices[1] - source.vertices[0];
		Vector3 Vec0P = vertex - source.vertices[0];
		Vector3 Cross0 = Vector3::Cross(Vec01, Vec0P);
		if (Cross0.y < 0.0f)
		{
			continue;
		}

		Vector3 Vec12 = source.vertices[2] - source.vertices[1];
		Vector3 Vec1P = vertex - source.vertices[1];
		Vector3 Cross1 = Vector3::Cross(Vec12, Vec1P);
		if (Cross1.y < 0.0f)
		{
			continue;
		}

		Vector3 Vec23 = source.vertices[3] - source.vertices[2];
		Vector3 Vec2P = vertex - source.vertices[2];
		Vector3 Cross2 = Vector3::Cross(Vec23, Vec2P);
		if (Cross2.y < 0.0f)
		{
			continue;
		}

		Vector3 Vec30 = source.vertices[0] - source.vertices[3];
		Vector3 Vec3P = vertex - source.vertices[3];
		Vector3 Cross3 = Vector3::Cross(Vec30, Vec3P);
		if (Cross3.y < 0.0f)
		{
			continue;
		}

		//�Փ˂���
		return true;
	}

	//�Փ˂Ȃ�
	return false;
}