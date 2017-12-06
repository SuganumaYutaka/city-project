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
	m_Renderer = m_pGameObject->AddComponent<Polygon3DRenderer>();
	m_Renderer->LoadTexture( "data/TEXTURE/field.jpg");
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void RoadView::Uninit( void)
{
	m_Attribute->UnregisterView();
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

	//View���̍X�V
	m_Renderer->m_pTransform->SetLocalPosition( m_Attribute->GetCenterPosition());
	m_Renderer->m_pTransform->SetWorldRotationLookDirection( m_Attribute->GetVector());
	m_Renderer->m_pTransform->SetLocalScale( m_Attribute->GetWidth(), 1.0f, m_Attribute->GetLength());
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

	//View���̍X�V
	m_Renderer->m_pTransform->SetLocalPosition( m_Attribute->GetCenterPosition());
	m_Renderer->m_pTransform->SetWorldRotationLookDirection( m_Attribute->GetVector());
	m_Renderer->m_pTransform->SetLocalScale( m_Attribute->GetWidth(), 1.0f, m_Attribute->GetLength());
}
