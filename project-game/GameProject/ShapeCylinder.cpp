/*==============================================================================

    ShapeCylinder.cpp - �����̎��������[�{�b�N�X
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/9
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "ShapeCylinder.h"
#include "Wall.h"
#include "Roof.h"
#include "DebugLog.h"

#include "BuildingRule.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
ShapeCylinder::ShapeCylinder(GameObject* buildingObject)
{
	SetBuildingObject( buildingObject);
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
ShapeCylinder::~ShapeCylinder()
{
	Uninit();
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void ShapeCylinder::Init(const Vector3& position, float rotation, float height, float radius, BuildingRule* rule)
{
	SetPosition( position);
	SetRotation( rotation);
	m_Height = height;
	m_Radius = radius;
	m_Rule = rule;

	Vector3 size = Vector3( radius * 2.0f, height, radius * 2.0f);

	//�����̐���
	auto roof = new Roof(GetBuildingObject());
	roof->InitPlane( position, rotation, size, "data/TEXTURE/circle.png");
	AddRoof( roof);

	//�ǂ̐���
	CreateWalls();
}

/*------------------------------------------------------------------------------
	�ǂ̐���
------------------------------------------------------------------------------*/
void ShapeCylinder::CreateWalls(void)
{
	//�ǂ̏���
	if (GetWalls().size() != 0)
	{
		ClearWall();
	}

	auto matrix = GetMatrix();

	Vector3 bottomLeft = Vector3( 0.0f, 0.0f, m_Radius);
	auto wall = new Wall( GetBuildingObject());
	wall->InitCurve( matrix, m_Height, m_Radius * 2 * D3DX_PI, bottomLeft, m_Rule);
	AddWall( wall);
}

/*------------------------------------------------------------------------------
	�ړ�
------------------------------------------------------------------------------*/
void ShapeCylinder::Move(const Vector3& value)
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
void ShapeCylinder::Rotate( float value)
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
void ShapeCylinder::ScaleUpDown(const Vector3& value)
{
	m_Radius += value.x;
	m_Height += value.y;

	//�ǂ̍X�V
	CreateWalls();

	//�����̍X�V
	for (auto roof : GetRoofs())
	{
		roof->UpdateSize( Vector3(m_Radius * 2.0f, m_Height, m_Radius * 2.0f));
	}
}

/*------------------------------------------------------------------------------
	�g��k���i�䗦�E���{�j
------------------------------------------------------------------------------*/
void ShapeCylinder::ScaleRate( float rate)
{
	m_Radius *= rate;
	m_Height *= rate;

	//�ǂ̍X�V
	CreateWalls();

	//�����̍X�V
	for (auto roof : GetRoofs())
	{
		roof->UpdateSize( Vector3(m_Radius * 2.0f, m_Height, m_Radius * 2.0f));
	}
}

/*------------------------------------------------------------------------------
	�g��k���i�䗦�j
------------------------------------------------------------------------------*/
void ShapeCylinder::ScaleRate(const Vector3& rate)
{
	m_Radius *= rate.x;
	m_Height *= rate.y;

	//�ǂ̍X�V
	CreateWalls();

	//�����̍X�V
	for (auto roof : GetRoofs())
	{
		roof->UpdateSize( Vector3(m_Radius * 2.0f, m_Height, m_Radius * 2.0f));
	}
}

/*------------------------------------------------------------------------------
	�`����m�肳����
------------------------------------------------------------------------------*/
void ShapeCylinder::ConfirmShape(void)
{
	
}


