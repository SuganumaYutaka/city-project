/*==============================================================================

    Vector3.cpp - 3D�x�N�g��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/3/30
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Vector3.h"
#include <assert.h>
#include <math.h>

/*------------------------------------------------------------------------------
	�x�N�g���̒���
	����
	void
	�߂�l
	float
------------------------------------------------------------------------------*/
float Vector3::Length( void) const
{
	return (float)sqrt( ( x * x) + ( y * y) + ( z * z));
}

/*------------------------------------------------------------------------------
	�x�N�g���̒���(�����Ńx�N�g�����w��)
	����
	const Vector3 &V1
	�߂�l
	float
------------------------------------------------------------------------------*/
float Vector3::Length( const Vector3 &V1)
{
	return (float)sqrt( ( V1.x * V1.x) + ( V1.y * V1.y) + ( V1.z * V1.z));
}

/*------------------------------------------------------------------------------
	�x�N�g���̐��K��
	����
	void
	�߂�l
	Vector3
------------------------------------------------------------------------------*/
Vector3 Vector3::Normalize( void) const
{
	const float fLength = Length();
	if( fLength == 0.0f)
	{
		return Vector3();
	}
	return *this / fLength;
}

/*------------------------------------------------------------------------------
	�x�N�g���̐��K��(�����Ńx�N�g�����w��)
	����
	const Vector3 &V1
	�߂�l
	Vector3
------------------------------------------------------------------------------*/
Vector3 Vector3::Normalize( const Vector3 &V1)
{
	const float fLength = Length( V1);
	if( fLength == 0.0f)
	{
		return Vector3();
	}
	return V1 / fLength;
}

/*------------------------------------------------------------------------------
	�x�N�g���̓���
	����
	const Vector3 &V1
	�߂�l
	Vector3
------------------------------------------------------------------------------*/
float Vector3::Dot( const Vector3 &V1) const
{
	return ( x * V1.x) + ( y * V1.y) + ( z * V1.z);
}

/*------------------------------------------------------------------------------
	�x�N�g���̓���(�����Ńx�N�g�����w��)
	����
	const Vector3 &V1
	const Vector3 &V2
	�߂�l
	Vector3
------------------------------------------------------------------------------*/
float Vector3::Dot( const Vector3 &V1, const Vector3 &V2)
{
	return ( V1.x * V2.x) + ( V1.y * V2.y) + ( V1.z * V2.z);
}

/*------------------------------------------------------------------------------
	�x�N�g���̊O��
	����
	const Vector3 &V1
	�߂�l
	Vector3
------------------------------------------------------------------------------*/
Vector3 Vector3::Cross( const Vector3 &V1) const
{
	return Vector3( ( y * V1.z) - ( z * V1.y), ( z * V1.x) - ( x * V1.z), ( x * V1.y) - ( y * V1.x));
}

/*------------------------------------------------------------------------------
	�x�N�g���̊O��(�����Ńx�N�g�����w��)
	����
	const Vector3 &V1
	const Vector3 &V2
	�߂�l
	Vector3
------------------------------------------------------------------------------*/
Vector3 Vector3::Cross( const Vector3 &V1, const Vector3 &V2)
{
	return Vector3( ( V1.y * V2.z) - ( V1.z * V2.y), ( V1.z * V2.x) - ( V1.x * V2.z), ( V1.x * V2.y) - ( V1.y * V2.x));
}

/*------------------------------------------------------------------------------
	2�_�Ԃ̋���
	����
	const Vector3 &V1
	�߂�l
	Vector3
------------------------------------------------------------------------------*/
float Vector3::Distance( const Vector3 &V1) const
{
	return ( *this - V1).Length();
}

/*------------------------------------------------------------------------------
	2�_�Ԃ̋���(�����Ńx�N�g�����w��)
	����
	const Vector3 &V1
	const Vector3 &V2
	�߂�l
	Vector3
------------------------------------------------------------------------------*/
float Vector3::Distance( const Vector3 &V1, const Vector3 &V2)
{
	return ( V1 - V2).Length( V1);
}


//���Z�q�I�[�o�[���[�h
Vector3 &Vector3::operator += ( const Vector3 &V1)
{
	return *this = *this + V1;
}
Vector3 &Vector3::operator -= ( const Vector3 &V1)
{
	return *this = *this - V1;
}
Vector3 &Vector3::operator *= ( const Vector3 &V1)
{
	return *this = *this * V1;
}
Vector3 &Vector3::operator /= ( const Vector3 &V1)
{
	return *this = *this / V1;
}
Vector3 &Vector3::operator *= ( const float& fScalar)
{
	assert( fScalar != 0.0f);
	return *this = *this * fScalar;
}
Vector3 &Vector3::operator /= ( const float& fScalar)
{
	assert( fScalar != 0.0f);
	return *this = *this / fScalar;
}
Vector3 &Vector3::operator = ( const Vector3 &V1)
{
	this->x = V1.x;
	this->y = V1.y;
	this->z = V1.z;
	return *this;
}
Vector3 Vector3::operator + ( const Vector3 &V1) const
{
	return Vector3( x + V1.x, y + V1.y, z + V1.z);
}
Vector3 Vector3::operator - ( const Vector3 &V1) const
{
	return Vector3( x - V1.x, y - V1.y, z - V1.z);
}
Vector3 Vector3::operator * ( const Vector3 &V1) const
{
	return Vector3( x * V1.x, y * V1.y, z * V1.z);
}
Vector3 Vector3::operator / ( const Vector3 &V1) const
{
	return Vector3( x / V1.x, y / V1.y, z / V1.z);
}
Vector3 Vector3::operator * ( const float& fScalar) const
{
	//assert( fScalar != 0.0f);
	return Vector3( x * fScalar, y * fScalar, z * fScalar);
}
Vector3 Vector3::operator / ( const float& fScalar) const
{
	assert( fScalar != 0.0f);
	return Vector3( x / fScalar, y / fScalar, z / fScalar);
}
bool Vector3::operator == ( const Vector3 &V1) const
{
	return ( x == V1.x) && ( y == V1.y) && ( z == V1.z);
}
bool Vector3::operator != ( const Vector3 &V1) const
{
	return !( *this == V1);
}






