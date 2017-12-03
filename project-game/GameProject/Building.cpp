/*==============================================================================

    Building.cpp - ���̎��������[����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/3
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Building.h"
#include "Face.h"
#include "BuildingView.h"
#include "GameObject.h"
#include "ComponentInclude.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define BUILDING_HEIGHT (5.0f)

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Building::Building( GameObject* parent, HalfEdgeDataStructure::Face* face, const std::vector<Vector3> vertices)
	: m_Face( face)
{
	//���_���i�[
	m_Vertices = vertices;

	//TODO: �ʐςɉ����ăr���̍�����ݒ肷��
	m_Height = BUILDING_HEIGHT;

	//View�̐���
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<BuildingView>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Building::Building( GameObject* parent, HalfEdgeDataStructure::Face* face, const Vector3& topLeft, const Vector3& topRight, const Vector3& bottomLeft, const Vector3& bottomRight)
	: m_Face( face)
{
	//���_���i�[
	m_Vertices.resize( 4);
	m_Vertices[0] = topLeft;
	m_Vertices[1] = topRight;
	m_Vertices[2] = bottomLeft;
	m_Vertices[3] = bottomRight;

	//TODO: �ʐςɉ����ăr���̍�����ݒ肷��
	m_Height = BUILDING_HEIGHT;

	//View�̐���
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	m_View = gameObject->AddComponent<BuildingView>();
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Building::~Building()
{
	if( m_View)
	{
		m_View->m_pGameObject->ReleaseReserve();
	}
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void Building::Init(void)
{
	m_View->SetBuilding( this);
}
