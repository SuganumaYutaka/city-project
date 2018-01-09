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
	SetPosition( position);
	SetRotation( rotation);
	m_Size = size;
	m_Rule = rule;

	//�����̐���
	auto roof = new Roof(GetBuildingObject());
	roof->Init( position, rotation, size);
	AddRoof( roof);

	//�ǂ̐���
	CreateWalls();
}

void ShapeBox::CreateWalls(void)
{
	//�ǂ̏���
	if (GetWalls().size() != 0)
	{
		ClearWall();
	}

	auto matrix = GetMatrix();

	//��O
	Vector3 leftFront( m_Size.x * -0.5f, 0.0f, m_Size.z * 0.5f);
	auto frontWall = new Wall( GetBuildingObject());
	frontWall->InitDefault( matrix, m_Size.y, m_Size.x, leftFront, Vector3(0.0f, 0.0f, -1.0f), m_Rule);
	AddWall( frontWall);

	//�E
	Vector3 rightFront( m_Size.x * -0.5f, 0.0f, m_Size.z * -0.5f);
	auto rightWall = new Wall( GetBuildingObject());
	rightWall->InitDefault( matrix, m_Size.y, m_Size.z, rightFront, Vector3(1.0f, 0.0f, 0.0f), m_Rule);
	AddWall( rightWall);

	//��
	Vector3 rightBack( m_Size.x * +0.5f, 0.0f, m_Size.z * -0.5f);
	auto backWall = new Wall( GetBuildingObject());
	backWall->InitDefault( matrix, m_Size.y, m_Size.x, rightBack, Vector3(0.0f, 0.0f, 1.0f), m_Rule);
	AddWall( backWall);

	//��
	Vector3 leftBack( m_Size.x * 0.5f, 0.0f, m_Size.z * +0.5f);
	auto leftWall = new Wall( GetBuildingObject());
	leftWall->InitDefault( matrix, m_Size.y, m_Size.z, leftBack, Vector3(-1.0f, 0.0f, 0.0f), m_Rule);
	AddWall( leftWall);
}

/*------------------------------------------------------------------------------
	�ړ�
------------------------------------------------------------------------------*/
void ShapeBox::Move(const Vector3& value)
{
	UpdatePosition( value);

	//�ǂ̍X�V
	for( auto wall : GetWalls())
	{
		wall->UpdateView( GetMatrix());
	}

	//�����̍X�V
	for (auto roof : GetRoofs())
	{
		roof->UpdatePosition( GetPosition());
	}
}

/*------------------------------------------------------------------------------
	��]
------------------------------------------------------------------------------*/
void ShapeBox::Rotate( float value)
{
	UpdateRotation( value);

	//�ǂ̍X�V
	for( auto wall : GetWalls())
	{
		wall->UpdateView( GetMatrix());
	}

	//�����̍X�V
	for (auto roof : GetRoofs())
	{
		roof->UpdateRotation( GetRotation());
	}
}

/*------------------------------------------------------------------------------
	�g��k���i�ω��ʁj
------------------------------------------------------------------------------*/
void ShapeBox::ScaleUpDown(const Vector3& value)
{
	m_Size += value;

	//�ǂ̍X�V
	CreateWalls();

	//�����̍X�V
	for (auto roof : GetRoofs())
	{
		roof->UpdateSize( m_Size);
	}
}

/*------------------------------------------------------------------------------
	�g��k���i�䗦�E���{�j
------------------------------------------------------------------------------*/
void ShapeBox::ScaleRate( float rate)
{
	m_Size *= rate;

	//�ǂ̍X�V
	CreateWalls();

	//�����̍X�V
	for (auto roof : GetRoofs())
	{
		roof->UpdateSize( m_Size);
	}
}

/*------------------------------------------------------------------------------
	�g��k���i�䗦�j
------------------------------------------------------------------------------*/
void ShapeBox::ScaleRate(const Vector3& rate)
{
	m_Size.x *= rate.x;
	m_Size.y *= rate.y;
	m_Size.z *= rate.z;

	//�ǂ̍X�V
	CreateWalls();

	//�����̍X�V
	for (auto roof : GetRoofs())
	{
		roof->UpdateSize( m_Size);
	}
}

/*------------------------------------------------------------------------------
	�`����m�肳����
------------------------------------------------------------------------------*/
void ShapeBox::ConfirmShape(void)
{
	//4�̕ǂ�Z������
	auto walls = GetWalls();
	auto wall = walls.front();
	for (auto other : walls)
	{
		if (wall == other)
		{
			continue;
		}
		wall->FusionSameShape( other);
		SubWall( other);
	}

	//�`����̍X�V
	wall->UpdateView( GetMatrix());
}


