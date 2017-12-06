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
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Building::~Building()
{
	
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void Building::Init( BlockAttribute* attribute, GameObject* parent, HalfEdgeDataStructure::Face* face, const std::vector<Vector3> vertices)
{
	m_Face = face;

	//���_���i�[
	m_Vertices = vertices;

	//TODO: �ʐςɉ����ăr���̍�����ݒ肷��
	m_Height = BUILDING_HEIGHT;

	//View�̐���
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	auto view = gameObject->AddComponent<BuildingView>();
	view->SetBuilding( this, attribute);
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void Building::Init( BlockAttribute* attribute, GameObject* parent, HalfEdgeDataStructure::Face* face, const Vector3& topLeft, const Vector3& topRight, const Vector3& bottomLeft, const Vector3& bottomRight)
{
	m_Face = face;

	//���_���i�[
	m_Vertices.resize( 4);
	m_Vertices[0] = topLeft;
	m_Vertices[1] = topRight;
	m_Vertices[2] = bottomRight;
	m_Vertices[3] = bottomLeft;

	//TODO: �ʐςɉ����ăr���̍�����ݒ肷��
	m_Height = BUILDING_HEIGHT;

	//View�̐���
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	auto view = gameObject->AddComponent<BuildingView>();
	view->SetBuilding( this, attribute);
}
