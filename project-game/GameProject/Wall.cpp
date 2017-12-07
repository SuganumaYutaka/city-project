/*==============================================================================

    Wall.cpp - �����̎��������[��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Wall.h"
#include "Floor.h"
#include "WallRenderer.h"
#include "GameObject.h"
#include "ComponentInclude.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Wall::Wall( GameObject* buildingObject)
{
	m_Height = 1.0f;
	m_Width = 1.0f;
	m_Floors.clear();

	m_Renderer = buildingObject->AddComponent<WallRenderer>();
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Wall::~Wall()
{
	//�t���A�̏���
	for (auto floor : m_Floors)
	{
		delete floor;
	}
	m_Floors.clear();
}


/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void Wall::Init( float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal)
{
	m_Height = height;
	m_Width = width;

	//TODO:�t���A�̐���


	UpdateView( bottomLeftPosition, normal);
}

/*------------------------------------------------------------------------------
	�������i�~�ɉ����ċȂ���j
------------------------------------------------------------------------------*/
void Wall::Init( float height, float width, const Vector3& bottomLeftPosition, const Vector3& center, float radius)
{
	m_Height = height;
	m_Width = width;

	//TODO:�t���A�̐���


	UpdateView( bottomLeftPosition, center, radius);
}

/*------------------------------------------------------------------------------
	�`����̍X�V
------------------------------------------------------------------------------*/
bool Wall::UpdateView( const Vector3& bottomLeftPosition, const Vector3& normal)
{
	//Renderer�̍X�V
	Vector3 position = bottomLeftPosition;
	Vector3 vector = Vector3::Cross( normal, Vector3(0.0f, 1.0f, 0.0f)).Normalize();
	
	auto pVtx = m_Renderer->StartSetVertexBuffer( CulcCountVertex(), CulcCountPolygon());
	
	for (auto floor : m_Floors)
	{
		floor->SetVertexBuffer( pVtx, position, normal, vector);
		position.y += floor->GetHeight();
	}

	m_Renderer->EndSetVertexBuffer();

	return true;
}

/*------------------------------------------------------------------------------
	�`����̍X�V�i�~�ɉ����ċȂ���j
------------------------------------------------------------------------------*/
bool Wall::UpdateView( const Vector3& bottomLeftPosition, const Vector3& center, float radius)
{
	//Renderer�̍X�V
	Vector3 position = bottomLeftPosition;

	auto pVtx = m_Renderer->StartSetVertexBuffer( CulcCountVertex(), CulcCountPolygon());

	for (auto floor : m_Floors)
	{
		floor->SetVertexBufferCurve( pVtx, position, center, radius);
		position.y += floor->GetHeight();
	}

	m_Renderer->EndSetVertexBuffer();

	return true;
}

/*------------------------------------------------------------------------------
	���_�����Z�o
------------------------------------------------------------------------------*/
int Wall::CulcCountVertex(void)
{
	int count = 0;
	for (auto floor : m_Floors)
	{
		count += floor->CulcCountVertex();
	}

	return count - 1;
}

/*------------------------------------------------------------------------------
	���_�����Z�o�i�~�ɉ����ċȂ���ꍇ�j
------------------------------------------------------------------------------*/
int Wall::CulcCountVertexCurve( float radius)
{
	int count = 0;
	for (auto floor : m_Floors)
	{
		count += floor->CulcCountVertexCurve( radius);
	}

	return count - 1;
}

/*------------------------------------------------------------------------------
	�|���S�������Z�o
------------------------------------------------------------------------------*/
int Wall::CulcCountPolygon(void)
{
	int count = 0;
	for (auto floor : m_Floors)
	{
		count += floor->CulcCountPolygon();
	}

	return count - 2;
}

/*------------------------------------------------------------------------------
	�|���S�������Z�o�i�~�ɉ����ċȂ���ꍇ�j
------------------------------------------------------------------------------*/
int Wall::CulcCountPolygonCurve(float radius)
{
	int count = 0;
	for (auto floor : m_Floors)
	{
		count += floor->CulcCountPolygonCurve( radius);
	}

	return count - 2;
}