/*==============================================================================

    Land.h - ���̎��������[�y�n
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Land.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "TrafficLand.h"
#include "MeshPolygonRenderer.h"
#include "CityAttributeManager.h"
#include "CityAttribute.h"
#include "LandManager.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Land::Land( LandManager* manager, GameObject* parent)
{
	m_GameObject = new GameObject( parent);
	m_Renderer = m_GameObject->AddComponent<MeshPolygonRenderer>();
	m_TrafficLand = NULL;
	m_AttributeManager = NULL;
	m_BlockID = -1;

	m_Manager = manager;
	m_Manager->RegisterLand( this);
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Land::~Land()
{
	
}

/*------------------------------------------------------------------------------
	����
------------------------------------------------------------------------------*/
void Land::Delete(void)
{
	m_GameObject->ReleaseReserve();

	m_Vertices.clear();
	m_Vertices.shrink_to_fit();

	//�����N������
	if (m_AttributeManager)
	{
		m_AttributeManager->GetBlock( m_BlockID)->UnlinkLand( this);
	}
	m_Manager->UnregisterLand( this);
}

/*------------------------------------------------------------------------------
	����������
------------------------------------------------------------------------------*/
void Land::Init( const std::vector<Vector3>& vertices)
{
	//���S�ʒu��ݒ�
	Vector3 vec02 = vertices[2] - vertices[0];
	Vector3 center = vertices[0] + vec02 * 0.5f;

	//�S���_�𒆐S����̑��΍��W�ɂ���
	m_Vertices.clear();
	for (auto vertex : vertices)
	{
		m_Vertices.push_back( vertex - center);
	}
	m_Renderer->SetVertices( m_Vertices);

	//GameObject���ړ�
	m_GameObject->m_pTransform->SetWorldPosition( center);
}

/*------------------------------------------------------------------------------
	���������̏���
------------------------------------------------------------------------------*/
void Land::OnDeleteBlock(void)
{
	//�f�X�g���N�^���Ƀ����N�̉��������Ȃ�
	m_AttributeManager = NULL;
	m_BlockID = -1;

	//��������
	delete this;
}

/*------------------------------------------------------------------------------
	���S�ʒu���Z�o
------------------------------------------------------------------------------*/
Vector3 Land::CulcCenterPosition(void)
{
	Vector3 vec02 = m_Vertices[2] - m_Vertices[0];
	return m_Vertices[0] + vec02 * 0.5f;
}

/*------------------------------------------------------------------------------
	���ƃ����N����
------------------------------------------------------------------------------*/
void Land::LinkAttribute(CityAttributeManager* attributeManager, int blockID)
{
	m_AttributeManager = attributeManager;
	m_BlockID = blockID;

	m_AttributeManager->GetBlock( m_BlockID)->LinkLand( this);
}

/*------------------------------------------------------------------------------
	��ʃV�X�e���̐ݒ�
------------------------------------------------------------------------------*/
void Land::SetTraffic(std::list<RoadAttribute*> roadAttributes)
{
	if (!m_TrafficLand)
	{
		m_TrafficLand = m_GameObject->AddComponent<TrafficLand>();
	}

	m_TrafficLand->Init( roadAttributes);
}

/*------------------------------------------------------------------------------
	�����������N
------------------------------------------------------------------------------*/
int Land::LinkBuilding(Building* building)
{
	m_Buildings.push_back( building);
	return m_Buildings.size() - 1;
}

/*------------------------------------------------------------------------------
	�����̃����N�̉���
------------------------------------------------------------------------------*/
void Land::UnlinkBuilding(Building* building)
{
	int size = m_Buildings.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Buildings[i] == building)
		{
			m_Buildings[i] = NULL;
		}
	}
}

/*------------------------------------------------------------------------------
	�����̃����N�̉���
------------------------------------------------------------------------------*/
void Land::UnlinkBuilding(int buildingID)
{
	m_Buildings[ buildingID] = NULL;
}

