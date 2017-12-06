/*==============================================================================

    BlockView.cpp - ���̎��������[���r���[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BlockView.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "Polygon3DRenderer.h"
#include "CityAttribute.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* BlockView::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BlockView>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BlockView::BlockView( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	
	m_Attribute = NULL;
	m_IsUpdatedAttribute = false;

	//�����_���[�̐ݒ�
	
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void BlockView::Uninit( void)
{
	m_Attribute->UnregisterView();
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void BlockView::Update( void)
{
	if (!m_IsUpdatedAttribute)
	{
		return;
	}

	m_IsUpdatedAttribute = false;

	//�������Đ���
	m_Attribute->CreateBuilding();
}

/*------------------------------------------------------------------------------
	�������̐ݒ�
------------------------------------------------------------------------------*/
void BlockView::SetAttribute( BlockAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;

	//�����𐶐�
	m_Attribute->CreateBuilding();
}

