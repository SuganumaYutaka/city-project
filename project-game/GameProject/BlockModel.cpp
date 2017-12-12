/*==============================================================================

    BlockModel.cpp - ���̎��������[��惂�f��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/12
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BlockModel.h"
#include "GameObject.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"
#include "CityAttribute.h"

#include "BuildingGeometry.h"
#include "BuildingRuleFactory.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define DEFAULT_ROAD_WIDTH (8.0f)			//�f�t�H���g�̓��H��
#define DEFAULT_LAND_SIZE (10.0f)			//�f�t�H���g�̓y�n�T�C�Y
#define DISTANCE_OF_LANDS (0.8f)			//�y�n���m�̊Ԋu

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* BlockModel::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BlockModel>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BlockModel::BlockModel( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();


}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void BlockModel::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	���[�����̐���
	���@�l�p�`�̋��̂ݑΉ�
------------------------------------------------------------------------------*/
bool BlockModel::CreateBuilding( BlockAttribute* attribute)
{
	//���݂̌������폜
	for (auto geometries : m_BuildingGeometries)
	{
		geometries->m_pGameObject->ReleaseReserve();
	}
	m_BuildingGeometries.clear();
	
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
		return false;
	}

	//�������烋�[�������t�@�N�g���[���擾
	auto buildingRuleFactory = attribute->GetBuildingRuleFactory();

	//4���_����4�ӂ𐶐�
	std::vector<BlockEdge> edges;
	edges.resize( 4);
	for(int nCnt = 0; nCnt < 4; nCnt++)
	{
		//�ӂ̊J�n�ʒu�̐ݒ�
		edges[ nCnt].vertices.push_back( corners[nCnt]->GetPosition());
	}
	for(int nCnt = 0; nCnt < 4; nCnt++)
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
			edges[ nCnt].vertices.push_back( edges[ nCnt].vertices[0] + vector * (float)( i + 1));
		}
	}

	//�y�n��ݒ�
	std::vector<BlockLand> lands;
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//3�_����ʂ��쐬���邽�߂ɁA�O�̕ӂ̍Ō�̓_��ݒ�
		Vector3 pointFourth;
		if (nCnt != 0)
		{
			int prevSize = edges[ nCnt - 1].vertices.size();
			pointFourth = edges[ nCnt - 1].vertices[ prevSize - 1];
		}
		else
		{
			int prevSize = edges[ 3].vertices.size();
			pointFourth = edges[ 3].vertices[ prevSize - 1];
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
			lands.push_back( land);

			//���̐����ɂ�pointThird�𗘗p����
			pointFourth = pointThird;
		}
	}

	//TODO: �y�n�̏Փ˔��聨�y�n�̍���
	
	//�y�n�����߂�
	float roadWidthHalf = DEFAULT_ROAD_WIDTH * 0.5f;
	for (BlockLand& land : lands)
	{
		//�y�n�𓹘H�������߂�
		if ( !NarrowLand(land.vertices[0], land.vertices[3], DEFAULT_ROAD_WIDTH * 0.5f))
		{
			land.canCreateBuilding = false;
			continue;
		}
		if ( !NarrowLand(land.vertices[1], land.vertices[2], DEFAULT_ROAD_WIDTH * 0.5f))
		{
			land.canCreateBuilding = false;
			continue;
		}

		//���H�����E�ɂ��炷
		if (!MoveLand(land.vertices[0], land.vertices[1], DEFAULT_ROAD_WIDTH * 0.5f))
		{
			land.canCreateBuilding = false;
			continue;
		}
		if (!MoveLand(land.vertices[3], land.vertices[2], DEFAULT_ROAD_WIDTH * 0.5f))
		{
			land.canCreateBuilding = false;
			continue;
		}

		//�y�n�̉E�����Ԋu�����߂�
		if ( !NarrowLand(land.vertices[1], land.vertices[0], DISTANCE_OF_LANDS))
		{
			land.canCreateBuilding = false;
			continue;
		}
		if ( !NarrowLand(land.vertices[2], land.vertices[3], DISTANCE_OF_LANDS))
		{
			land.canCreateBuilding = false;
			continue;
		}
	}

	//�����𐶐�
	for (BlockLand& land : lands)
	{
		if (land.canCreateBuilding)
		{
			//�����̐���
			auto gameObject = new GameObject( m_pGameObject);
			auto geometry = gameObject->AddComponent<BuildingGeometry>();
			geometry->Init( land.vertices, buildingRuleFactory->CreateBuildingRule( land.vertices, attribute));
			m_BuildingGeometries.push_back( geometry);
		}
	}

	//�ӁE�y�n�f�[�^�̉��
	for (BlockEdge& edge : edges)
	{
		edge.vertices.clear();
		edge.vertices.shrink_to_fit();
	}
	for (BlockLand& land : lands)
	{
		land.vertices.clear();
		land.vertices.shrink_to_fit();
	}

	return true;
}

/*------------------------------------------------------------------------------
	���[�y�n�����߂�
------------------------------------------------------------------------------*/
bool BlockModel::NarrowLand(Vector3& start, Vector3& end, float value)
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
	���[�y�n���ړ�������
------------------------------------------------------------------------------*/
bool BlockModel::MoveLand(Vector3& start, Vector3& end, float value)
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