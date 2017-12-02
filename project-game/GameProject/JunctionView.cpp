/*==============================================================================

    JunctionView.h - ���̎��������[�����_�r���[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "JunctionView.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "Polygon3DRenderer.h"
#include "CityAttribute.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* JunctionView::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<JunctionView>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
JunctionView::JunctionView( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Attribute = NULL;
	m_IsUpdatedAttribute = false;

	//�����_���[�̐ݒ�
	m_Renderer = m_pGameObject->AddComponent<Polygon3DRenderer>();
	
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void JunctionView::Uninit( void)
{
	m_Attribute->UnregisterView();
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void JunctionView::Update( void)
{
	if (!m_IsUpdatedAttribute)
	{
		return;
	}

	m_IsUpdatedAttribute = false;

	//View���̍X�V
	m_pTransform->SetLocalPosition( m_Attribute->GetPosition());
	m_pTransform->SetLocalPositionY( 0.1f);
}

/*------------------------------------------------------------------------------
	�������̐ݒ�
------------------------------------------------------------------------------*/
void JunctionView::SetAttribute( JunctionAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;

	//View���̍X�V
	m_pTransform->SetLocalPosition( m_Attribute->GetPosition());
	m_pTransform->SetLocalPositionY( 0.1f);
}

