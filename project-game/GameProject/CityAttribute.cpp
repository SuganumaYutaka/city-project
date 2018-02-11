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
#include "TrafficJunction.h"
#include "TrafficRoad.h"
#include "Land.h"
#include "CityAttributeManager.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�����_�[�R���X�g���N�^
------------------------------------------------------------------------------*/
JunctionAttribute::JunctionAttribute( Model* model, int id, CityAttributeManager* manager, GameObject* parent) : VertexAttribute( model, id), m_Manager( manager)
{
	m_GameObject = new GameObject( parent);
	m_GameObject->IsCreatedByOtherComponent = true;
	m_View = m_GameObject->AddComponent<JunctionView>();
	m_TrafficJunction = m_GameObject->AddComponent<TrafficJunction>();

	m_Manager->RegisterJunction( this);
}

/*------------------------------------------------------------------------------
	�����_�[�f�X�g���N�^
------------------------------------------------------------------------------*/
JunctionAttribute::~JunctionAttribute()
{
	
}

/*------------------------------------------------------------------------------
	�����_�[�폜
------------------------------------------------------------------------------*/
void JunctionAttribute::Delete(void)
{
	m_GameObject->ReleaseReserve();

	//�����N������
	auto vertex = GetVertex();
	if (vertex)
	{
		vertex->LinkAttribute( NULL);
	}

	m_Manager->UnregisterJunction( this);

	delete this;
}

/*------------------------------------------------------------------------------
	�����_�[������
------------------------------------------------------------------------------*/
void JunctionAttribute::Init(void)
{
	m_View->SetAttribute( this);
	m_TrafficJunction->SetAttribute( this);
}

/*------------------------------------------------------------------------------
	�����_�[�X�V
------------------------------------------------------------------------------*/
void JunctionAttribute::Update(void)
{
	m_View->UpdateAttribute();
	m_TrafficJunction->UpdateAttribute();
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
RoadAttribute::RoadAttribute( Model* model, int id, CityAttributeManager* manager, GameObject* parent) : EdgeAttribute( model, id), m_Manager( manager), Width( DEFAULT_ROAD_WIDTH)
{
	m_GameObject = new GameObject( parent);
	m_GameObject->IsCreatedByOtherComponent = true;
	m_View = m_GameObject->AddComponent<RoadView>();
	m_TrafficRoad = m_GameObject->AddComponent<TrafficRoad>();

	m_Manager->RegisterRoad( this);
}

/*------------------------------------------------------------------------------
	���H�[�f�X�g���N�^
------------------------------------------------------------------------------*/
RoadAttribute::~RoadAttribute()
{
	
}

/*------------------------------------------------------------------------------
	���H�[�폜
------------------------------------------------------------------------------*/
void RoadAttribute::Delete(void)
{
	m_GameObject->ReleaseReserve();

	//�����N������
	auto edge = GetEdge();
	if (edge)
	{
		edge->LinkAttribute( NULL);
	}

	m_Manager->UnregisterRoad( this);

	delete this;
}

/*------------------------------------------------------------------------------
	���H�[������
------------------------------------------------------------------------------*/
void RoadAttribute::Init(void)
{
	m_View->SetAttribute( this);
	m_TrafficRoad->SetAttribute( this);
}

/*------------------------------------------------------------------------------
	���H�[�X�V
------------------------------------------------------------------------------*/
void RoadAttribute::Update(void)
{
	m_View->UpdateAttribute();
	m_TrafficRoad->UpdateAttribute();
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
	���H�[���_�����擾
------------------------------------------------------------------------------*/
std::vector<Vector3> RoadAttribute::GetVertices(void)
{
	std::vector<Vector3> vertices;
	auto edge = GetEdge();
	Vector3 start = GetEdge()->GetStart()->GetPosition();
	Vector3 end = GetEdge()->GetEnd()->GetPosition();
	Vector3 vector = GetVector();
	Vector3 vertical( -vector.z, vector.y, vector.x);
	vertical = vertical.Normalize();
	
	vertices.push_back( start + vertical * Width * 0.5f);
	vertices.push_back( end + vertical * Width * 0.5f);
	vertices.push_back( end - vertical * Width * 0.5f);
	vertices.push_back( start - vertical * Width * 0.5f);
	
	return vertices;
}

/*------------------------------------------------------------------------------
	���[�R���X�g���N�^
------------------------------------------------------------------------------*/
BlockAttribute::BlockAttribute( Model* model, int id, CityAttributeManager* manager, GameObject* parent) : FaceAttribute( model, id), m_Manager( manager)
{
	m_GameObject = new GameObject( parent);
	m_GameObject->IsCreatedByOtherComponent = true;
	m_View = m_GameObject->AddComponent<BlockView>();
	m_Lands.clear();

	m_Manager->RegisterBlock( this);
}

/*------------------------------------------------------------------------------
	���[�f�X�g���N�^
------------------------------------------------------------------------------*/
BlockAttribute::~BlockAttribute()
{
	
}

/*------------------------------------------------------------------------------
	���[�폜
------------------------------------------------------------------------------*/
void BlockAttribute::Delete(void)
{
	m_GameObject->ReleaseReserve();

	//�y�n�ɏ�����`����
	int count = m_Lands.size();
	for (int i = 0; i < count; i++)
	{
		if (m_Lands[i])
		{
			m_Lands[i]->OnDeleteBlock();
		}
	}
	m_Lands.clear();
	m_Lands.shrink_to_fit();

	//�����N������
	auto face = GetFace();
	if (face)
	{
		face->LinkAttribute( NULL);
	}

	m_Manager->UnregisterBlock( this);

	delete this;
}

/*------------------------------------------------------------------------------
	���[������
------------------------------------------------------------------------------*/
void BlockAttribute::Init(void)
{
	m_View->SetAttribute( this);
}

/*------------------------------------------------------------------------------
	���[�X�V
------------------------------------------------------------------------------*/
void BlockAttribute::Update(void)
{
	m_View->UpdateAttribute();
}

/*------------------------------------------------------------------------------
	���[�y�n�������N
------------------------------------------------------------------------------*/
int BlockAttribute::LinkLand(Land* land)
{
	m_Lands.push_back( land);
	return m_Lands.size() - 1;
}

/*------------------------------------------------------------------------------
	���[�y�n�̃����N�̉���
------------------------------------------------------------------------------*/
void BlockAttribute::UnlinkLand(Land* land)
{
	int size = m_Lands.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Lands[i] == land)
		{
			m_Lands[i] = NULL;
		}
	}
}

/*------------------------------------------------------------------------------
	���[�y�n�̃����N�̉���
------------------------------------------------------------------------------*/
void BlockAttribute::UnlinkLand(int landID)
{
	m_Lands[ landID] = NULL;
}


