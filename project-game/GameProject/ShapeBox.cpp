/*==============================================================================

    ShapeBox.cpp - �����̎��������[�{�b�N�X
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/8
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "ShapeBox.h"
#include "Wall.h"
#include "Roof.h"
#include "DebugLog.h"

#include "BuildingRule.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
ShapeBox::ShapeBox(GameObject* buildingObject)
{
	SetBuildingObject( buildingObject);
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
ShapeBox::~ShapeBox()
{
	Uninit();
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void ShapeBox::Init(const Vector3& position, float rotation, const Vector3& size, BuildingRule* rule)
{
	m_Position = position;
	m_Rotation = rotation;
	m_Size = size;

	//�����̐���
	auto roof = new Roof(GetBuildingObject());
	roof->Init( position, rotation, size);
	AddRoof( roof);

	//�ǂ̐���
	//�E
	Vector3 rightFront( size.x * -0.5f, 0.0f, size.z * -0.5f);
	auto rightWall = new Wall( GetBuildingObject());
	rightWall->Init( size.y, size.z, rightFront, Vector3(1.0f, 0.0f, 0.0f), rule);
	AddWall( rightWall);

	//��
	Vector3 leftBack( size.x * 0.5f, 0.0f, size.z * +0.5f);
	auto leftWall = new Wall( GetBuildingObject());
	leftWall->Init( size.y, size.z, leftBack, Vector3(-1.0f, 0.0f, 0.0f), rule);
	AddWall( leftWall);

	//��
	Vector3 rightBack( size.x * +0.5f, 0.0f, size.z * -0.5f);
	auto backWall = new Wall( GetBuildingObject());
	backWall->Init( size.y, size.x, rightBack, Vector3(0.0f, 0.0f, 1.0f), rule);
	AddWall( backWall);

	//��O
	Vector3 leftFront( size.x * -0.5f, 0.0f, size.z * 0.5f);
	auto frontWall = new Wall( GetBuildingObject());
	frontWall->Init( size.y, size.x, leftFront, Vector3(0.0f, 0.0f, -1.0f), rule);
	AddWall( frontWall);
}

/*------------------------------------------------------------------------------
	�ړ�
------------------------------------------------------------------------------*/
void ShapeBox::Move(const Vector3& value)
{

}

/*------------------------------------------------------------------------------
	��]
------------------------------------------------------------------------------*/
void ShapeBox::Rotate( float value)
{

}

/*------------------------------------------------------------------------------
	�g��k��
------------------------------------------------------------------------------*/
void ShapeBox::Scale(const Vector3& value)
{

}


