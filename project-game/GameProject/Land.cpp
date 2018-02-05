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

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Land::Land(GameObject* parent)
{
	m_GameObject = new GameObject( parent);
	m_Renderer = m_GameObject->AddComponent<MeshPolygonRenderer>();
	m_TrafficLand = NULL;
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Land::~Land()
{
	m_GameObject->ReleaseReserve();

	m_Vertices.clear();
	m_Vertices.shrink_to_fit();
}

/*------------------------------------------------------------------------------
	����������
------------------------------------------------------------------------------*/
void Land::Init( const std::vector<Vector3>& vertices)
{
	m_Vertices.clear();
	m_Vertices = vertices;
	m_Renderer->SetVertices( vertices);
}

/*------------------------------------------------------------------------------
	���������̏���
------------------------------------------------------------------------------*/
void Land::OnDeleteBlock(void)
{
	
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

