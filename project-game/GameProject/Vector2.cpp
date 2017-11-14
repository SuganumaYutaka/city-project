/*==============================================================================

    Vector2.cpp - 2D�x�N�g��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/3/30
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Vector2.h"
#include <assert.h>
#include <math.h>

/*------------------------------------------------------------------------------
	�x�N�g���̒���
	����
	void
	�߂�l
	float
------------------------------------------------------------------------------*/
float Vector2::Length( void) const
{
	return (float)sqrt( ( x * x) + ( y * y));
}

/*------------------------------------------------------------------------------
	�x�N�g���̒���(�����Ńx�N�g�����w��)
	����
	const Vector2 &V1
	�߂�l
	float
------------------------------------------------------------------------------*/
float Vector2::Length( const Vector2 &V1)
{
	return (float)sqrt( ( V1.x * V1.x) + ( V1.y * V1.y));
}

/*------------------------------------------------------------------------------
	�x�N�g���̐��K��
	����
	void
	�߂�l
	Vector2
------------------------------------------------------------------------------*/
Vector2 Vector2::Normalize( void) const
{
	const float fLength = Length();
	if( fLength == 0.0f)
	{
		return Vector2();
	}
	return *this / fLength;
}

/*------------------------------------------------------------------------------
	�x�N�g���̐��K��(�����Ńx�N�g�����w��)
	����
	const Vector2 &V1
	�߂�l
	Vector2
------------------------------------------------------------------------------*/
Vector2 Vector2::Normalize( const Vector2 &V1)
{
	const float fLength = Length( V1);
	if( fLength == 0.0f)
	{
		return Vector2();
	}
	return V1 / fLength;
}

/*------------------------------------------------------------------------------
	�x�N�g���̓���
	����
	const Vector2 &V1
	�߂�l
	Vector2
------------------------------------------------------------------------------*/
float Vector2::Dot( const Vector2 &V1) const
{
	return ( x * V1.x) + ( y * V1.y);
}

/*------------------------------------------------------------------------------
	�x�N�g���̓���(�����Ńx�N�g�����w��)
	����
	const Vector2 &V1
	const Vector2 &V2
	�߂�l
	Vector2
------------------------------------------------------------------------------*/
float Vector2::Dot( const Vector2 &V1, const Vector2 &V2)
{
	return ( V1.x * V2.x) + ( V1.y * V2.y);
}

/*------------------------------------------------------------------------------
	�x�N�g���̊O��
	����
	const Vector2 &V1
	�߂�l
	Vector2
------------------------------------------------------------------------------*/
float Vector2::Cross( const Vector2 &V1) const
{
	return ( x * V1.y) - ( y * V1.x);
}

/*------------------------------------------------------------------------------
	�x�N�g���̊O��(�����Ńx�N�g�����w��)
	����
	const Vector2 &V1
	const Vector2 &V2
	�߂�l
	Vector2
------------------------------------------------------------------------------*/
float Vector2::Cross( const Vector2 &V1, const Vector2 &V2)
{
	return ( V1.x * V2.y) - ( V1.y * V2.x);
}

/*------------------------------------------------------------------------------
	2�_�Ԃ̋���
	����
	const Vector2 &V1
	�߂�l
	Vector2
------------------------------------------------------------------------------*/
float Vector2::Distance( const Vector2 &V1) const
{
	return ( *this - V1).Length();
}

/*------------------------------------------------------------------------------
	2�_�Ԃ̋���(�����Ńx�N�g�����w��)
	����
	const Vector2 &V1
	const Vector2 &V2
	�߂�l
	Vector2
------------------------------------------------------------------------------*/
float Vector2::Distance( const Vector2 &V1, const Vector2 &V2)
{
	return ( V1 - V2).Length( V1);
}

/*------------------------------------------------------------------------------
	std::string�ɕϊ�
------------------------------------------------------------------------------*/
std::string Vector2::ConvertToString()
{
	std::string str;
	str += std::to_string(x);
	str += ' ';
	str += std::to_string(y);
	return str;
}

/*------------------------------------------------------------------------------
	std::string����ϊ�
------------------------------------------------------------------------------*/
int Vector2::ConvertFromString(std::string& str, int current)
{
	int end = current;
	end = str.find_first_of( " ", current);
	if (end == std::string::npos)
	{
		return -1;
	}
	x = std::stof(str.substr( current, end - current));
	current = end + 1;

	end = str.find_first_of( " ", current);
	if (end == std::string::npos)
	{
		return -1;
	}
	y = std::stof(str.substr( current, end - current));
	
	return end;
}


//���Z�q�I�[�o�[���[�h
Vector2 &Vector2::operator += ( const Vector2 &V1)
{
	return *this = *this + V1;
}
Vector2 &Vector2::operator -= ( const Vector2 &V1)
{
	return *this = *this - V1;
}
Vector2 &Vector2::operator *= ( const float& fScalar)
{
	return *this = *this * fScalar;
}
Vector2 &Vector2::operator /= ( const float& fScalar)
{
	assert( fScalar != 0.0f);
	return *this = *this / fScalar;
}
Vector2 &Vector2::operator = ( const Vector2 &V1)
{
	this->x = V1.x;
	this->y = V1.y;
	return *this;
}
Vector2 Vector2::operator + ( const Vector2 &V1) const
{
	return Vector2( x + V1.x, y + V1.y);
}
Vector2 Vector2::operator - ( const Vector2 &V1) const
{
	return Vector2( x - V1.x, y - V1.y);
}
Vector2 Vector2::operator * ( const float& fScalar) const
{
	return Vector2( x * fScalar, y * fScalar);
}
Vector2 Vector2::operator / ( const float& fScalar) const
{
	assert( fScalar != 0.0f);
	return Vector2( x / fScalar, y / fScalar);
}
bool Vector2::operator == ( const Vector2 &V1) const
{
	return ( x == V1.x) && ( y == V1.y);
}
bool Vector2::operator != ( const Vector2 &V1) const
{
	return !( *this == V1);
}






