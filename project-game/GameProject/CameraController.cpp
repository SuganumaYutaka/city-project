/*==============================================================================
	
	CameraController.h - カメラ操作
														Author : Yutaka Suganuma
														Date   : 2017/6/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "CameraController.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "TextureManager.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "DebugRenderer.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define MOVE_SPEED ( 0.05f)		//カメラ平行移動速度
#define ROT_SPEED ( 0.003f)			//カメラ回転速度

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* CameraController::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<CameraController>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
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
	終了処理
------------------------------------------------------------------------------*/
void CameraController::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void CameraController::Update()
{
	//長さ・角度の更新
	Vector3 Dir = m_pCamera->GetPosAt() - m_pTransform->GetWorldPosition();
	m_fLength = Dir.Length();

	//カメラ追従
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

	//注視点をターゲットに向ける
	if (m_pTargetPosAt != NULL)
	{
		//近すぎるときは注視点を動かさない
		if (m_pTargetPosEye != NULL &&
			Vector3::Distance(m_pTargetPosEye->m_pTransform->GetWorldPosition(), m_pTargetPosAt->m_pTransform->GetWorldPosition()) < 2.5f)
		{
			//動かさない
		}
		else
		{
			auto PosAt = m_pCamera->GetPosAt();
			Vector3 Move = (m_pTargetPosAt->GetWorldPosition() - PosAt) * 0.1f;
			m_pCamera->SetPosAt( PosAt + Move);
		}
	}

	//カメラ平行移動
	if( Manager::GetInputMouse()->GetClickPress(2))
	{
		if (m_pTargetPosAt == NULL)
		{
			Vector3 Move;
			//Move.y -= Manager::GetInputMouse()->GetAxisY() * MOVE_SPEED * m_fLength;
			//Move.x -= Manager::GetInputMouse()->GetAxisX() * MOVE_SPEED * m_fLength;
			Move.y -= Manager::GetInputMouse()->GetAxisY() * MOVE_SPEED;
			Move.x -= Manager::GetInputMouse()->GetAxisX() * MOVE_SPEED;

			Vector3 cameraVec = m_pCamera->GetCameraVec();
			Vector3 vecUp = m_pCamera->GetVecUp();
			cameraVec = Vector3::Cross( cameraVec, vecUp);
			m_pCamera->Move( cameraVec.Normalize() * Move.x);
			
			cameraVec = vecUp;
			m_pCamera->Move( cameraVec.Normalize() * Move.y);
		}
	}

	if( Manager::GetInputMouse()->GetClickPress(1))
	{
		//カメラ回転
		if (Manager::GetInputKeyboard()->GetKeyPress(DIK_LSHIFT) || Manager::GetInputKeyboard()->GetKeyPress(DIK_RSHIFT))
		{
			if (m_pTargetPosAt == NULL && m_pTargetPosEye == NULL)
			{	
				m_Rot.x -= Manager::GetInputMouse()->GetAxisY() * ROT_SPEED;
				m_Rot.y -= Manager::GetInputMouse()->GetAxisX() * ROT_SPEED;

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
		
		//注視点回転
		else
		{
			if (m_pTargetPosAt == NULL)
			{
				m_Rot.x -= Manager::GetInputMouse()->GetAxisY() * ROT_SPEED;
				m_Rot.y += Manager::GetInputMouse()->GetAxisX() * ROT_SPEED;

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
	}

	//ズームインアウト・Z軸移動
	float fZoom = Manager::GetInputMouse()->GetWheel() * MOVE_SPEED;

	if (fZoom != 0.0f)
	{
		Vector3 CameraVec = m_pCamera->GetCameraVec();

		//ズームインアウト
		if (Manager::GetInputKeyboard()->GetKeyPress(DIK_LSHIFT) || Manager::GetInputKeyboard()->GetKeyPress(DIK_RSHIFT))
		{
			m_pTransform->Move(  CameraVec * fZoom);
			if ((m_pCamera->GetPosAt() - m_pTransform->GetWorldPosition()).Length() < fZoom)
			{
				m_pTransform->Move( CameraVec * fZoom * -1);
			}
		}

		//Z軸移動
		else
		{
			
			m_pCamera->Move( CameraVec * fZoom);
		}
	}

	//回転
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
	衝突時の処理
------------------------------------------------------------------------------*/
void CameraController::OnCollision( Collider *pCollider)
{
	
}

