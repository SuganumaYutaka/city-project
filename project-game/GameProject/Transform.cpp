/*==============================================================================

    Transform.cpp - 位置・回転・スケール
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/27
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Transform.h"
#include "GameObject.h"

#include "DebugRenderer.h"

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* Transform::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<Transform>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Transform::Transform()
{
	m_pGameObject = NULL;
	m_pTransform = this;
	D3DXQuaternionIdentity( &m_Rotation);
	m_Scale = Vector3( 1.0f, 1.0f, 1.0f);
	m_pParent = NULL;
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Transform::Transform( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = this;
	D3DXQuaternionIdentity( &m_Rotation);
	m_Scale = Vector3( 1.0f, 1.0f, 1.0f);

	if( m_pGameObject->GetParent() != NULL)
	{
		m_pParent = m_pGameObject->GetParent()->m_pTransform;
	}
	else
	{
		m_pParent = NULL;
	}

	UpdateLocalMatrix();
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void Transform::Uninit( void)
{

}

/*------------------------------------------------------------------------------
	ワールド座標変換行列を渡す
------------------------------------------------------------------------------*/
D3DXMATRIX Transform::WorldMatrix()
{
	if (m_bLocalMatrixUpdate == true)
	{
		m_bLocalMatrixUpdate = false;

		UpdateLocalMatrix();
	}
	
	D3DXMATRIX mtxWorld = m_LocalMatrix;
	
	if( m_pParent != NULL)
	{
		mtxWorld = mtxWorld * m_pParent->WorldMatrix();
	}

	return mtxWorld;
}

