/*==============================================================================

    Transform.h - �ʒu�E��]�E�X�P�[��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/27
==============================================================================*/
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define GRAVITY_ ( 0.012f)

/*------------------------------------------------------------------------------
	�񋓌^��`
------------------------------------------------------------------------------*/
typedef struct
{
	Vector3 X;
	Vector3 Y;
	Vector3 Z;
}Axis;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Transform : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	Transform( GameObject *pGameObject);
	void Uninit( void);

	D3DXMATRIX WorldMatrix();
	
	//�f�[�^���擾
	Vector3 GetLocalPosition() const { return m_Position;}
	Vector3 GetLocalScale() const { return m_Scale;}
	D3DXQUATERNION GetLocalRotation() const { return m_Rotation;}
	Vector3 GetWorldPosition() const;
	Vector3 GetWorldScale() const;
	D3DXQUATERNION GetWorldRotation() const;
	Vector3 GetForward();
	Axis GetAxis();

	//�f�[�^��ݒ�

	//�ʒu
	void SetWorldPosition( const Vector3& Pos);
	void SetWorldPosition( const float x, const float y, const float z);
	void SetWorldPositionX( const float x);
	void SetWorldPositionY( const float y);
	void SetWorldPositionZ( const float z);

	void SetLocalPosition( const Vector3& Pos){ m_Position = Pos; m_bLocalMatrixUpdate = true;}
	void SetLocalPosition( const float x, const float y, const float z){ m_Position = Vector3( x, y, z); m_bLocalMatrixUpdate = true;}
	void SetLocalPositionX( const float x){ m_Position.x = x; m_bLocalMatrixUpdate = true;}
	void SetLocalPositionY( const float y){ m_Position.y = y; m_bLocalMatrixUpdate = true;}
	void SetLocalPositionZ( const float z){ m_Position.z = z; m_bLocalMatrixUpdate = true;}

	void Move( const Vector3& Value);

	//��]
	void SetWorldRotation( const D3DXQUATERNION& Rot);
	void SetWorldRotationEuler( const Vector3& Euler);
	void SetWorldRotationEuler( const float x, const float y, const float z);
	void SetWorldRotationAxis( const Vector3& Axis, float Value);
	void SetWorldRotationLookAt(const Vector3& Position);
	void SetWorldRotationLookAtHorizontal(const Vector3& Position);
	void SetWorldRotationLookDirection(const Vector3& Direction);

	void SetLocalRotation( const D3DXQUATERNION& Rot){ m_Rotation = Rot;  m_bLocalMatrixUpdate = true;}
	void SetLocalRotationEuler( const Vector3& Euler);
	void SetLocalRotationEuler( const float x, const float y, const float z);
	void SetLocalRotationAxis( const Vector3& Axis, float Value);

	void RotateMultiply( const D3DXQUATERNION& Quaternion){ m_Rotation *= Quaternion; m_bLocalMatrixUpdate = true;}
	void RotateSlerp( const D3DXQUATERNION& Quaternion, float Value);
	void RotateAxis( const Vector3& Axis, float Value);
	void RotateLookAt(const Vector3& Position, float Value);
	void RotateLookAtHorizontal(const Vector3& Position, float Value);
	void RotateLookDirection(const Vector3& Direction, float Value);

	//�X�P�[��
	void SetLocalScale( const Vector3& Scale);
	void SetLocalScale( const float x, const float y, const float z);
	void SetLocalScaleX( const float x);
	void SetLocalScaleY( const float y);
	void SetLocalScaleZ( const float z);

	void ScaleUpDown( const Vector3& Value) { m_Scale += Value;  m_bLocalMatrixUpdate = true;}
	void ScaleUpDown( const float& Value) { m_Scale.x += Value; m_Scale.y += Value; m_Scale.z += Value;  m_bLocalMatrixUpdate = true;}
	
	//�e����ݒ�E�擾
	void SetParent( Transform* pParent);
	void UnParent( void);
	Transform* GetParent( void) const { return m_pParent;}

	virtual void Save( Text& text);
	virtual void Load( Text& text);

	virtual void SetImGuiView( void);

private:
	Transform();
	void Update( void);
	void UpdateLocalMatrix( void);

	D3DXMATRIX m_LocalMatrix;			//���[�J���s��
	bool m_bLocalMatrixUpdate;			//���[�J���s��X�V�t���O

	Transform *m_pParent;				//�e��Transform�|�C���^

	Vector3 m_Position;					//�ʒu
	Vector3 m_Scale;					//�X�P�[��
	D3DXQUATERNION m_Rotation;			//��]�ʁi�N�H�[�^�j�I���j
};

#endif

