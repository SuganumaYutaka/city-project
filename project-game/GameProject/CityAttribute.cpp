/*==============================================================================

    CityAttribute.cpp - ���̎��������[����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "CityAttribute.h"
#include "GameObject.h"
#include "Edge.h"
#include "Vertex.h"
#include "Face.h"
#include "HalfEdge.h"
#include "JunctionView.h"
#include "RoadView.h"
#include "BlockView.h"
#include "Building.h"
#include "BuildingView.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define DEFAULT_ROAD_WIDTH (4.0f)			//�f�t�H���g�̓��H��
#define DEFAULT_LAND_SIZE (10.0f)			//�f�t�H���g�̓y�n�T�C�Y
#define DISTANCE_OF_LANDS (0.8f)			//�y�n���m�̊Ԋu

/*------------------------------------------------------------------------------
	�����_�[�R���X�g���N�^
------------------------------------------------------------------------------*/
JunctionAttribute::JunctionAttribute( GameObject* parent)
{
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<JunctionView>();
}

/*------------------------------------------------------------------------------
	�����_�[�f�X�g���N�^
------------------------------------------------------------------------------*/
JunctionAttribute::~JunctionAttribute()
{
	if( m_View)
	{
		m_View->m_pGameObject->ReleaseReserve();
	}
}

/*------------------------------------------------------------------------------
	�����_�[������
------------------------------------------------------------------------------*/
void JunctionAttribute::Init(void)
{
	m_View->SetAttribute( this);
}

/*------------------------------------------------------------------------------
	�����_�[�X�V
------------------------------------------------------------------------------*/
void JunctionAttribute::Update(void)
{
	m_View->UpdateAttribute();
}

/*------------------------------------------------------------------------------
	�ʒu���擾
------------------------------------------------------------------------------*/
const Vector3& JunctionAttribute::GetPosition(void)
{
	return GetVertex()->GetPosition();
}

/*------------------------------------------------------------------------------
	���H�[�R���X�g���N�^
------------------------------------------------------------------------------*/
RoadAttribute::RoadAttribute( GameObject* parent) : Width( DEFAULT_ROAD_WIDTH)
{
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<RoadView>();
}

/*------------------------------------------------------------------------------
	���H�[�f�X�g���N�^
------------------------------------------------------------------------------*/
RoadAttribute::~RoadAttribute()
{
	if( m_View)
	{
		m_View->m_pGameObject->ReleaseReserve();
	}
}

/*------------------------------------------------------------------------------
	���H�[������
------------------------------------------------------------------------------*/
void RoadAttribute::Init(void)
{
	m_View->SetAttribute( this);
}

/*------------------------------------------------------------------------------
	���H�[�X�V
------------------------------------------------------------------------------*/
void RoadAttribute::Update(void)
{
	m_View->UpdateAttribute();
}

/*------------------------------------------------------------------------------
	���H�[�������擾
------------------------------------------------------------------------------*/
float RoadAttribute::GetLength( void)
{
	Vector3 start = GetEdge()->GetStart()->GetPosition();
	Vector3 end = GetEdge()->GetEnd()->GetPosition();

	return Vector3::Distance( start, end);
}

/*------------------------------------------------------------------------------
	���H�[���S�ʒu���擾
------------------------------------------------------------------------------*/
Vector3 RoadAttribute::GetCenterPosition( void)
{
	return ( GetEdge()->GetStart()->GetPosition() + GetEdge()->GetEnd()->GetPosition()) * 0.5f;
}

/*------------------------------------------------------------------------------
	���H�[�x�N�g�����擾
------------------------------------------------------------------------------*/
Vector3 RoadAttribute::GetVector( void)
{
	return GetEdge()->GetVector();
}

/*------------------------------------------------------------------------------
	���[�R���X�g���N�^
------------------------------------------------------------------------------*/
BlockAttribute::BlockAttribute( GameObject* parent)
{
	m_ViewGameObject = new GameObject( parent);
	m_ViewGameObject->IsCreatedByOtherComponent = true;
	m_View = m_ViewGameObject->AddComponent<BlockView>();
}

/*------------------------------------------------------------------------------
	���[�f�X�g���N�^
------------------------------------------------------------------------------*/
BlockAttribute::~BlockAttribute()
{
	if( m_View)
	{
		m_View->m_pGameObject->ReleaseReserve();
	}

	for (auto building : m_Buildings)
	{
		delete building;
	}
	m_Buildings.clear();
}

/*------------------------------------------------------------------------------
	���[������
------------------------------------------------------------------------------*/
void BlockAttribute::Init(void)
{
	m_View->SetAttribute( this);
	
	//CreateBuilding();
}

/*------------------------------------------------------------------------------
	���[�X�V
------------------------------------------------------------------------------*/
void BlockAttribute::Update(void)
{
	m_View->UpdateAttribute();

	//CreateBuilding();
}

/*------------------------------------------------------------------------------
	���[�����̐���
	���@�l�p�`�̋��̂ݑΉ�
------------------------------------------------------------------------------*/
bool BlockAttribute::CreateBuilding(void)
{
	//���݂̌������폜
	for (auto building : m_Buildings)
	{
		delete building;
	}
	m_Buildings.clear();
	
	//������View���폜
	auto views = m_ViewGameObject->GetComponentListInChildren<BuildingView>();
	for (auto view : views)
	{
		view->m_pGameObject->ReleaseReserve();
	}

	//��悩��p�ɂ�����4���_�𒊏o
	std::vector<Vertex*> corners;
	auto halfEdge = GetFace()->GetHalfEdge();
	for (;;)
	{
		//���ς�0�O�と�p�Ƃ���
		float dot = Vector3::Dot(halfEdge->GetVector().Normalize(), halfEdge->GetNext()->GetVector().Normalize());
		if (dot < 0.85f)
		{
			corners.push_back( halfEdge->GetEnd());
		}

		halfEdge = halfEdge->GetNext();
		if (halfEdge == GetFace()->GetHalfEdge())
		{
			break;
		}
	}

	//4���_�ȊO�̏ꍇ�͖��Ή�
	if (corners.size() != 4)
	{
		return false;
	}

	//4���_����4�ӂ𐶐�
	std::vector<FaceEdge> edges;
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
	std::vector<FaceLand> lands;
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
			FaceLand land;
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
	for (FaceLand& land : lands)
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
	for (FaceLand& land : lands)
	{
		if (land.canCreateBuilding)
		{
			//�����̐���
			auto building = new Building();
			building->Init( this, m_ViewGameObject, GetFace(), land.vertices);
			m_Buildings.push_back( building);
		}
	}

	//�ӁE�y�n�f�[�^�̉��
	for (FaceEdge& edge : edges)
	{
		edge.vertices.clear();
		edge.vertices.shrink_to_fit();
	}
	for (FaceLand& land : lands)
	{
		land.vertices.clear();
		land.vertices.shrink_to_fit();
	}

	return true;
}

/*------------------------------------------------------------------------------
	���[�y�n�����߂�
------------------------------------------------------------------------------*/
bool BlockAttribute::NarrowLand(Vector3& start, Vector3& end, float value)
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
bool BlockAttribute::MoveLand(Vector3& start, Vector3& end, float value)
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
