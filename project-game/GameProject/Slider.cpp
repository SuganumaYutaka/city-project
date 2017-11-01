/*==============================================================================
	
	Slider.cpp - �X���C�_�[
														Author : Yutaka Suganuma
														Date   : 2017/7/19
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Slider.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "SpriteRenderer.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Slider::Slider( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	//�����l�ݒ�
	m_Size = Vector2( 200.0f, 50.0f);
	m_fMaxValue = 1.0f;
	m_fValue = 1.0f;

	//�����_���[�ǉ�
	GameObject* obj = new GameObject( pGameObject);
	m_pBack = obj->AddComponent<SpriteRenderer>();
	obj = new GameObject( pGameObject);
	m_pSlider = obj->AddComponent<SpriteRenderer>();

	//�X�V
	UpdateSprite();
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void Slider::Uninit( void)
{

}

/*------------------------------------------------------------------------------
	�X�v���C�g�����_���[�X�V
------------------------------------------------------------------------------*/
void Slider::UpdateSprite( void)
{
	Vector3 Size = Vector3( m_Size.x, m_Size.y, 1.0f);
	m_pBack->m_pTransform->SetLocalPosition( m_pTransform->GetLocalPosition());
	m_pBack->m_pTransform->SetLocalScale( Size);

	Size.x = m_Size.x * m_fValue / m_fMaxValue;
	Vector3 Pos = m_pTransform->GetLocalPosition();
	Pos.x += ( -m_Size.x + Size.x) * 0.5f;
	m_pSlider->m_pTransform->SetLocalPosition( Pos);
	m_pSlider->m_pTransform->SetLocalScale( Size);
}
