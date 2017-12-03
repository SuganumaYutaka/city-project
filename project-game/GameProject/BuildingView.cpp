/*==============================================================================

    BuildingView.cpp - ���̎��������[�����r���[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/3
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BuildingView.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "Building.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* BuildingView::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BuildingView>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BuildingView::BuildingView( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Building = NULL;

	//�����_���[�̐ݒ�
	m_Renderer = m_pGameObject->AddComponent<MeshPolygonRenderer>();
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void BuildingView::Uninit( void)
{
	m_Building->UnregisterView();
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void BuildingView::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	�������̐ݒ�
------------------------------------------------------------------------------*/
void BuildingView::SetBuilding( Building* building)
{ 
	if( m_Building)
	{	
		return;
	}
	
	m_Building = building;

	//View���̍X�V
	m_Renderer->SetVertices( m_Building->GetVertices());
}

