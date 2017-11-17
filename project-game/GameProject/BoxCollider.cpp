/*==============================================================================

    BoxCollider.cpp - �{�b�N�X�R���C�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BoxCollider.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshBoxRenderer.h"

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* BoxCollider::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BoxCollider>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BoxCollider::BoxCollider( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = pGameObject->GetComponent<Transform>();
	m_ColType = eColBox;
	m_Size = Vector3(1.0f, 1.0f, 1.0f);

	//�f�o�b�O�\��
#ifdef _DEBUG
	auto obj = new GameObject( m_pGameObject->GetRoot());
	obj->IsCreatedByOtherComponent = true;
	m_pRenderer = obj->AddComponent<MeshBoxRenderer>();
	m_pRenderer->SetWireFrame();
	obj->IsCreatedByOtherComponent = true;
#endif // _DEBUG

}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void BoxCollider::Uninit( void)
{
	Manager::GetCollisionManager()->ReleaseCollider( this);

#ifdef _DEBUG
	m_pRenderer->m_pGameObject->ReleaseReserve();
#endif // _DEBUG
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void BoxCollider::Update()
{
#ifdef _DEBUG
	m_pRenderer->m_pTransform->SetWorldPosition( m_pTransform->GetWorldPosition());

	//���S�ʒu�����炷
	D3DXVECTOR3 center = m_Center.ConvertToDX();
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationQuaternion( &mtxRot, &m_pTransform->GetWorldRotation());
	D3DXVec3TransformCoord( &center, &center, &mtxRot);
	m_pRenderer->m_pTransform->Move( Vector3::ConvertFromDX(center));

#endif // _DEBUG
}

/*------------------------------------------------------------------------------
	�Փ�
	����
		Collider *pCollider			�Փ˂����R���C�_�[
------------------------------------------------------------------------------*/
void BoxCollider::OnCollision( Collider *pCollider)
{
	
}

/*------------------------------------------------------------------------------
	���S�ʒu�ݒ�
------------------------------------------------------------------------------*/
void BoxCollider::SetCenter(const Vector3& Center)
{
	m_Center = Center;

	//�f�o�b�O�\��
#ifdef _DEBUG
	m_pRenderer->SetCenter( Center);

#endif // _DEBUG
}

/*------------------------------------------------------------------------------
	�傫���ݒ�
------------------------------------------------------------------------------*/
void BoxCollider::SetSize(const Vector3& Size)
{
	m_Size = Size;

	//�f�o�b�O�\��
#ifdef _DEBUG
	m_pRenderer->SetSize( Size);

#endif // _DEBUG
}

/*------------------------------------------------------------------------------
	�A�N�e�B�u�ݒ�
------------------------------------------------------------------------------*/
void BoxCollider::SetActive(bool bEnable)
{
	if (m_bEnable == true && bEnable == false)
	{
		//�}�l�[�W���[����폜
		Manager::GetCollisionManager()->ReleaseCollider( this);
	}
	else if (m_bEnable == false && bEnable == true)
	{
		//�}�l�[�W���[�ɓo�^
		Manager::GetCollisionManager()->AddCollider( this);
	}

	m_bEnable = bEnable;

#ifdef _DEBUG
	m_pRenderer->SetActive( bEnable);

#endif // _DEBUG

}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void BoxCollider::Load(Text& text)
{
	//text��ǂݐi�߂�
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "Center")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_Center.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}
		if (text.GetWord() == "Size")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_Size.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}
		if (text.GetWord() == "IsTrigger")
		{
			text.ForwardPositionToNextWord();
			m_IsTrigger = (bool)std::stoi(text.GetWord());
		}

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
void BoxCollider::Save(Text& text)
{
	StartSave(text);

	text += "Center " + m_Center.ConvertToString() + '\n';
	text += "Size " + m_Size.ConvertToString() + '\n';
	text += "IsTrigger" + std::to_string((int)m_IsTrigger) ; '\n';

	EndSave( text);
}