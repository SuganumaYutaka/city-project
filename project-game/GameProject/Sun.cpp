/*==============================================================================
	
	Sun.cpp - ���z
														Author : Yutaka Suganuma
														Date   : 2017/8/16
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Sun.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "Light.h"

#include "InputKeyboard.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define DEFAULT_FIELD_SIZE (500.0f)

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* Sun::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<Sun>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Sun::Sun( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	//���C�g
	GameObject *pLight = new GameObject( pGameObject);
	pLight->IsCreatedByOtherComponent = true;
	m_pLight = pLight->AddComponent<Light>();
	m_pLight->IsCreatedByOtherComponent = true;
	
	//���C�g�̃J����
	m_pCamera = pLight->AddComponent<Camera>();
	m_pCamera->IsCreatedByOtherComponent = true;
	m_pCamera->ChangeLightCameraFormat( Vector3( DEFAULT_FIELD_SIZE, DEFAULT_FIELD_SIZE, DEFAULT_FIELD_SIZE));
	m_pCamera->SetPosAt( Vector3(0.0f, 0.0f, 0.0f));
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void Sun::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void Sun::Update()
{
	//��]
	//Vector3 Axis = Vector3( 0.5f, 0.3f, 0.0f);
	//m_pTransform->RotateAxis( Axis, 0.01f);

	//�ړ�
	/*Vector3 move;
	if (Manager::GetInputKeyboard()->GetKeyPress(DIK_RIGHT))
	{
		move.x += 1.0f;
	}
	if (Manager::GetInputKeyboard()->GetKeyPress(DIK_LEFT))
	{
		move.x -= 1.0f;
	}
	if (Manager::GetInputKeyboard()->GetKeyPress(DIK_UP))
	{
		move.z += 1.0f;
	}
	if (Manager::GetInputKeyboard()->GetKeyPress(DIK_DOWN))
	{
		move.z -= 1.0f;
	}

	m_pTransform->Move(move);*/
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void Sun::Load(Text& text)
{
	//text��ǂݐi�߂�
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		//text��ǂݐi�߂�
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	�Z�[�u
------------------------------------------------------------------------------*/
void Sun::Save(Text& text)
{
	StartSave(text);

	

	EndSave( text);
}