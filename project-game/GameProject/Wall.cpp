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
#include "TileSplit.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define SPLIT_EPSILON (0.00001f)

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
	m_IsCurve = false;

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
	m_IsCurve = true;

	//�t���A�̐���
	rule->ProceduralFloorCurve( this);

	UpdateView( shapeMatrix);
}

/*------------------------------------------------------------------------------
	�`����̍X�V
------------------------------------------------------------------------------*/
bool Wall::UpdateView( D3DXMATRIX shapeMatrix)
{
	//�ǂ̍X�V
	Transform( shapeMatrix);

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
	�ʒu�Ɩ@���̍X�V
------------------------------------------------------------------------------*/
void Wall::Transform(D3DXMATRIX shapeMatrix)
{
	auto position = m_BottomLeftPosition.ConvertToDX();
	auto normal = m_Normal.ConvertToDX();

	D3DXVec3TransformCoord( &position, &position, &shapeMatrix);
	D3DXVec3TransformNormal( &normal, &normal, &shapeMatrix);

	m_BottomLeftPosition = Vector3::ConvertFromDX( position);
	m_Normal = Vector3::ConvertFromDX( normal);
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

/*------------------------------------------------------------------------------
	�x�N�g���̎擾
------------------------------------------------------------------------------*/
Vector3 Wall::GetVector(void)
{
	if (m_IsCurve)
	{
		return Vector3();
	}
	auto cross = Vector3::Cross( m_Normal, Vector3( 0.0f, 1.0f, 0.0f));
	return cross.Normalize() * m_Width;
}

/*------------------------------------------------------------------------------
	���S�ʒu�̎擾
------------------------------------------------------------------------------*/
Vector3 Wall::GetCenter(void)
{
	if (!m_IsCurve)
	{
		return Vector3();
	}

	return m_BottomLeftPosition - m_Normal * GetRadius();
}

/*------------------------------------------------------------------------------
	���a�̎擾
------------------------------------------------------------------------------*/
float Wall::GetRadius(void)
{
	if (!m_IsCurve)
	{
		return 0.0f;
	}

	return m_Width * 0.5f / D3DX_PI;
}

/*------------------------------------------------------------------------------
	�ǂƂ̗􂯖ڏ���
------------------------------------------------------------------------------*/
bool Wall::Split(Wall* other)
{
	if (m_IsCurve)
	{
		if (other->m_IsCurve)
		{
			if (!SplitCylinders(this, other))
			{
				return false;
			}
		}
		else
		{
			if (!SplitPlaneCylinder(other, this))
			{
				return false;
			}
		}
	}
	else
	{
		if (other->m_IsCurve)
		{
			if (!SplitPlaneCylinder(this, other))
			{
				return false;
			}
		}
		else
		{
			if (!SplitPlanes(this, other))
			{
				return false;
			}
		}
	}

	return true;
}

/*------------------------------------------------------------------------------
	�ǂƂ̗􂯖ڏ����i���ʂƕ��ʁj
------------------------------------------------------------------------------*/
bool Wall::SplitPlanes(Wall* source, Wall* dest)
{
	//�ǂƂ̌�_���Z�o
	Vector3 positionSplit;
	if (!CulcPositionSplitPlanes(source, dest, &positionSplit))
	{
		return false;
	}

	//�n�_����̋������Z�o
	float length1 = Vector3::Distance( source->GetBottomLeftPosition(), positionSplit);
	float length2 = Vector3::Distance( dest->GetBottomLeftPosition(), positionSplit);
	
	//�Ō�̗]�����������t���A�K�w�����ڂ�ݒ�
	auto floors1 = source->GetFloors();
	auto floors2 = dest->GetFloors();

	if (source->GetWidth() < length1)
	{
		return false;
	}
	else if (dest->GetWidth() < length2)
	{
		return false;
	}

	//�K�������Ȃ��ǂ���Ƃ��đ}������
	if (floors1.size() <= floors2.size())
	{
		if (!InsertSplit(floors1, floors2, length1, length2, positionSplit))
		{
			return false;
		}
	}
	else
	{
		if (!InsertSplit(floors2, floors1, length2, length1, positionSplit))
		{
			return false;
		}
	}

	return true;
}

/*------------------------------------------------------------------------------
	���ڂ̑}��
------------------------------------------------------------------------------*/
bool Wall::InsertSplit(const std::list<Floor*>& floors1, const std::list<Floor*>& floors2, float length1, float length2, Vector3 positionSplit)
{
	auto ite2 = floors2.begin();
	//for (auto ite1 = floors1.begin(); *ite1 != floors1.back(); ++ite1, ++ite2)
	//for (auto ite1 = floors1.begin(); ite1 != floors1.end(); ++ite1, ++ite2)
	auto ite1 = floors1.begin();
	for(;;)
	{
		if (ite1 == floors1.end())
		{
			 break;
		}
		if ((*ite1)->GetType() == eFloorMargin)
		{
			break;
		}

		//���ڂƂȂ�^�C���𐶐����Ċe�t���A�ɑ}������
		TileSplit* split1 = new TileSplit();
		TileSplit* split2 = new TileSplit();
		split1->Init( split2, positionSplit);
		split2->Init( split1, positionSplit);

		if (!(*ite1)->InsertSplit(split1, length1))
		{
			delete split1;
			delete split2;
			return false;
		}
		if (!(*ite2)->InsertSplit(split2, length2))
		{
			delete split1;
			delete split2;
			return false;
		}

		//���ڂ̈ʒu�����������炷
		positionSplit.y += (*ite1)->GetHeight();

		++ite1;
		++ite2;
	}

	return true;
}

/*------------------------------------------------------------------------------
	�ǂƂ̗􂯖ڏ����i�~���Ɖ~���j
------------------------------------------------------------------------------*/
bool Wall::SplitCylinders(Wall* source, Wall* dest)
{


	return true;
}

/*------------------------------------------------------------------------------
	�ǂƂ̗􂯖ڏ����i���ʂƉ~���j
------------------------------------------------------------------------------*/
bool Wall::SplitPlaneCylinder(Wall* plane, Wall* cylinder)
{


	return true;
}

/*------------------------------------------------------------------------------
	�ǂƂ̌�_���Z�o�i���ʂƕ��ʁj
------------------------------------------------------------------------------*/
bool Wall::CulcPositionSplitPlanes(Wall* source, Wall* dest, Vector3* out)
{
	//�ϐ��̐ݒ�
	Vector3 s1 = source->GetBottomLeftPosition();
	Vector3 s2 = dest->GetBottomLeftPosition();
	Vector3 v1 = source->GetVector();
	Vector3 v2 = dest->GetVector();
	Vector3 n1 = v1.Normalize();
	Vector3 n2 = v2.Normalize();
	float work1 = Vector3::Dot( n1, n2);
	float work2 = 1 - work1 * work1;

	//���s���m�F
	if( work2 > -SPLIT_EPSILON && work2 < SPLIT_EPSILON){ return 0;}

	//�n�_�����ԃx�N�g��
	Vector3 vs = s2 - s1;

	//�n�_����̋���
	float d1 = ( Vector3::Dot( vs, n1) - work1 * Vector3::Dot( vs, n2)) / work2;
	float d2 = ( work1 * Vector3::Dot( vs, n1) - Vector3::Dot( vs, n2)) / work2;

	//v1��̍ŋߓ_
	Vector3 p1;
	p1.x = s1.x + d1 * n1.x;
	p1.y = s1.y + d1 * n1.y;
	p1.z = s1.z + d1 * n1.z;

	//v2��̍ŋߓ_
	Vector3 p2;
	p2.x = s2.x + d2 * n2.x;
	p2.y = s2.y + d2 * n2.y;
	p2.z = s2.z + d2 * n2.z;

	//��������
	if (Vector3::Distance(p1, p2) > 0.01f)
	{
		return false;
	}

	//�����̒�������
	if (v1.Length() < d1)
	{
		return false;
	}
	if (v2.Length() < d2)
	{
		return false;
	}

	//�����̕�������
	auto vec1 = p1 - s1;
	auto dot1 = Vector3::Dot( n1, vec1.Normalize());
	if (dot1 < 0)
	{
		return false;
	}
	auto vec2 = p2 - s2;
	auto dot2 = Vector3::Dot( n2, vec2.Normalize());
	if (dot2 < 0)
	{
		return false;
	}
	
	if(out)
	{
		*out = p1;
	}

	return true;
	
	////���ꕽ�ʁi�n�ʁj�ł̐����Ɛ����̌�_�����߂�
	//Vector2 _s1 = Vector2( source->GetBottomLeftPosition().x, source->GetBottomLeftPosition().z);
	//Vector2 _s2 = Vector2( dest->GetBottomLeftPosition().x, dest->GetBottomLeftPosition().z);
	//Vector2 _v1 = Vector2( source->GetVector().x, source->GetVector().z);
	//Vector2 _v2 = Vector2( dest->GetVector().x, dest->GetVector().z);
	//
	////�O�ςŕ��s���m�F
	//float crossV1V2 = Vector2::Cross( _v1, _v2);
	////if (crossV1V2 == 0.0f)
	//if( crossV1V2 > -0.01f && crossV1V2 < 0.01f)
	//{
	//	return false;
	//}

	////�n�_�����ԃx�N�g���Ɗe�x�N�g���Ƃ̊O�ς��Z�o
	//Vector2 _vs = _s2 - _s1;
	//float crossVSV1 = Vector2::Cross( _vs, _v1);
	//float crossVSV2 = Vector2::Cross( _vs, _v2);

	////�x�N�g���𕪊������i������j�������Ɏ��܂邩�ŏՓ˔���
	//float ratio1 = crossVSV2 / crossV1V2;
	//float ratio2 = crossVSV1 / crossV1V2;

	//Vector2 answer = _s1 + _v1 * ratio1;
	//Vector3 vec = Vector3( answer.x, 0.0f, answer.y);

	//if (ratio1 + SPLIT_EPSILON < 0 || ratio1 - SPLIT_EPSILON > 1 || ratio2 + SPLIT_EPSILON < 0 || ratio2 - SPLIT_EPSILON > 1)
	//{
	//	return false;
	//}

	////��_���Z�o
	//if(out)
	//{
	//	*out = Vector3( answer.x, 0.0f, answer.y);
	//}

	//return true;
}

/*------------------------------------------------------------------------------
	�t���A�̐擪�^�C�����擾
------------------------------------------------------------------------------*/
Tile* Wall::GetStartTile(int floorCount)
{
	//�K�w����v����t���A�̐擪�^�C����Ԃ�
	int count = 0;
	for (auto floor : m_Floors)
	{
		if (count == floorCount)
		{
			return floor->GetTile();
		}
		count++;
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	�����_���[�̏���
------------------------------------------------------------------------------*/
void Wall::ClearRenderer(void)
{
	if (m_Renderer)
	{
		m_Renderer->ReleaseReserve();
		m_Renderer = NULL;
	}
}

/*------------------------------------------------------------------------------
	��ԏ�̃^�C�����擾
------------------------------------------------------------------------------*/
Tile* Wall::GetTopTile(void)
{
	return m_Floors.back()->GetTile();
}
