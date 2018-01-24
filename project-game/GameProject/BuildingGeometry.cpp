/*==============================================================================

    BuildingGeometry.cpp - �����̎��������[�����W�I���g��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BuildingGeometry.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "Land.h"
#include "Shape.h"
#include "BuildingRule.h"

#include "Tile.h"
#include "TileSplit.h"
#include "WallRenderer.h"
#include "BuildingSurfacePattern.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define MAX_TILE (300)

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* BuildingGeometry::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BuildingGeometry>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BuildingGeometry::BuildingGeometry( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void BuildingGeometry::Uninit( void)
{
	if (m_Rule)
	{
		delete m_Rule;
	}

	if (m_Land)
	{
		delete m_Land;
	}
	
	for (Shape* shape : m_Shapes)
	{
		delete shape;
	}
	m_Shapes.clear();
}

/*------------------------------------------------------------------------------
	����������
------------------------------------------------------------------------------*/
bool BuildingGeometry::Init( const std::vector<Vector3>& vertices, BuildingRule* rule)
{
	//�y�n�̐���
	m_Land = new Land( m_pGameObject);
	
	//Position(World���W)��y�n�̒��S��
	Vector3 vec02 = vertices[2] - vertices[0];
	auto center = vertices[0] + vec02 * 0.5f;
	m_pTransform->SetWorldPosition( center);

	//�y�n�����H�ƕ��s�ƂȂ�悤�ɉ�]
	Vector3 vec21 = vertices[1] - vertices[2];
	m_pTransform->SetWorldRotationLookDirection( vec21);

	//���Έʒu���Z�o���ēy�n��ݒ�
	std::vector<Vector3> landVertices = vertices;
	for (auto& vertex : landVertices)
	{
		vertex -= center;
	}
	m_Land->Init( landVertices);

	//�`��̐���
	rule->ProceduralShape( this);

	m_Rule = rule;

	return true;
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void BuildingGeometry::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	�W�I���g�����m�肳����
------------------------------------------------------------------------------*/
void BuildingGeometry::ConfirmGeometry(void)
{
	//�P��Shape�̏ꍇ
	if (m_Shapes.size() == 1)
	{
		//�ǂ̗Z��
		m_Shapes.front()->ConfirmShape();

		//�`��̍X�V
		m_Shapes.front()->UpdateView();

		return;
	}

	//Shape�̗Z��
	//Shape���m�ň�x������
	for (auto ite1 = m_Shapes.begin(); *ite1 != m_Shapes.back(); ++ite1)
	{
		auto ite2 = ite1;
		++ite2;
		for (; ite2 != m_Shapes.end(); ++ite2)
		{
			//��������Ƃ���ɗ􂯖ڂ�ݒ�
			(*ite1)->Split( *ite2);
		}
	}

	//�`����m�肳����
	for (auto shape : m_Shapes)
	{
		shape->ConfirmShape();
	}

	//�ǂ�Renderer�̏���
	for (auto shape : m_Shapes)
	{
		shape->ClearWallRenderer();
	}

	//�eShape����t���A�����^�C���̐擪���擾
	std::list<Tile*> tiles;
	int floorCount = 0;
	for (;;)
	{
		//�擪�^�C�����擾
		Tile* tile = NULL;
		for (auto shape : m_Shapes)
		{
			tile = SearchStartTile( shape, floorCount);
			if (tile)
			{
				//�e�t���A�ɂ��P������΂n�j
				break;
			}
		}

		//�\���ł���擪�^�C�����Ȃ��Ƃ��i�[�I��
		if (!tile)
		{
			break;
		}
		
		//�^�C�����i�[���ĊK������グ��
		tiles.push_back( tile);
		floorCount++;
	}

	//�eShape�̍Ō��Floor�̐擪�^�C�����i�[
	for (auto shape : m_Shapes)
	{
		auto tile = shape->GetTopTile();
		tiles.push_back( tile);
	}
	

	//1����WallRenderer�ɕ\�ɏo�Ă���S�t���A�̑S�^�C���̕`�����ݒ�
	//���_���ƃ|���S�������Z�o
	int vertexCount = 0;
	int polygonCount = 0;

	for (auto start : tiles)
	{
		//�^�C���̃��X�g�����ǂ��Ē��_���E�|���S���������Z
		auto tile = start;
		int tileCount = 0;
		for (;;)
		{
			vertexCount += tile->CulcCountVertex();
			polygonCount += tile->CulcCountPolygon();

			tile = tile->GetNext();
			if (!tile || tile == start)
			{
				break;
			}

			//����Shape�Ƃ̂Ȃ��ڂ�����Ƃ�����Shape�̃^�C���ɃW�����v����
			if (tile->GetType() == eTileSplit)
			{
				auto split = dynamic_cast<TileSplit*>( tile);
				if (split)
				{
					tile = split->GetOtherSplit();
					
					//prev���ݒ�
					auto prev = split->GetOtherSplit()->GetPrevTile();
					vertexCount += prev->CulcCountVertex();
					polygonCount += prev->CulcCountPolygon();
				}
			}

			//�^�C�����ő吔�𒴂�����I��
			tileCount++;
			if (tileCount > MAX_TILE)
			{
				break;
			}
		}
	}

	//WallRenderer��ݒ�
	auto wallRenderer = m_pGameObject->AddComponent<WallRenderer>();
	wallRenderer->LoadTexture( m_Rule->GetSurfacePattern()->GetTextureFileName());
	auto pVtx = wallRenderer->StartSetVertexBuffer( vertexCount + 1, polygonCount);
	for (auto start : tiles)
	{
		//�^�C���̃��X�g�����ǂ��Ē��_����ݒ�
		auto tile = start;
		int tileCount = 0;
		for (;;)
		{
			tile->SetVertexBuffer(pVtx);
			pVtx += tile->CulcCountVertex();

			tile = tile->GetNext();
			if (!tile || tile == start)
			{
				break;
			}

			//����Shape�Ƃ̂Ȃ��ڂ�����Ƃ�����Shape�̃^�C���ɃW�����v����
			if (tile->GetType() == eTileSplit)
			{
				auto split = dynamic_cast<TileSplit*>( tile);
				if (split)
				{
					tile = split->GetOtherSplit();
					
					//prev���ݒ�
					auto prev = split->GetOtherSplit()->GetPrevTile();
					prev->SetVertexBuffer(pVtx);
					pVtx += prev->CulcCountVertex();
				}
			}

			//�^�C�����ő吔�𒴂�����I��
			tileCount++;
			if (tileCount > MAX_TILE)
			{
				break;
			}
		}
	}
	wallRenderer->EndSetVertexBuffer();

}

/*------------------------------------------------------------------------------
	�w��K�w�̕\�ɂ���擪�^�C����T��
------------------------------------------------------------------------------*/
Tile* BuildingGeometry::SearchStartTile(Shape* shape, int floorCount)
{
	//�t���A�̈�ԏ�͐ݒ肵�Ȃ�
	if (floorCount >= shape->GetFloorCount() - 1)
	{
		return NULL;
	}

	//Shape����擪�^�C�����擾
	auto tile = shape->GetStartTile( floorCount);
	if (!tile)
	{
		return NULL;
	}

	//�󃊃X�g�����ǂ��ĕ\�ɂ���^�C����������
	auto start = tile;
	for (;;)
	{
		//�^�C�����\�ɂ��邩����
		if (CanLookTile(tile, shape))
		{
			//�Ώۂ̃^�C������
			return tile;
		}

		tile = tile->GetNext();
		if (!tile || tile == start)
		{
			break;
		}
	}

	return NULL;
}

/*------------------------------------------------------------------------------
	�^�C�������F�ł��邩
------------------------------------------------------------------------------*/
bool BuildingGeometry::CanLookTile(Tile* tile, Shape* owner)
{
	for (auto otherShape : m_Shapes)
	{
		if (owner == otherShape)
		{
			continue;
		}

		//�^�C��������Shape�̒��ɂ��邩�m�F
		if ( otherShape->CollisionPoint(tile->GetBottomLeftPosition()))
		{
			//���F�ł��Ȃ�
			return false;
		}
	}
	
	//����Shape�ƏՓ˂Ȃ������F�ł���
	return true;
}