/*------------------------------------------------------------------------------
	ローカル行列の更新
------------------------------------------------------------------------------*/
void Transform::UpdateLocalMatrix( void)
{
	D3DXMATRIX mtxWorld, mtxPos, mtxRot, mtxScl;

	//行列の初期化（単位行列の作成）
	D3DXMatrixIdentity( &mtxWorld);
	D3DXMatrixIdentity( &mtxPos);
	D3DXMatrixIdentity( &mtxRot);
	D3DXMatrixIdentity( &mtxScl);

	//拡大行列の設定
	D3DXMatrixScaling( &mtxScl, m_Scale.x, m_Scale.y, m_Scale.z);

	//回転行列の設定
	D3DXMatrixRotationQuaternion( &mtxRot, &m_Rotation);

	//平行移動行列
	D3DXMatrixTranslation( &mtxPos, m_Position.x, m_Position.y, m_Position.z);

	//行列の合成
	m_LocalMatrix = mtxWorld * mtxScl * mtxRot * mtxPos;		//拡大→回転→移動の順
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void Transform::Update()
{
	//ローカル行列更新
	UpdateLocalMatrix();
}

/*------------------------------------------------------------------------------
	ワールド：位置取得
------------------------------------------------------------------------------*/
Vector3 Transform::GetWorldPosition() const
{
	D3DXVECTOR3 Pos = m_Position.ConvertToDX();

	if( m_pParent != NULL)
	{
		D3DXVec3TransformCoord( &Pos, &Pos, &m_pParent->WorldMatrix());
	}

	return Vector3::ConvertFromDX(Pos);
}

/*------------------------------------------------------------------------------
	ワールド：スケール取得
------------------------------------------------------------------------------*/
Vector3 Transform::GetWorldScale() const
{
	D3DXVECTOR3 Scale = m_Scale.ConvertToDX();

	if( m_pParent != NULL)
	{
		D3DXMATRIX WorldMatrix = m_pParent->WorldMatrix();

		Scale.x *= Vector3( WorldMatrix._11, WorldMatrix._12, WorldMatrix._13).Length();
		Scale.y *= Vector3( WorldMatrix._21, WorldMatrix._22, WorldMatrix._23).Length();
		Scale.z *= Vector3( WorldMatrix._31, WorldMatrix._32, WorldMatrix._33).Length();
	}

	return Vector3::ConvertFromDX(Scale);
}

/*------------------------------------------------------------------------------
	ワールド：回転取得
------------------------------------------------------------------------------*/
D3DXQUATERNION Transform::GetWorldRotation() const
{
	D3DXQUATERNION Rot = m_Rotation;

	if( m_pParent != NULL)
	{
		Rot = Rot * m_pParent->GetWorldRotation();
	}

	return Rot;
}

/*------------------------------------------------------------------------------
	ワールド：正面ベクトル取得
------------------------------------------------------------------------------*/
Vector3 Transform::GetForward()
{
	D3DXVECTOR3 Forward( 0.0f, 0.0f, 1.0f);
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationQuaternion( &mtxRot, &GetWorldRotation());
	D3DXVec3TransformCoord( &Forward, &Forward, &mtxRot);
	D3DXVec3Normalize( &Forward, &Forward);

	return Vector3::ConvertFromDX( Forward);
}

/*------------------------------------------------------------------------------
	ワールド：XYZ軸取得
------------------------------------------------------------------------------*/
Axis Transform::GetAxis()
{
	Axis axis;
	D3DXVECTOR3 VecUp( 0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 axisX, axisY, axisZ;
	axisZ = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);
	D3DXMATRIX mtxRot;
	
	D3DXMatrixRotationQuaternion( &mtxRot, &GetWorldRotation());
	D3DXVec3TransformCoord( &axisZ, &axisZ, &mtxRot);
	D3DXVec3Cross( &axisX, &axisZ, &VecUp);
	D3DXVec3Cross( &axisY, &axisX, &axisZ);
	
	D3DXVec3Normalize( &axisX, &axisX);
	D3DXVec3Normalize( &axisY, &axisY);
	D3DXVec3Normalize( &axisZ, &axisZ);
	
	axis.X = Vector3::ConvertFromDX( axisX);
	axis.Y = Vector3::ConvertFromDX( axisY);
	axis.Z = Vector3::ConvertFromDX( axisZ);

	return axis;
}

/*------------------------------------------------------------------------------
	ワールド：位置設定
------------------------------------------------------------------------------*/
void Transform::SetWorldPosition(const Vector3& Pos)
{
	D3DXVECTOR3 pos = Pos.ConvertToDX();	//位置
	
	//ワールド→ローカル
	D3DXMATRIX mtxWorldInv;					//ワールド座標変換行列の逆行列
	D3DXMatrixInverse( &mtxWorldInv, NULL, &m_pParent->WorldMatrix());
	D3DXVec3TransformCoord( &pos, &pos, &mtxWorldInv);

	//ローカル座標更新
	m_Position = Vector3::ConvertFromDX( pos);

	m_bLocalMatrixUpdate = true;
}
void Transform::SetWorldPosition(const float x, const float y, const float z)
{
	D3DXVECTOR3 pos( x, y, z);			//位置
	
	//ワールド→ローカル
	D3DXMATRIX mtxWorldInv;				//ワールド座標変換行列の逆行列
	D3DXMatrixInverse( &mtxWorldInv, NULL, &m_pParent->WorldMatrix());
	D3DXVec3TransformCoord( &pos, &pos, &mtxWorldInv);

	//ローカル座標更新
	m_Position = Vector3::ConvertFromDX( pos);

	m_bLocalMatrixUpdate = true;
}
void Transform::SetWorldPositionX(const float x)
{
	Vector3 Posi = GetWorldPosition();			//位置
	D3DXVECTOR3 pos( x, Posi.y, Posi.z);		//位置
	
	//ワールド→ローカル
	D3DXMATRIX mtxWorldInv;				//ワールド座標変換行列の逆行列
	D3DXMatrixInverse( &mtxWorldInv, NULL, &m_pParent->WorldMatrix());
	D3DXVec3TransformCoord( &pos, &pos, &mtxWorldInv);

	//ローカル座標更新
	m_Position = Vector3::ConvertFromDX( pos);

	m_bLocalMatrixUpdate = true;
}
void Transform::SetWorldPositionY(const float y)
{
	Vector3 Posi = GetWorldPosition();			//位置
	D3DXVECTOR3 pos( Posi.x, y, Posi.z);			//位置
	
	//ワールド→ローカル
	D3DXMATRIX mtxWorldInv;				//ワールド座標変換行列の逆行列
	D3DXMatrixInverse( &mtxWorldInv, NULL, &m_pParent->WorldMatrix());
	D3DXVec3TransformCoord( &pos, &pos, &mtxWorldInv);

	//ローカル座標更新
	m_Position = Vector3::ConvertFromDX( pos);

	m_bLocalMatrixUpdate = true;
}
void Transform::SetWorldPositionZ(const float z)
{
	Vector3 Posi = GetWorldPosition();			//位置
	D3DXVECTOR3 pos( Posi.x, Posi.y, z);		//位置
	
	//ワールド→ローカル
	D3DXMATRIX mtxWorldInv;				//ワールド座標変換行列の逆行列
	D3DXMatrixInverse( &mtxWorldInv, NULL, &m_pParent->WorldMatrix());
	D3DXVec3TransformCoord( &pos, &pos, &mtxWorldInv);

	//ローカル座標更新
	m_Position = Vector3::ConvertFromDX( pos);

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	指定方向に移動
------------------------------------------------------------------------------*/
void Transform::Move(const Vector3& Value)
{
	D3DXVECTOR3 pos = GetWorldPosition().ConvertToDX();		//位置
	
	//移動
	pos += Value.ConvertToDX();

	//親がルートの場合
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		m_Position = Vector3::ConvertFromDX( pos);
	}

	//ワールド→ローカル
	else
	{
		D3DXMATRIX mtxWorldInv;									//ワールド座標変換行列の逆行列
		D3DXMatrixInverse( &mtxWorldInv, NULL, &m_pParent->WorldMatrix());
		D3DXVec3TransformCoord( &pos, &pos, &mtxWorldInv);

		//ローカル座標更新
		m_Position = Vector3::ConvertFromDX( pos);
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	ワールド：回転量設定（クォータニオン）
------------------------------------------------------------------------------*/
void Transform::SetWorldRotation(const D3DXQUATERNION& Rot)
{
	//ワールド→ローカル
	D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//親のワールド回転量の逆回転
	D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);
	m_Rotation = Rot * WorldRotInv;

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	ワールド：回転量設定（オイラー角）
------------------------------------------------------------------------------*/
void Transform::SetWorldRotationEuler(const Vector3& Euler)
{
	D3DXQUATERNION Rot;				//回転量
	D3DXQUATERNION WorldRotInv;		//ワールド回転量の逆回転

	D3DXQuaternionIdentity( &Rot);
	D3DXQuaternionRotationYawPitchRoll( &Rot, Euler.y, Euler.x, Euler.z);

	//ワールド→ローカル
	D3DXQuaternionInverse( &WorldRotInv, &GetWorldRotation());
	m_Rotation = Rot * WorldRotInv;

	m_bLocalMatrixUpdate = true;
}
void Transform::SetWorldRotationEuler(const float x, const float y, const float z)
{
	D3DXQUATERNION Rot;				//回転量
	
	D3DXQuaternionIdentity( &Rot);
	D3DXQuaternionRotationYawPitchRoll( &Rot, y, x, z);

	//ワールド→ローカル
	D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//親のワールド回転量の逆回転
	D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);
	m_Rotation = Rot * WorldRotInv;

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	ワールド：回転量設定（指定した軸で回転）
------------------------------------------------------------------------------*/
void Transform::SetWorldRotationAxis( const Vector3& Axis, float Value)
{
	//指定した軸で回転
	D3DXQUATERNION quat;
	D3DXQuaternionRotationAxis( &quat, &Axis.ConvertToDX(), Value);

	//ワールド→ローカル
	D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//親のワールド回転量の逆回転
	D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);
	m_Rotation = quat * WorldRotInv;

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	ワールド：回転量設定（指定位置を向くように）
------------------------------------------------------------------------------*/
void Transform::SetWorldRotationLookAt(const Vector3& Position)
{
	if (Position == m_Position)
	{
		return;
	}

	Vector3 Direction = Position - GetWorldPosition();	//向く方向
	Vector3 VecZ = Vector3( 0.0f, 0.0f, 1.0f);			//初期方向

	//二つのベクトルがなす角を求める
	float X = Vector3::Dot( VecZ, Direction) / (VecZ.Length() * Direction.Length());
	if (X == 1.0f)
	{
		X -= 0.0001f;
	}
	float Angle = acos( X);			//なす角

	//回転軸の設定
	Vector3 cross = Vector3::Cross( VecZ, Direction);

	//外積がゼロベクトル（2つのベクトルが一直線）のときVecUpを回転軸にする
	if (cross.Length() <= 0.1f)
	{
		cross = Vector3( 0.0f, 1.0f, 0.0f);
	}

	//回転量の更新
	//親がルートの場合
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionIdentity( &m_Rotation);
		D3DXQuaternionRotationAxis( &m_Rotation, &cross.ConvertToDX(), Angle);
	}
	
	//それ以外の場合
	else
	{
		//回転してワールド→ローカル
		auto quatW = GetWorldRotation();	//ワールド回転量
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//親のワールド回転量の逆回転
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionIdentity( &quatW);
		D3DXQuaternionRotationAxis( &quatW, &cross.ConvertToDX(), Angle);
		m_Rotation = quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	ワールド：回転量設定（指定位置を向くように地面と水平に回転）
------------------------------------------------------------------------------*/
void Transform::SetWorldRotationLookAtHorizontal(const Vector3& Position)
{
	if (Position == m_Position)
	{
		return;
	}

	Vector3 Direction = Position - GetWorldPosition();	//向く方向
	Direction.y = 0.0f;
	Vector3 VecZ = Vector3( 0.0f, 0.0f, 1.0f);			//初期方向

	//二つのベクトルがなす角を求める
	float X = Vector3::Dot( VecZ, Direction) / (VecZ.Length() * Direction.Length());
	if (X == 1.0f)
	{
		X -= 0.0001f;
	}
	float Angle = acos( X);			//なす角

	//回転軸の設定
	Vector3 cross = Vector3::Cross( VecZ, Direction);

	//外積がゼロベクトル（2つのベクトルが一直線）のときVecUpを回転軸にする
	if (cross.Length() <= 0.1f)
	{
		cross = Vector3( 0.0f, 1.0f, 0.0f);
	}

	//回転量の更新
	//親がルートの場合
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionIdentity( &m_Rotation);
		D3DXQuaternionRotationAxis( &m_Rotation, &cross.ConvertToDX(), Angle);
	}
	
	//それ以外の場合
	else
	{
		//回転してワールド→ローカル
		auto quatW = GetWorldRotation();	//ワールド回転量
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//親のワールド回転量の逆回転
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionIdentity( &quatW);
		D3DXQuaternionRotationAxis( &quatW, &cross.ConvertToDX(), Angle);
		m_Rotation = quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	ワールド：回転量設定（指定方向を向くように）
------------------------------------------------------------------------------*/
void Transform::SetWorldRotationLookDirection(const Vector3& Direction)
{
	if (Direction.Length() == 0.0f)
	{
		return;
	}

	Vector3 VecZ = Vector3( 0.0f, 0.0f, 1.0f);		//初期方向

	//二つのベクトルがなす角を求める
	float X = Vector3::Dot( VecZ, Direction) / (VecZ.Length() * Direction.Length());
	if (X == 1.0f)
	{
		X -= 0.0001f;
	}
	float Angle = acos( X);			//なす角

	//回転軸の設定
	Vector3 cross = Vector3::Cross( VecZ, Direction);

	//外積がゼロベクトル（2つのベクトルが一直線）のときVecUpを回転軸にする
	if (cross.Length() <= 0.1f)
	{
		cross = Vector3( 0.0f, 1.0f, 0.0f);
	}

	//回転量の更新
	//親がルートの場合
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionIdentity( &m_Rotation);
		D3DXQuaternionRotationAxis( &m_Rotation, &cross.ConvertToDX(), Angle);
	}
	
	//それ以外の場合
	else
	{
		//回転してワールド→ローカル
		auto quatW = GetWorldRotation();	//ワールド回転量
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//親のワールド回転量の逆回転
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionIdentity( &quatW);
		D3DXQuaternionRotationAxis( &quatW, &cross.ConvertToDX(), Angle);
		m_Rotation = quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	ローカル：回転量設定（オイラー角）
------------------------------------------------------------------------------*/
void Transform::SetLocalRotationEuler( const Vector3& Euler)
{ 
	D3DXQuaternionIdentity( &m_Rotation);
	D3DXQuaternionRotationYawPitchRoll( &m_Rotation, Euler.y, Euler.x, Euler.z);
	m_bLocalMatrixUpdate = true;
}
void Transform::SetLocalRotationEuler( const float x, const float y, const float z)
{
	D3DXQuaternionIdentity( &m_Rotation);
	D3DXQuaternionRotationYawPitchRoll( &m_Rotation, y, x, z);
	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	ローカル：回転量設定（指定した軸で回転）
------------------------------------------------------------------------------*/
void Transform::SetLocalRotationAxis( const Vector3& Axis, float Value)
{
	//指定した軸で回転
	D3DXQuaternionRotationAxis( &m_Rotation, &Axis.ConvertToDX(), Value);

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	スラープ回転
------------------------------------------------------------------------------*/
void Transform::RotateSlerp( const D3DXQUATERNION& Quaternion, float Value)
{
	//スラープ回転
	//親がルートの場合
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionSlerp( &m_Rotation, &m_Rotation, &Quaternion, Value);
	}
	
	//それ以外の場合
	else
	{
		//回転してワールド→ローカル
		auto quatW = GetWorldRotation();								//ワールド回転量
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//親のワールド回転量の逆回転
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionSlerp( &quatW, &quatW, &Quaternion, Value);
		m_Rotation = quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	指定した軸で回転
------------------------------------------------------------------------------*/
void Transform::RotateAxis( const Vector3& Axis, float Value)
{
	//指定した軸で回転
	//親がルートの場合
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis( &quat, &Axis.ConvertToDX(), Value);
		m_Rotation *= quat;
	}
	
	//それ以外の場合
	else
	{
		//回転してワールド→ローカル
		auto quatW = GetWorldRotation();								//ワールド回転量
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//親のワールド回転量の逆回転
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionRotationAxis( &quatW, &Axis.ConvertToDX(), Value);
		m_Rotation *= quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	指定位置を向くように回転
------------------------------------------------------------------------------*/
void Transform::RotateLookAt(const Vector3& Position, float Value)
{
	if (Position == m_Position)
	{
		return;
	}

	Vector3 Vec = Position - GetWorldPosition();	//向く方向
	Vector3 VecZ = Vector3( 0.0f, 0.0f, 1.0f);		//初期方向

	//二つのベクトルがなす角を求める
	float X = Vector3::Dot( VecZ, Vec) / (VecZ.Length() * Vec.Length());
	if (X == 1.0f)
	{
		X -= 0.0001f;
	}
	float Angle = acos( X);				//なす角

	//回転軸の設定
	Vector3 cross = Vector3::Cross( VecZ, Vec);

	//外積がゼロベクトル（2つのベクトルが一直線）のときVecUpを回転軸にする
	if (cross.Length() <= 0.1f)
	{
		cross = Vector3( 0.0f, 1.0f, 0.0f);
	}

	//クォータニオンの作成
	D3DXQUATERNION quat;							//クォータニオン
	D3DXQuaternionIdentity( &quat);
	D3DXQuaternionRotationAxis( &quat, &cross.ConvertToDX(), Angle);
	
	//作成したクォータニオンに向けて回転
	//親がルートの場合
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionSlerp( &m_Rotation, &m_Rotation, &quat, Value);
	}
	
	//それ以外の場合
	else
	{
		//回転してワールド→ローカル
		auto quatW = GetWorldRotation();								//ワールド回転量
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//親のワールド回転量の逆回転
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionSlerp( &quatW, &quatW, &quat, Value);
		m_Rotation = quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	指定位置を向くように地面と水平に回転
------------------------------------------------------------------------------*/
void Transform::RotateLookAtHorizontal(const Vector3& Position, float Value)
{
	if (Position == m_Position)
	{
		return;
	}

	Vector3 Vec = Position - GetWorldPosition();	//向く方向
	Vec.y = 0.0f;
	Vector3 VecZ = Vector3( 0.0f, 0.0f, 1.0f);		//初期方向

	//二つのベクトルがなす角を求める
	float X = Vector3::Dot( VecZ, Vec) / (VecZ.Length() * Vec.Length());
	if (X == 1.0f)
	{
		X -= 0.0001f;
	}
	float Angle = acos( X);				//なす角

	//回転軸の設定
	Vector3 cross = Vector3::Cross( VecZ, Vec);

	//外積がゼロベクトル（2つのベクトルが一直線）のときVecUpを回転軸にする
	if (cross.Length() <= 0.1f)
	{
		cross = Vector3( 0.0f, 1.0f, 0.0f);
	}

	//クォータニオンの作成
	D3DXQUATERNION quat;							//クォータニオン
	D3DXQuaternionIdentity( &quat);
	D3DXQuaternionRotationAxis( &quat, &cross.ConvertToDX(), Angle);
	
	//作成したクォータニオンに向けて回転
	//親がルートの場合
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionSlerp( &m_Rotation, &m_Rotation, &quat, Value);
	}
	
	//それ以外の場合
	else
	{
		//回転してワールド→ローカル
		auto quatW = GetWorldRotation();								//ワールド回転量
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//親のワールド回転量の逆回転
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionSlerp( &quatW, &quatW, &quat, Value);
		m_Rotation = quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	指定方向を向くように回転
------------------------------------------------------------------------------*/
void Transform::RotateLookDirection(const Vector3& Direction, float Value)
{
	if (Direction.Length() == 0.0f)
	{
		return;
	}

	Vector3 VecZ = Vector3( 0.0f, 0.0f, 1.0f);		//初期方向

	//二つのベクトルがなす角を求める
	float X = Vector3::Dot( VecZ, Direction) / (VecZ.Length() * Direction.Length());
	if (X == 1.0f)
	{
		X -= 0.0001f;
	}
	float Angle = acos( X);			//なす角

	//回転軸の設定
	Vector3 cross = Vector3::Cross( VecZ, Direction);

	//外積がゼロベクトル（2つのベクトルが一直線）のときVecUpを回転軸にする
	if (cross.Length() <= 0.1f)
	{
		cross = Vector3( 0.0f, 1.0f, 0.0f);
	}

	//クォータニオンの作成
	D3DXQUATERNION quat;							//クォータニオン
	D3DXQuaternionIdentity( &quat);
	D3DXQuaternionRotationAxis( &quat, &cross.ConvertToDX(), Angle);

	//作成したクォータニオンに向けて回転
	//親がルートの場合
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionSlerp( &m_Rotation, &m_Rotation, &quat, Value);
	}
	
	//それ以外の場合
	else
	{
		//回転してワールド→ローカル
		auto quatW = GetWorldRotation();								//ワールド回転量
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//親のワールド回転量の逆回転
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionSlerp( &quatW, &quatW, &quat, Value);
		m_Rotation = quatW * WorldRotInv;
	}
	
	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	ローカル：スケール設定
------------------------------------------------------------------------------*/
void Transform::SetLocalScale( const Vector3& Scale)
{ 
	if (Scale.x == 0.0f || Scale.y == 0.0f || Scale.z == 0.0f)
	{
		//MessageBox( NULL, "スケール値に0.0fが設定されました\n", "エラー", MB_OK);
		//assert(false);
	}
	m_Scale = Scale;
	m_bLocalMatrixUpdate = true;
}
void Transform::SetLocalScale( const float x, const float y, const float z)
{
	if (x == 0.0f || y == 0.0f || z == 0.0f)
	{
		//MessageBox( NULL, "スケール値に0.0fが設定されました\n", "エラー", MB_OK);
		//assert(false);
	}
	m_Scale = Vector3( x, y, z); 
	m_bLocalMatrixUpdate = true;
}
void Transform::SetLocalScaleX( const float x)
{
	if (x == 0.0f)
	{
		//MessageBox( NULL, "スケール値に0.0fが設定されました\n", "エラー", MB_OK);
		//assert(false);
	}
	m_Scale.x = x;  
	m_bLocalMatrixUpdate = true;
}
void Transform::SetLocalScaleY( const float y)
{
	if (y == 0.0f)
	{
		//MessageBox( NULL, "スケール値に0.0fが設定されました\n", "エラー", MB_OK);
		//assert(false);
	}
	m_Scale.y = y;  
	m_bLocalMatrixUpdate = true;
}
void Transform::SetLocalScaleZ( const float z)
{
	if (z == 0.0f)
	{
		MessageBox( NULL, "スケール値に0.0fが設定されました\n", "エラー", MB_OK);
		//assert(false);
	}
	m_Scale.z = z; 
	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	親Transformを設定
------------------------------------------------------------------------------*/
void Transform::SetParent(Transform* pParent)
{
	UpdateLocalMatrix();

	D3DXMATRIX mtxInv;
	D3DXMatrixIdentity( &mtxInv);
	D3DXMatrixInverse( &mtxInv, NULL, &pParent->WorldMatrix());
	m_LocalMatrix = m_LocalMatrix * m_pParent->WorldMatrix() * mtxInv;
	m_pParent = pParent;

	//データ取得
	m_Position = Vector3( m_LocalMatrix._41, m_LocalMatrix._42, m_LocalMatrix._43);
	D3DXQuaternionRotationMatrix( &m_Rotation, &m_LocalMatrix);

	m_Scale.x = Vector3( m_LocalMatrix._11, m_LocalMatrix._12, m_LocalMatrix._13).Length();
	m_Scale.y = Vector3( m_LocalMatrix._21, m_LocalMatrix._22, m_LocalMatrix._23).Length();
	m_Scale.z = Vector3( m_LocalMatrix._31, m_LocalMatrix._32, m_LocalMatrix._33).Length();
	
	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	親Transformから切り離す
	親ObjectのTransformを設定
------------------------------------------------------------------------------*/
void Transform::UnParent( void)
{
	if (m_pParent == NULL)
	{
		return;
	}

	UpdateLocalMatrix();

	Transform* pParent = m_pGameObject->GetParent()->m_pTransform;

	D3DXMATRIX mtxInv;
	D3DXMatrixInverse( &mtxInv, NULL, &pParent->WorldMatrix());
	m_LocalMatrix = m_LocalMatrix * m_pParent->WorldMatrix() * mtxInv;
	m_pParent = pParent;

	//データ取得
	m_Position = Vector3( m_LocalMatrix._41, m_LocalMatrix._42, m_LocalMatrix._43);
	D3DXQuaternionRotationMatrix( &m_Rotation, &m_LocalMatrix);
	
	m_Scale.x = Vector3( m_LocalMatrix._11, m_LocalMatrix._12, m_LocalMatrix._13).Length();
	m_Scale.y = Vector3( m_LocalMatrix._21, m_LocalMatrix._22, m_LocalMatrix._23).Length();
	m_Scale.z = Vector3( m_LocalMatrix._31, m_LocalMatrix._32, m_LocalMatrix._33).Length();

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void Transform::Load(Text& text)
{
	//textを読み進める
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "Position")
		{
			text.ForwardPositionToNextWord();
			
			text.SetPosition( m_Position.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}

		else if (text.GetWord() == "Scale")
		{
			text.ForwardPositionToNextWord();
			
			text.SetPosition( m_Scale.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}

		else if (text.GetWord() == "Rotation")
		{
			text.ForwardPositionToNextWord();
			
			m_Rotation.x = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Rotation.y = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Rotation.z = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Rotation.w = std::stof(text.GetWord());
		}

		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	セーブ
------------------------------------------------------------------------------*/
void Transform::Save(Text& text)
{
	StartSave(text);

	text += "Position " + m_Position.ConvertToString() + '\n';
	text += "Scale " + m_Scale.ConvertToString() + '\n';
	text += "Rotation " 
		+ std::to_string(m_Rotation.x) + ' '
		+ std::to_string(m_Rotation.y) + ' '
		+ std::to_string(m_Rotation.z) + ' '
		+ std::to_string(m_Rotation.w) + '\n';

	EndSave( text);
}

/*------------------------------------------------------------------------------
	ImGuiの表示情報設定
------------------------------------------------------------------------------*/
void Transform::SetImGuiView()
{
	ImGui::Text(typeid(*this).name());

	ImGui::DragFloat3("Position", &m_Position.x);
	ImGui::DragFloat4("Rotation", (float*)&m_Rotation);
	ImGui::DragFloat3("Scale", &m_Scale.x);
}