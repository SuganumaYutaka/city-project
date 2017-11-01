/*==============================================================================
	
	CameraController.h - �J��������
														Author : Yutaka Suganuma
														Date   : 2017/6/7
==============================================================================*/
#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Camera;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class CameraController : public Component
{
public:
	CameraController( GameObject* pGameObject);
	void Uninit( void);
	
	void Update();
	void OnCollision( Collider *pCollider);

	void SetTargetPosAt( Transform* pTarget) { m_pTargetPosAt = pTarget;}
	void SetTargetPosEye(Transform* pTarget, const Vector3& PosEyeFromTarget = Vector3())
	{
		m_pTargetPosEye = pTarget;
		m_PosEyeFromTarget = PosEyeFromTarget;
	}

private:
	Transform *m_pTargetPosAt;		//�����_�ƂȂ�I�u�W�F�N�g
	Transform *m_pTargetPosEye;		//�J�����ʒu�̎w�W
	Vector3 m_PosEyeFromTarget;		//�w�W����̃J�����ʒu
	Vector3 m_Rot;					//�J�����̉�]��
	float m_fLength;				//�J�����̋���

	Camera *m_pCamera;				//�J����
};


#endif

