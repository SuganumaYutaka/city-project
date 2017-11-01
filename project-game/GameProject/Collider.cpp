/*==============================================================================

    Collider.cpp - �R���C�_�[�i���N���X�j
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Collider.h"
#include "CollisionManager.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Collider::Collider()
{
	 m_ColType = eColNone;
	 m_IsTrigger = true;

	 Manager::GetCollisionManager()->AddCollider( this);
}

/*------------------------------------------------------------------------------
	�A�N�e�B�u�ݒ�
------------------------------------------------------------------------------*/
void Collider::SetActive(bool bEnable)
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
}
