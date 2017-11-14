/*==============================================================================
	
	CameraController.h - �J��������
														Author : Yutaka Suganuma
														Date   : 2017/6/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "CameraController.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "TextureManager.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "DebugRenderer.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define MOVE_SPEED ( 0.10f)		//�J�������s�ړ����x
#define ROT_SPEED ( 0.03f)		//�J������]���x

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* CameraController::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<CameraController>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
CameraController::CameraController( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_pTargetPosAt = NULL;
	m_pTargetPosEye = NULL;

	m_pCamera = m_pGameObject->GetComponent<Camera>();

	Vector3 Dir = m_pCamera->GetPosAt() - m_pTransform->GetWorldPosition();
	m_Rot.x = atan2( Dir.y, Dir.z);
	m_Rot.y = atan2( Dir.x, -Dir.z);
	m_fLength = Dir.Length();

	m_PosEyeFromTarget = Vector3();
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void CameraController::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void CameraController::Update()
{
	//�����E�p�x�̍X�V
	Vector3 Dir = m_pCamera->GetPosAt() - m_pTransform->GetWorldPosition();
	m_fLength = Dir.Length();

	//�J�����Ǐ]
	if( m_pTargetPosEye != NULL)
	{
		auto PosEye = m_pTargetPosEye->GetWorldPosition();
		auto axis = m_pCamera->m_pTransform->GetAxis();
		PosEye += axis.X * m_PosEyeFromTarget.x;
		PosEye += axis.Y * m_PosEyeFromTarget.y;
		PosEye += axis.Z * m_PosEyeFromTarget.z;
		Vector3 Move = (PosEye - m_pCamera->m_pTransform->GetWorldPosition()) * 0.1f;
		m_pCamera->Move( Move);
	}

	//�����_���^�[�Q�b�g�Ɍ�����
	if (m_pTargetPosAt != NULL)
	{
		//�߂�����Ƃ��͒����_�𓮂����Ȃ�
		if (m_pTargetPosEye != NULL &&
			Vector3::Distance(m_pTargetPosEye->m_pTransform->GetWorldPosition(), m_pTargetPosAt->m_pTransform->GetWorldPosition()) < 2.5f)
		{
			//�������Ȃ�
		}
		else
		{
			auto PosAt = m_pCamera->GetPosAt();
			Vector3 Move = (m_pTargetPosAt->GetWorldPosition() - PosAt) * 0.1f;
			m_pCamera->SetPosAt( PosAt + Move);
		}
	}

	//�J�������s�ړ�
	if( Manager::GetInputMouse()->GetClickPress(2))
	{
		if (m_pTargetPosAt == NULL)
		{
			Vector3 Move;
			Move.y -= Manager::GetInputMouse()->GetAxisY() * MOVE_SPEED * m_fLength;
			Move.x -= Manager::GetInputMouse()->GetAxisX() * MOVE_SPEED * m_fLength;

			Vector3 CameraVec = m_pCamera->GetCameraVec();
			CameraVec = Vector3( -CameraVec.z, CameraVec.y, CameraVec.x);
			m_pCamera->Move( CameraVec * Move.x);
			
			CameraVec = m_pCamera->GetVecUp();
			m_pCamera->Move( CameraVec * Move.y);
		}
	}

	if( Manager::GetInputMouse()->GetClickPress(1))
	{
		//�����_��]
		if (Manager::GetInputKeyboard()->GetKeyPress(DIK_LSHIFT) || Manager::GetInputKeyboard()->GetKeyPress(DIK_RSHIFT))
		{
			if (m_pTargetPosAt == NULL)
			{
				m_Rot.x -= Manager::GetInputMouse()->GetAxisY() * 0.005f;
				m_Rot.y += Manager::GetInputMouse()->GetAxisX() * 0.005f;

				D3DXMATRIX mtxRotY, mtxRotX;
				D3DXMatrixIdentity( &mtxRotX);
				D3DXMatrixIdentity( &mtxRotY);
				D3DXMatrixRotationX( &mtxRotX, m_Rot.x);
				D3DXMatrixRotationY( &mtxRotY, m_Rot.y);
				mtxRotX *= mtxRotY;

				D3DXVECTOR3 PosAt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
				PosAt.z -= m_fLength;
				D3DXVec3TransformCoord( &PosAt, &PosAt, &mtxRotX);

				m_pCamera->SetPosAt( Vector3::ConvertFromDX( PosAt) + m_pTransform->GetWorldPosition());

				D3DXVECTOR3 VecUp( 0.0f, 1.0f, 0.0f);
				D3DXVec3TransformCoord( &VecUp, &VecUp, &mtxRotX);

				m_pCamera->SetVecUp( Vector3( VecUp.x, VecUp.y, VecUp.z));
			}
		}

		//�J������]
		else
		{
			if (m_pTargetPosAt == NULL && m_pTargetPosEye == NULL)
			{	
				m_Rot.x -= Manager::GetInputMouse()->GetAxisY() * 0.005f;
				m_Rot.y -= Manager::GetInputMouse()->GetAxisX() * 0.005f;

				D3DXMATRIX mtxRotY, mtxRotX;
				D3DXMatrixIdentity( &mtxRotX);
				D3DXMatrixIdentity( &mtxRotY);
				D3DXMatrixRotationX( &mtxRotX, m_Rot.x);
				D3DXMatrixRotationY( &mtxRotY, m_Rot.y);
				mtxRotX *= mtxRotY;
	
				D3DXVECTOR3 PosEye( 0.0f, 0.0f, 0.0f);
				PosEye.z += m_fLength;
				D3DXVec3TransformCoord( &PosEye, &PosEye, &mtxRotX);

				m_pTransform->SetWorldPosition( Vector3::ConvertFromDX( PosEye));
				m_pTransform->Move( m_pCamera->GetPosAt());

				D3DXVECTOR3 VecUp( 0.0f, 1.0f, 0.0f);
				D3DXVec3TransformCoord( &VecUp, &VecUp, &mtxRotX);

				m_pCamera->SetVecUp( Vector3( VecUp.x, VecUp.y, VecUp.z));
			}
		}
	}

	//�Y�[���C���A�E�g�EZ���ړ�
	float fZoom = Manager::GetInputMouse()->GetWheel() * MOVE_SPEED;

	if (fZoom != 0.0f)
	{
		Vector3 CameraVec = m_pCamera->GetCameraVec();

		//�Y�[���C���A�E�g
		if (Manager::GetInputKeyboard()->GetKeyPress(DIK_LSHIFT) || Manager::GetInputKeyboard()->GetKeyPress(DIK_RSHIFT))
		{
			m_pTransform->Move(  CameraVec * fZoom);
			if ((m_pCamera->GetPosAt() - m_pTransform->GetWorldPosition()).Length() < fZoom)
			{
				m_pTransform->Move( CameraVec * fZoom * -1);
			}
		}

		//Z���ړ�
		else
		{
			
			m_pCamera->Move( CameraVec * fZoom);
		}
	}

	//��]
	m_pTransform->SetWorldRotationLookAt( m_pCamera->GetPosAt());

//#ifdef _DEBUG
//	auto eye = m_pTransform->GetWorldPosition();
//	auto at = m_pCamera->GetPosAt();
//	DebugRenderer::AddText( "PosEye ");
//	DebugRenderer::AddText( std::to_string(eye.x));
//	DebugRenderer::AddText( ", ");
//	DebugRenderer::AddText( std::to_string(eye.y));
//	DebugRenderer::AddText( ", ");
//	DebugRenderer::AddText( std::to_string(eye.z));
//	DebugRenderer::AddText( "\n");
//	DebugRenderer::AddText( "PosAt ");
//	DebugRenderer::AddText( std::to_string(at.x));
//	DebugRenderer::AddText( ", ");
//	DebugRenderer::AddText( std::to_string(at.y));
//	DebugRenderer::AddText( ", ");
//	DebugRenderer::AddText( std::to_string(at.z));
//	DebugRenderer::AddText( "\n");
//	DebugRenderer::AddText( "Length ");
//	DebugRenderer::AddText( std::to_string(m_fLength));
//	DebugRenderer::AddText( "\n\n");
//#endif // _DEBUG
	
}

/*------------------------------------------------------------------------------
	�Փˎ��̏���
------------------------------------------------------------------------------*/
void CameraController::OnCollision( Collider *pCollider)
{
	
}

