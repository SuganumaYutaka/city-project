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
#include "BlockModel.h"

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
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void BlockView::Uninit( void)
{
	
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
}

/*------------------------------------------------------------------------------
	�������̍X�V
------------------------------------------------------------------------------*/
void BlockView::UpdateAttribute(void)
{
	m_IsUpdatedAttribute = true;
	
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
}

