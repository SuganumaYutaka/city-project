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
#include "LandView.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define DEFAULT_WIDTH (2.0f)	//�f�t�H���g�̕�

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
		m_View->ReleaseReserve();
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
RoadAttribute::RoadAttribute( GameObject* parent) : Width( DEFAULT_WIDTH)
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
		m_View->ReleaseReserve();
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
LandAttribute::LandAttribute( GameObject* parent)
{
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<LandView>();
}

/*------------------------------------------------------------------------------
	���[�f�X�g���N�^
------------------------------------------------------------------------------*/
LandAttribute::~LandAttribute()
{
	if( m_View)
	{
		m_View->ReleaseReserve();
	}
}

/*------------------------------------------------------------------------------
	���[������
------------------------------------------------------------------------------*/
void LandAttribute::Init(void)
{
	m_View->SetAttribute( this);
}

/*------------------------------------------------------------------------------
	���[�X�V
------------------------------------------------------------------------------*/
void LandAttribute::Update(void)
{
	m_View->UpdateAttribute();
}

