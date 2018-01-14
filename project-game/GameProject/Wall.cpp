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
#include "BuildingRule.h"

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
void Wall::InitDefault( D3DXMATRIX shapeMatrix, float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, BuildingRule* rule)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = normal;

	//�t���A�̐���
	rule->ProceduralFloor( this);

	UpdateView( shapeMatrix);
}

/*------------------------------------------------------------------------------
	�������i�~�ɉ����ċȂ���j
------------------------------------------------------------------------------*/
void Wall::InitCurve( D3DXMATRIX shapeMatrix, float height, float width, const Vector3& bottomLeftPosition,  BuildingRule* rule)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = bottomLeftPosition.Normalize();

	//�t���A�̐���
	rule->ProceduralFloorCurve( this);

	UpdateView( shapeMatrix);
}

/*------------------------------------------------------------------------------
	�`����̍X�V
------------------------------------------------------------------------------*/
bool Wall::UpdateView( D3DXMATRIX shapeMatrix)
{
	//Renderer�̍X�V
	auto pVtx = m_Renderer->StartSetVertexBuffer( CulcCountVertex() + 1, CulcCountPolygon());
	for (auto floor : m_Floors)
	{
		floor->Transform( shapeMatrix);
		floor->SetVertexBuffer( pVtx);
		pVtx += floor->CulcCountVertex();
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
	�t���A�̒ǉ�
------------------------------------------------------------------------------*/
void Wall::AddFloor( Floor* floor)
{ 
	m_Floors.push_back( floor);
}

/*------------------------------------------------------------------------------
	�t���A�̍폜
------------------------------------------------------------------------------*/
void Wall::SubFloor(Floor* floor)
{
	for (auto ite = m_Floors.begin(); ite != m_Floors.end(); ++ite)
	{
		if (*ite == floor)
		{
			delete floor;
			m_Floors.erase( ite);
			return;
		}
	}

	DebugLog::Add( "Wall��Floor��������Ȃ�\n");
}

/*------------------------------------------------------------------------------
	�e�N�X�`���̃��[�h
------------------------------------------------------------------------------*/
void Wall::LoadTexture(std::string fileName)
{
	m_Renderer->LoadTexture( fileName);
}

/*------------------------------------------------------------------------------
	����Shape��Wall�ƗZ������
------------------------------------------------------------------------------*/
void Wall::FusionSameShape(Wall* other)
{
	//Floor�̗Z��
	auto destFloor = other->m_Floors.begin();
	for (auto sourceFloor = m_Floors.begin(); sourceFloor != m_Floors.end(); ++sourceFloor)
	{
		(*sourceFloor)->FusionSameShape( (*destFloor));
		++destFloor;
	}

	//Renderer�̏���
	other->m_Renderer->ReleaseReserve();
	other->m_Renderer = NULL;
}

/*------------------------------------------------------------------------------
	�󃊃X�g�ɕύX
------------------------------------------------------------------------------*/
bool Wall::ChangeRingList(void)
{
	for (auto floor : m_Floors)
	{
		if (!floor->ChangeRingList())
		{
			return false;
		}
	}

	return true;
}