/*==============================================================================
	
	CameraController.h - カメラ操作
														Author : Yutaka Suganuma
														Date   : 2017/6/7
==============================================================================*/
#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Camera;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class CameraController : public Component
{
public:
	static Component* Create( GameObject* gameObject);

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
	Transform *m_pTargetPosAt;		//注視点となるオブジェクト
	Transform *m_pTargetPosEye;		//カメラ位置の指標
	Vector3 m_PosEyeFromTarget;		//指標からのカメラ位置
	Vector3 m_Rot;					//カメラの回転量
	float m_fLength;				//カメラの距離

	Camera *m_pCamera;				//カメラ
};


#endif

