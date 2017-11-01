/*==============================================================================

    Vector3.h - 3D�x�N�g��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/3/30
==============================================================================*/
#ifndef _VECTOR3_H_
#define _VECTOR3_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include <d3dx9.h>

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Vector3
{
public:
	float x;	//x���W
	float y;	//y���W
	float z;	//z���W

	Vector3() : x( 0.0f), y( 0.0f), z( 0.0f) {}
	Vector3( float X, float Y, float Z) : x(X), y(Y), z(Z) {}
	Vector3( D3DXVECTOR3 Vec) : x(Vec.x), y(Vec.y), z(Vec.z) {}
	
	float Length( void) const;												//����
	static float Length( const Vector3 &V1);								//����
	Vector3 Normalize( void) const;											//���K��
	static Vector3 Normalize( const Vector3 &V1);							//���K��
	float Dot( const Vector3 &V1) const;									//����
	static float Dot( const Vector3 &V1, const Vector3 &V2);				//����
	Vector3 Cross( const Vector3 &V1) const;								//�O��
	static Vector3 Cross( const Vector3 &V1, const Vector3 &V2);			//�O��
	float Distance( const Vector3 &V1) const;								//2�_�Ԃ̋���
	static float Distance( const Vector3 &V1, const Vector3 &V2);			//2�_�Ԃ̋���
	D3DXVECTOR3 ConvertToDX() const { return D3DXVECTOR3( x, y, z);}		//D3DXVECTOR3�ɕϊ�
	static Vector3 ConvertFromDX( D3DXVECTOR3 Vec) 							//D3DXVECTOR3����ϊ�
	{ 
		return Vector3(Vec.x, Vec.y, Vec.z);
	}
	
	//���Z�q�I�[�o�[���[�h
	Vector3 &operator = ( const Vector3 &V1);
	Vector3 &operator += ( const Vector3 &V1);
	Vector3 &operator -= ( const Vector3 &V1);
	Vector3 &operator *= ( const Vector3 &V1);
	Vector3 &operator /= ( const Vector3 &V1);
	Vector3 &operator *= ( const float& fScalar);
	Vector3 &operator /= ( const float& fScalar);
	Vector3 operator + ( const Vector3 &V1) const;
	Vector3 operator - ( const Vector3 &V1) const;
	Vector3 operator * ( const Vector3 &V1) const;
	Vector3 operator / ( const Vector3 &V1) const;
	Vector3 operator * ( const float& fScalar) const;
	Vector3 operator / ( const float& fScalar) const;
	bool operator == ( const Vector3 &V1) const;
	bool operator != ( const Vector3 &V1) const;

private:

};


#endif