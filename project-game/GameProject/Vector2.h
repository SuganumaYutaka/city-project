/*==============================================================================

    Vector2.h - 2D�x�N�g��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/3/30
==============================================================================*/
#ifndef _VECTOR2_H_
#define _VECTOR2_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include <d3dx9.h>
#include <string>

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Vector2
{
public:
	float x;	//x���W
	float y;	//y���W

	Vector2() : x( 0.0f), y( 0.0f) {}
	Vector2( const float& X, const float& Y) : x( X), y( Y) {}
	Vector2( D3DXVECTOR2 Vec) : x(Vec.x), y(Vec.y) {}
	
	float Length( void) const;												//����
	static float Length( const Vector2 &V1);								//����
	Vector2 Normalize( void) const;											//���K��
	static Vector2 Normalize( const Vector2 &V1);							//���K��
	float Dot( const Vector2 &V1) const;									//����
	static float Dot( const Vector2 &V1, const Vector2 &V2);				//����
	float Cross( const Vector2 &V1) const;									//�O��
	static float Cross( const Vector2 &V1, const Vector2 &V2);				//�O��
	float Distance( const Vector2 &V1) const;								//2�_�Ԃ̋���
	static float Distance( const Vector2 &V1, const Vector2 &V2);			//2�_�Ԃ̋���
	D3DXVECTOR2 ConvertToDX() const { return D3DXVECTOR2( x, y);}			//D3DXVECTOR2�ɕϊ�
	static Vector2 ConvertFromDX( D3DXVECTOR2 Vec) 							//D3DXVECTOR2����ϊ�
	{ 
		return Vector2(Vec.x, Vec.y);
	}
	std::string ConvertToString();
	
	//���Z�q�I�[�o�[���[�h
	Vector2 &operator = ( const Vector2 &V1);
	Vector2 &operator += ( const Vector2 &V1);
	Vector2 &operator -= ( const Vector2 &V1);
	Vector2 &operator *= ( const float& fScalar);
	Vector2 &operator /= ( const float& fScalar);
	Vector2 operator + ( const Vector2 &V1) const;
	Vector2 operator - ( const Vector2 &V1) const;
	Vector2 operator * ( const float& fScalar) const;
	Vector2 operator / ( const float& fScalar) const;
	bool operator == ( const Vector2 &V1) const;
	bool operator != ( const Vector2 &V1) const;

private:

};


#endif