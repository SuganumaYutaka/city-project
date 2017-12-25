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

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define DEFAULT_ROAD_WIDTH (12.0f)			//�f�t�H���g�̓��H��

/*------------------------------------------------------------------------------
	�����_�[�R���X�g���N�^
------------------------------------------------------------------------------*/
JunctionAttribute::JunctionAttribute( GameObject* parent)
{
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<JunctionView>();
	m_TrafficJunction = gameObject->AddComponent<TrafficJunction>();
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
RoadAttribute::RoadAttribute( GameObject* parent) : Width( DEFAULT_ROAD_WIDTH)
{
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<RoadView>();
	m_TrafficRoad = gameObject->AddComponent<TrafficRoad>();
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
BlockAttribute::BlockAttribute( GameObject* parent, BuildingRuleFactory* buildingRuleFactory, BuildingManager* buildingManager)
{
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<BlockView>();
	m_BuildingRuleFactory = buildingRuleFactory;
	m_BuildingManager = buildingManager;
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
