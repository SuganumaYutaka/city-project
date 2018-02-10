/*==============================================================================

    RoadView.cpp - ���̎��������[���H�r���[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "RoadView.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "Polygon3DRenderer.h"
#include "CityAttribute.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* RoadView::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<RoadView>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
RoadView::RoadView( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Attribute = NULL;
	m_IsUpdatedAttribute = false;

	//�����_���[�̐ݒ�
	m_GrayRenderer = m_pGameObject->AddComponent<MeshPolygonRenderer>();
	m_GrayRenderer->LoadTexture( "data/TEXTURE/gray.jpg");

	m_WhiteLineRenderer = m_pGameObject->AddComponent<MeshPolygonRenderer>();
	m_WhiteLineRenderer->LoadTexture( "data/TEXTURE/whiteline.png");
	m_WhiteLineRenderer->SetPass( 1);
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void RoadView::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void RoadView::Update( void)
{
	if (!m_IsUpdatedAttribute)
	{
		return;
	}

	m_IsUpdatedAttribute = false;

	//�`����̍X�V
	UpdateRenderer();
}

/*------------------------------------------------------------------------------
	�������̐ݒ�
------------------------------------------------------------------------------*/
void RoadView::SetAttribute( RoadAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;

	//�`����̍X�V
	UpdateRenderer();
}

/*------------------------------------------------------------------------------
	�`����̍X�V
------------------------------------------------------------------------------*/
void RoadView::UpdateRenderer(void)
{
	//���H�̒��_���擾
	auto& vertices = m_Attribute->GetVertices();
	
	//�����_���Z������
	float width = m_Attribute->GetWidth();
	Vector3 vector = m_Attribute->GetVector().Normalize();
	vertices[0] += vector * width * ( 0.5f - 0.03f);
	vertices[1] -= vector * width * ( 0.5f - 0.03f);
	vertices[2] -= vector * width * ( 0.5f - 0.03f);
	vertices[3] += vector * width * ( 0.5f - 0.03f);

	//�A�X�t�@���g
	m_GrayRenderer->SetVertices( vertices);

	//����
	for (auto& vertex : vertices)
	{
		vertex.y += 0.01f;
	}
	m_WhiteLineRenderer->SetVertices( vertices);
}

