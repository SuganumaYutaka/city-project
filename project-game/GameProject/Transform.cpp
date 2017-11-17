/*==============================================================================

    Transform.cpp - �ʒu�E��]�E�X�P�[��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/27
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Transform.h"
#include "GameObject.h"

#include "DebugRenderer.h"

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* Transform::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<Transform>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
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
	�R���X�g���N�^
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
	�I������
------------------------------------------------------------------------------*/
void Transform::Uninit( void)
{

}

/*------------------------------------------------------------------------------
	���[���h���W�ϊ��s���n��
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
	���[�J���s��̍X�V
------------------------------------------------------------------------------*/
void Transform::UpdateLocalMatrix( void)
{
	D3DXMATRIX mtxWorld, mtxPos, mtxRot, mtxScl;

	//�s��̏������i�P�ʍs��̍쐬�j
	D3DXMatrixIdentity( &mtxWorld);
	D3DXMatrixIdentity( &mtxPos);
	D3DXMatrixIdentity( &mtxRot);
	D3DXMatrixIdentity( &mtxScl);

	//�g��s��̐ݒ�
	D3DXMatrixScaling( &mtxScl, m_Scale.x, m_Scale.y, m_Scale.z);

	//��]�s��̐ݒ�
	D3DXMatrixRotationQuaternion( &mtxRot, &m_Rotation);

	//���s�ړ��s��
	D3DXMatrixTranslation( &mtxPos, m_Position.x, m_Position.y, m_Position.z);

	//�s��̍���
	m_LocalMatrix = mtxWorld * mtxScl * mtxRot * mtxPos;		//�g�偨��]���ړ��̏�
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void Transform::Update()
{
	//���[�J���s��X�V
	UpdateLocalMatrix();
}

/*------------------------------------------------------------------------------
	���[���h�F�ʒu�擾
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
	���[���h�F�X�P�[���擾
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
	���[���h�F��]�擾
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
	���[���h�F���ʃx�N�g���擾
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
	���[���h�FXYZ���擾
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
	���[���h�F�ʒu�ݒ�
------------------------------------------------------------------------------*/
void Transform::SetWorldPosition(const Vector3& Pos)
{
	D3DXVECTOR3 pos = Pos.ConvertToDX();	//�ʒu
	
	//���[���h�����[�J��
	D3DXMATRIX mtxWorldInv;					//���[���h���W�ϊ��s��̋t�s��
	D3DXMatrixInverse( &mtxWorldInv, NULL, &m_pParent->WorldMatrix());
	D3DXVec3TransformCoord( &pos, &pos, &mtxWorldInv);

	//���[�J�����W�X�V
	m_Position = Vector3::ConvertFromDX( pos);

	m_bLocalMatrixUpdate = true;
}
void Transform::SetWorldPosition(const float x, const float y, const float z)
{
	D3DXVECTOR3 pos( x, y, z);			//�ʒu
	
	//���[���h�����[�J��
	D3DXMATRIX mtxWorldInv;				//���[���h���W�ϊ��s��̋t�s��
	D3DXMatrixInverse( &mtxWorldInv, NULL, &m_pParent->WorldMatrix());
	D3DXVec3TransformCoord( &pos, &pos, &mtxWorldInv);

	//���[�J�����W�X�V
	m_Position = Vector3::ConvertFromDX( pos);

	m_bLocalMatrixUpdate = true;
}
void Transform::SetWorldPositionX(const float x)
{
	Vector3 Posi = GetWorldPosition();			//�ʒu
	D3DXVECTOR3 pos( x, Posi.y, Posi.z);		//�ʒu
	
	//���[���h�����[�J��
	D3DXMATRIX mtxWorldInv;				//���[���h���W�ϊ��s��̋t�s��
	D3DXMatrixInverse( &mtxWorldInv, NULL, &m_pParent->WorldMatrix());
	D3DXVec3TransformCoord( &pos, &pos, &mtxWorldInv);

	//���[�J�����W�X�V
	m_Position = Vector3::ConvertFromDX( pos);

	m_bLocalMatrixUpdate = true;
}
void Transform::SetWorldPositionY(const float y)
{
	Vector3 Posi = GetWorldPosition();			//�ʒu
	D3DXVECTOR3 pos( Posi.x, y, Posi.z);			//�ʒu
	
	//���[���h�����[�J��
	D3DXMATRIX mtxWorldInv;				//���[���h���W�ϊ��s��̋t�s��
	D3DXMatrixInverse( &mtxWorldInv, NULL, &m_pParent->WorldMatrix());
	D3DXVec3TransformCoord( &pos, &pos, &mtxWorldInv);

	//���[�J�����W�X�V
	m_Position = Vector3::ConvertFromDX( pos);

	m_bLocalMatrixUpdate = true;
}
void Transform::SetWorldPositionZ(const float z)
{
	Vector3 Posi = GetWorldPosition();			//�ʒu
	D3DXVECTOR3 pos( Posi.x, Posi.y, z);		//�ʒu
	
	//���[���h�����[�J��
	D3DXMATRIX mtxWorldInv;				//���[���h���W�ϊ��s��̋t�s��
	D3DXMatrixInverse( &mtxWorldInv, NULL, &m_pParent->WorldMatrix());
	D3DXVec3TransformCoord( &pos, &pos, &mtxWorldInv);

	//���[�J�����W�X�V
	m_Position = Vector3::ConvertFromDX( pos);

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	�w������Ɉړ�
------------------------------------------------------------------------------*/
void Transform::Move(const Vector3& Value)
{
	D3DXVECTOR3 pos = GetWorldPosition().ConvertToDX();		//�ʒu
	
	//�ړ�
	pos += Value.ConvertToDX();

	//�e�����[�g�̏ꍇ
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		m_Position = Vector3::ConvertFromDX( pos);
	}

	//���[���h�����[�J��
	else
	{
		D3DXMATRIX mtxWorldInv;									//���[���h���W�ϊ��s��̋t�s��
		D3DXMatrixInverse( &mtxWorldInv, NULL, &m_pParent->WorldMatrix());
		D3DXVec3TransformCoord( &pos, &pos, &mtxWorldInv);

		//���[�J�����W�X�V
		m_Position = Vector3::ConvertFromDX( pos);
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	���[���h�F��]�ʐݒ�i�N�H�[�^�j�I���j
------------------------------------------------------------------------------*/
void Transform::SetWorldRotation(const D3DXQUATERNION& Rot)
{
	//���[���h�����[�J��
	D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//�e�̃��[���h��]�ʂ̋t��]
	D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);
	m_Rotation = Rot * WorldRotInv;

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	���[���h�F��]�ʐݒ�i�I�C���[�p�j
------------------------------------------------------------------------------*/
void Transform::SetWorldRotationEuler(const Vector3& Euler)
{
	D3DXQUATERNION Rot;				//��]��
	D3DXQUATERNION WorldRotInv;		//���[���h��]�ʂ̋t��]

	D3DXQuaternionIdentity( &Rot);
	D3DXQuaternionRotationYawPitchRoll( &Rot, Euler.y, Euler.x, Euler.z);

	//���[���h�����[�J��
	D3DXQuaternionInverse( &WorldRotInv, &GetWorldRotation());
	m_Rotation = Rot * WorldRotInv;

	m_bLocalMatrixUpdate = true;
}
void Transform::SetWorldRotationEuler(const float x, const float y, const float z)
{
	D3DXQUATERNION Rot;				//��]��
	
	D3DXQuaternionIdentity( &Rot);
	D3DXQuaternionRotationYawPitchRoll( &Rot, y, x, z);

	//���[���h�����[�J��
	D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//�e�̃��[���h��]�ʂ̋t��]
	D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);
	m_Rotation = Rot * WorldRotInv;

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	���[���h�F��]�ʐݒ�i�w�肵�����ŉ�]�j
------------------------------------------------------------------------------*/
void Transform::SetWorldRotationAxis( const Vector3& Axis, float Value)
{
	//�w�肵�����ŉ�]
	D3DXQUATERNION quat;
	D3DXQuaternionRotationAxis( &quat, &Axis.ConvertToDX(), Value);

	//���[���h�����[�J��
	D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//�e�̃��[���h��]�ʂ̋t��]
	D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);
	m_Rotation = quat * WorldRotInv;

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	���[���h�F��]�ʐݒ�i�w��ʒu�������悤�Ɂj
------------------------------------------------------------------------------*/
void Transform::SetWorldRotationLookAt(const Vector3& Position)
{
	if (Position == m_Position)
	{
		return;
	}

	Vector3 Direction = Position - GetWorldPosition();	//��������
	Vector3 VecZ = Vector3( 0.0f, 0.0f, 1.0f);			//��������

	//��̃x�N�g�����Ȃ��p�����߂�
	float X = Vector3::Dot( VecZ, Direction) / (VecZ.Length() * Direction.Length());
	if (X == 1.0f)
	{
		X -= 0.0001f;
	}
	float Angle = acos( X);			//�Ȃ��p

	//��]���̐ݒ�
	Vector3 cross = Vector3::Cross( VecZ, Direction);

	//�O�ς��[���x�N�g���i2�̃x�N�g�����꒼���j�̂Ƃ�VecUp����]���ɂ���
	if (cross.Length() <= 0.1f)
	{
		cross = Vector3( 0.0f, 1.0f, 0.0f);
	}

	//��]�ʂ̍X�V
	//�e�����[�g�̏ꍇ
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionIdentity( &m_Rotation);
		D3DXQuaternionRotationAxis( &m_Rotation, &cross.ConvertToDX(), Angle);
	}
	
	//����ȊO�̏ꍇ
	else
	{
		//��]���ă��[���h�����[�J��
		auto quatW = GetWorldRotation();	//���[���h��]��
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//�e�̃��[���h��]�ʂ̋t��]
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionIdentity( &quatW);
		D3DXQuaternionRotationAxis( &quatW, &cross.ConvertToDX(), Angle);
		m_Rotation = quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	���[���h�F��]�ʐݒ�i�w��ʒu�������悤�ɒn�ʂƐ����ɉ�]�j
------------------------------------------------------------------------------*/
void Transform::SetWorldRotationLookAtHorizontal(const Vector3& Position)
{
	if (Position == m_Position)
	{
		return;
	}

	Vector3 Direction = Position - GetWorldPosition();	//��������
	Direction.y = 0.0f;
	Vector3 VecZ = Vector3( 0.0f, 0.0f, 1.0f);			//��������

	//��̃x�N�g�����Ȃ��p�����߂�
	float X = Vector3::Dot( VecZ, Direction) / (VecZ.Length() * Direction.Length());
	if (X == 1.0f)
	{
		X -= 0.0001f;
	}
	float Angle = acos( X);			//�Ȃ��p

	//��]���̐ݒ�
	Vector3 cross = Vector3::Cross( VecZ, Direction);

	//�O�ς��[���x�N�g���i2�̃x�N�g�����꒼���j�̂Ƃ�VecUp����]���ɂ���
	if (cross.Length() <= 0.1f)
	{
		cross = Vector3( 0.0f, 1.0f, 0.0f);
	}

	//��]�ʂ̍X�V
	//�e�����[�g�̏ꍇ
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionIdentity( &m_Rotation);
		D3DXQuaternionRotationAxis( &m_Rotation, &cross.ConvertToDX(), Angle);
	}
	
	//����ȊO�̏ꍇ
	else
	{
		//��]���ă��[���h�����[�J��
		auto quatW = GetWorldRotation();	//���[���h��]��
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//�e�̃��[���h��]�ʂ̋t��]
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionIdentity( &quatW);
		D3DXQuaternionRotationAxis( &quatW, &cross.ConvertToDX(), Angle);
		m_Rotation = quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	���[���h�F��]�ʐݒ�i�w������������悤�Ɂj
------------------------------------------------------------------------------*/
void Transform::SetWorldRotationLookDirection(const Vector3& Direction)
{
	if (Direction.Length() == 0.0f)
	{
		return;
	}

	Vector3 VecZ = Vector3( 0.0f, 0.0f, 1.0f);		//��������

	//��̃x�N�g�����Ȃ��p�����߂�
	float X = Vector3::Dot( VecZ, Direction) / (VecZ.Length() * Direction.Length());
	if (X == 1.0f)
	{
		X -= 0.0001f;
	}
	float Angle = acos( X);			//�Ȃ��p

	//��]���̐ݒ�
	Vector3 cross = Vector3::Cross( VecZ, Direction);

	//�O�ς��[���x�N�g���i2�̃x�N�g�����꒼���j�̂Ƃ�VecUp����]���ɂ���
	if (cross.Length() <= 0.1f)
	{
		cross = Vector3( 0.0f, 1.0f, 0.0f);
	}

	//��]�ʂ̍X�V
	//�e�����[�g�̏ꍇ
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionIdentity( &m_Rotation);
		D3DXQuaternionRotationAxis( &m_Rotation, &cross.ConvertToDX(), Angle);
	}
	
	//����ȊO�̏ꍇ
	else
	{
		//��]���ă��[���h�����[�J��
		auto quatW = GetWorldRotation();	//���[���h��]��
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//�e�̃��[���h��]�ʂ̋t��]
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionIdentity( &quatW);
		D3DXQuaternionRotationAxis( &quatW, &cross.ConvertToDX(), Angle);
		m_Rotation = quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	���[�J���F��]�ʐݒ�i�I�C���[�p�j
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
	���[�J���F��]�ʐݒ�i�w�肵�����ŉ�]�j
------------------------------------------------------------------------------*/
void Transform::SetLocalRotationAxis( const Vector3& Axis, float Value)
{
	//�w�肵�����ŉ�]
	D3DXQuaternionRotationAxis( &m_Rotation, &Axis.ConvertToDX(), Value);

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	�X���[�v��]
------------------------------------------------------------------------------*/
void Transform::RotateSlerp( const D3DXQUATERNION& Quaternion, float Value)
{
	//�X���[�v��]
	//�e�����[�g�̏ꍇ
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionSlerp( &m_Rotation, &m_Rotation, &Quaternion, Value);
	}
	
	//����ȊO�̏ꍇ
	else
	{
		//��]���ă��[���h�����[�J��
		auto quatW = GetWorldRotation();								//���[���h��]��
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//�e�̃��[���h��]�ʂ̋t��]
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionSlerp( &quatW, &quatW, &Quaternion, Value);
		m_Rotation = quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	�w�肵�����ŉ�]
------------------------------------------------------------------------------*/
void Transform::RotateAxis( const Vector3& Axis, float Value)
{
	//�w�肵�����ŉ�]
	//�e�����[�g�̏ꍇ
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis( &quat, &Axis.ConvertToDX(), Value);
		m_Rotation *= quat;
	}
	
	//����ȊO�̏ꍇ
	else
	{
		//��]���ă��[���h�����[�J��
		auto quatW = GetWorldRotation();								//���[���h��]��
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//�e�̃��[���h��]�ʂ̋t��]
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionRotationAxis( &quatW, &Axis.ConvertToDX(), Value);
		m_Rotation *= quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	�w��ʒu�������悤�ɉ�]
------------------------------------------------------------------------------*/
void Transform::RotateLookAt(const Vector3& Position, float Value)
{
	if (Position == m_Position)
	{
		return;
	}

	Vector3 Vec = Position - GetWorldPosition();	//��������
	Vector3 VecZ = Vector3( 0.0f, 0.0f, 1.0f);		//��������

	//��̃x�N�g�����Ȃ��p�����߂�
	float X = Vector3::Dot( VecZ, Vec) / (VecZ.Length() * Vec.Length());
	if (X == 1.0f)
	{
		X -= 0.0001f;
	}
	float Angle = acos( X);				//�Ȃ��p

	//��]���̐ݒ�
	Vector3 cross = Vector3::Cross( VecZ, Vec);

	//�O�ς��[���x�N�g���i2�̃x�N�g�����꒼���j�̂Ƃ�VecUp����]���ɂ���
	if (cross.Length() <= 0.1f)
	{
		cross = Vector3( 0.0f, 1.0f, 0.0f);
	}

	//�N�H�[�^�j�I���̍쐬
	D3DXQUATERNION quat;							//�N�H�[�^�j�I��
	D3DXQuaternionIdentity( &quat);
	D3DXQuaternionRotationAxis( &quat, &cross.ConvertToDX(), Angle);
	
	//�쐬�����N�H�[�^�j�I���Ɍ����ĉ�]
	//�e�����[�g�̏ꍇ
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionSlerp( &m_Rotation, &m_Rotation, &quat, Value);
	}
	
	//����ȊO�̏ꍇ
	else
	{
		//��]���ă��[���h�����[�J��
		auto quatW = GetWorldRotation();								//���[���h��]��
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//�e�̃��[���h��]�ʂ̋t��]
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionSlerp( &quatW, &quatW, &quat, Value);
		m_Rotation = quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	�w��ʒu�������悤�ɒn�ʂƐ����ɉ�]
------------------------------------------------------------------------------*/
void Transform::RotateLookAtHorizontal(const Vector3& Position, float Value)
{
	if (Position == m_Position)
	{
		return;
	}

	Vector3 Vec = Position - GetWorldPosition();	//��������
	Vec.y = 0.0f;
	Vector3 VecZ = Vector3( 0.0f, 0.0f, 1.0f);		//��������

	//��̃x�N�g�����Ȃ��p�����߂�
	float X = Vector3::Dot( VecZ, Vec) / (VecZ.Length() * Vec.Length());
	if (X == 1.0f)
	{
		X -= 0.0001f;
	}
	float Angle = acos( X);				//�Ȃ��p

	//��]���̐ݒ�
	Vector3 cross = Vector3::Cross( VecZ, Vec);

	//�O�ς��[���x�N�g���i2�̃x�N�g�����꒼���j�̂Ƃ�VecUp����]���ɂ���
	if (cross.Length() <= 0.1f)
	{
		cross = Vector3( 0.0f, 1.0f, 0.0f);
	}

	//�N�H�[�^�j�I���̍쐬
	D3DXQUATERNION quat;							//�N�H�[�^�j�I��
	D3DXQuaternionIdentity( &quat);
	D3DXQuaternionRotationAxis( &quat, &cross.ConvertToDX(), Angle);
	
	//�쐬�����N�H�[�^�j�I���Ɍ����ĉ�]
	//�e�����[�g�̏ꍇ
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionSlerp( &m_Rotation, &m_Rotation, &quat, Value);
	}
	
	//����ȊO�̏ꍇ
	else
	{
		//��]���ă��[���h�����[�J��
		auto quatW = GetWorldRotation();								//���[���h��]��
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//�e�̃��[���h��]�ʂ̋t��]
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionSlerp( &quatW, &quatW, &quat, Value);
		m_Rotation = quatW * WorldRotInv;
	}

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	�w������������悤�ɉ�]
------------------------------------------------------------------------------*/
void Transform::RotateLookDirection(const Vector3& Direction, float Value)
{
	if (Direction.Length() == 0.0f)
	{
		return;
	}

	Vector3 VecZ = Vector3( 0.0f, 0.0f, 1.0f);		//��������

	//��̃x�N�g�����Ȃ��p�����߂�
	float X = Vector3::Dot( VecZ, Direction) / (VecZ.Length() * Direction.Length());
	if (X == 1.0f)
	{
		X -= 0.0001f;
	}
	float Angle = acos( X);			//�Ȃ��p

	//��]���̐ݒ�
	Vector3 cross = Vector3::Cross( VecZ, Direction);

	//�O�ς��[���x�N�g���i2�̃x�N�g�����꒼���j�̂Ƃ�VecUp����]���ɂ���
	if (cross.Length() <= 0.1f)
	{
		cross = Vector3( 0.0f, 1.0f, 0.0f);
	}

	//�N�H�[�^�j�I���̍쐬
	D3DXQUATERNION quat;							//�N�H�[�^�j�I��
	D3DXQuaternionIdentity( &quat);
	D3DXQuaternionRotationAxis( &quat, &cross.ConvertToDX(), Angle);

	//�쐬�����N�H�[�^�j�I���Ɍ����ĉ�]
	//�e�����[�g�̏ꍇ
	if (m_pTransform->GetParent() == m_pGameObject->GetRoot()->m_pTransform)
	{
		D3DXQuaternionSlerp( &m_Rotation, &m_Rotation, &quat, Value);
	}
	
	//����ȊO�̏ꍇ
	else
	{
		//��]���ă��[���h�����[�J��
		auto quatW = GetWorldRotation();								//���[���h��]��
		D3DXQUATERNION WorldRotInv = m_pParent->GetWorldRotation();		//�e�̃��[���h��]�ʂ̋t��]
		D3DXQuaternionInverse( &WorldRotInv, &WorldRotInv);

		D3DXQuaternionSlerp( &quatW, &quatW, &quat, Value);
		m_Rotation = quatW * WorldRotInv;
	}
	
	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	���[�J���F�X�P�[���ݒ�
------------------------------------------------------------------------------*/
void Transform::SetLocalScale( const Vector3& Scale)
{ 
	if (Scale.x == 0.0f || Scale.y == 0.0f || Scale.z == 0.0f)
	{
		//MessageBox( NULL, "�X�P�[���l��0.0f���ݒ肳��܂���\n", "�G���[", MB_OK);
		//assert(false);
	}
	m_Scale = Scale;
	m_bLocalMatrixUpdate = true;
}
void Transform::SetLocalScale( const float x, const float y, const float z)
{
	if (x == 0.0f || y == 0.0f || z == 0.0f)
	{
		//MessageBox( NULL, "�X�P�[���l��0.0f���ݒ肳��܂���\n", "�G���[", MB_OK);
		//assert(false);
	}
	m_Scale = Vector3( x, y, z); 
	m_bLocalMatrixUpdate = true;
}
void Transform::SetLocalScaleX( const float x)
{
	if (x == 0.0f)
	{
		//MessageBox( NULL, "�X�P�[���l��0.0f���ݒ肳��܂���\n", "�G���[", MB_OK);
		//assert(false);
	}
	m_Scale.x = x;  
	m_bLocalMatrixUpdate = true;
}
void Transform::SetLocalScaleY( const float y)
{
	if (y == 0.0f)
	{
		//MessageBox( NULL, "�X�P�[���l��0.0f���ݒ肳��܂���\n", "�G���[", MB_OK);
		//assert(false);
	}
	m_Scale.y = y;  
	m_bLocalMatrixUpdate = true;
}
void Transform::SetLocalScaleZ( const float z)
{
	if (z == 0.0f)
	{
		MessageBox( NULL, "�X�P�[���l��0.0f���ݒ肳��܂���\n", "�G���[", MB_OK);
		//assert(false);
	}
	m_Scale.z = z; 
	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	�eTransform��ݒ�
------------------------------------------------------------------------------*/
void Transform::SetParent(Transform* pParent)
{
	UpdateLocalMatrix();

	D3DXMATRIX mtxInv;
	D3DXMatrixIdentity( &mtxInv);
	D3DXMatrixInverse( &mtxInv, NULL, &pParent->WorldMatrix());
	m_LocalMatrix = m_LocalMatrix * m_pParent->WorldMatrix() * mtxInv;
	m_pParent = pParent;

	//�f�[�^�擾
	m_Position = Vector3( m_LocalMatrix._41, m_LocalMatrix._42, m_LocalMatrix._43);
	D3DXQuaternionRotationMatrix( &m_Rotation, &m_LocalMatrix);

	m_Scale.x = Vector3( m_LocalMatrix._11, m_LocalMatrix._12, m_LocalMatrix._13).Length();
	m_Scale.y = Vector3( m_LocalMatrix._21, m_LocalMatrix._22, m_LocalMatrix._23).Length();
	m_Scale.z = Vector3( m_LocalMatrix._31, m_LocalMatrix._32, m_LocalMatrix._33).Length();
	
	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	�eTransform����؂藣��
	�eObject��Transform��ݒ�
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

	//�f�[�^�擾
	m_Position = Vector3( m_LocalMatrix._41, m_LocalMatrix._42, m_LocalMatrix._43);
	D3DXQuaternionRotationMatrix( &m_Rotation, &m_LocalMatrix);
	
	m_Scale.x = Vector3( m_LocalMatrix._11, m_LocalMatrix._12, m_LocalMatrix._13).Length();
	m_Scale.y = Vector3( m_LocalMatrix._21, m_LocalMatrix._22, m_LocalMatrix._23).Length();
	m_Scale.z = Vector3( m_LocalMatrix._31, m_LocalMatrix._32, m_LocalMatrix._33).Length();

	m_bLocalMatrixUpdate = true;
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void Transform::Load(Text& text)
{
	//text��ǂݐi�߂�
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
	ImGui�̕\�����ݒ�
------------------------------------------------------------------------------*/
void Transform::SetImGuiView()
{
	ImGui::Text(typeid(*this).name());

	ImGui::DragFloat3("Position", &m_Position.x);
	ImGui::DragFloat4("Rotation", (float*)&m_Rotation);
	ImGui::DragFloat3("Scale", &m_Scale.x);
}