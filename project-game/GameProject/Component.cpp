/*==============================================================================

    Component.cpp - �R���|�[�l���g�i���N���X�j
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/27
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Component.h"
#include "GameObject.h"

/*------------------------------------------------------------------------------
	�ÓI�����o�ϐ��錾
------------------------------------------------------------------------------*/
std::list<Component*> Component::m_listReleaseComponent;

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Component::Component()
{
	m_pGameObject = NULL;
	m_pTransform = NULL;
	m_nLayer = eLayerDefault;
	m_bEnable = true;
	m_bRelease = false;
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
	����
	CScene *pScene
------------------------------------------------------------------------------*/
Component::Component( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_bEnable = true;
	m_bRelease = false;
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Component::~Component()
{

}

/*------------------------------------------------------------------------------
	�����\��
------------------------------------------------------------------------------*/
void Component::ReleaseReserve(void)
{
	if (m_bRelease == true)
	{
		return;
	}

	m_bRelease = true;
	m_bEnable = false;

	m_listReleaseComponent.push_back(this);

}

/*------------------------------------------------------------------------------
	���X�g�̏��ɃR���|�[�l���g����
------------------------------------------------------------------------------*/
void Component::ReleaseList(void)
{
	for (Component* pComp : m_listReleaseComponent)
	{
		pComp->m_pGameObject->ReleaseComponent(pComp);
	}

	//���X�g�����Z�b�g
	m_listReleaseComponent.clear();
}



