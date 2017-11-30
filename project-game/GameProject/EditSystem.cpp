/*==============================================================================
	
	EditSystem.cpp - �G�f�B�b�g�V�X�e��
														Author : Yutaka Suganuma
														Date   : 2017/11/17
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "EditSystem.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "Light.h"

#include "InputMouse.h"
#include "CollisionManager.h"

/*------------------------------------------------------------------------------
	�ÓI�����o�ϐ��錾
------------------------------------------------------------------------------*/
EditSystem* EditSystem::Instance = NULL;

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* EditSystem::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<EditSystem>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
EditSystem::EditSystem( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	SelectedGameObject = NULL;

	if (Instance != NULL)
	{
		ReleaseReserve();
		return;
	}
	Instance = this;
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void EditSystem::Uninit( void)
{
	Instance = NULL;
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void EditSystem::Update()
{
	if (ImGui::IsAnyWindowHovered())
	{
		return;
	}

	//�I�u�W�F�N�g��I��
	if (Manager::GetInputMouse()->GetClickTrigger(0))
	{
		//���C�̎擾
		auto ray = Camera::GetMainCamera()->CalcScreenPointToRay(Manager::GetInputMouse()->GetScreenPosition());
		RaycastHit hit;
		if (Manager::GetCollisionManager()->Raycast(ray, -1.0f, &hit))
		{
			if( SelectedGameObject)
			{
				SelectedGameObject->IsSelected = false;
			}
			SelectedGameObject = hit.pGameObject;
			SelectedGameObject->IsSelected = true;
		}
		else
		{
			if( SelectedGameObject)
			{
				SelectedGameObject->IsSelected = false;
				SelectedGameObject = NULL;
			}
		}
	}
}
