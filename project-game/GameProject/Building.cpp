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
void Building::Init( BlockAttribute* attribute, GameObject* parent, const std::vector<Vector3> vertices)
{
	//�W�I���g���̐���


	//View�̐���
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	auto view = gameObject->AddComponent<BuildingView>();
	view->SetBuilding( this, attribute);
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void Building::Init( BlockAttribute* attribute, GameObject* parent, const Vector3& topLeft, const Vector3& topRight, const Vector3& bottomLeft, const Vector3& bottomRight)
{
	//���_���i�[
	std::vector<Vector3> vertices;
	vertices.resize( 4);
	vertices[0] = topLeft;
	vertices[1] = topRight;
	vertices[2] = bottomRight;
	vertices[3] = bottomLeft;

	//�W�I���g���̐���


	//View�̐���
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	auto view = gameObject->AddComponent<BuildingView>();
	view->SetBuilding( this, attribute);
}
