/*==============================================================================

    Vector3.cpp - 3Dベクトル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/3/30
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Vector3.h"
#include <assert.h>
#include <math.h>

/*------------------------------------------------------------------------------
	ベクトルの長さ
	引数
	void
	戻り値
	float
------------------------------------------------------------------------------*/
float Vector3::Length( void) const
{
	return (float)sqrt( ( x * x) + ( y * y) + ( z * z));
}

/*------------------------------------------------------------------------------
	ベクトルの長さ(引数でベクトルを指定)
	引数
	const Vector3 &V1
	戻り値
	float
------------------------------------------------------------------------------*/
float Vector3::Length( const Vector3 &V1)
{
	return (float)sqrt( ( V1.x * V1.x) + ( V1.y * V1.y) + ( V1.z * V1.z));
}

/*------------------------------------------------------------------------------
	ベクトルの正規化
	引数
	void
	戻り値
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
	ベクトルの正規化(引数でベクトルを指定)
	引数
	const Vector3 &V1
	戻り値
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
	ベクトルの内積
	引数
	const Vector3 &V1
	戻り値
	Vector3
------------------------------------------------------------------------------*/
float Vector3::Dot( const Vector3 &V1) const
{
	return ( x * V1.x) + ( y * V1.y) + ( z * V1.z);
}

/*------------------------------------------------------------------------------
	ベクトルの内積(引数でベクトルを指定)
	引数
	const Vector3 &V1
	const Vector3 &V2
	戻り値
	Vector3
------------------------------------------------------------------------------*/
float Vector3::Dot( const Vector3 &V1, const Vector3 &V2)
{
	return ( V1.x * V2.x) + ( V1.y * V2.y) + ( V1.z * V2.z);
}

/*------------------------------------------------------------------------------
	ベクトルの外積
	引数
	const Vector3 &V1
	戻り値
	Vector3
------------------------------------------------------------------------------*/
Vector3 Vector3::Cross( const Vector3 &V1) const
{
	return Vector3( ( y * V1.z) - ( z * V1.y), ( z * V1.x) - ( x * V1.z), ( x * V1.y) - ( y * V1.x));
}

/*------------------------------------------------------------------------------
	ベクトルの外積(引数でベクトルを指定)
	引数
	const Vector3 &V1
	const Vector3 &V2
	戻り値
	Vector3
------------------------------------------------------------------------------*/
Vector3 Vector3::Cross( const Vector3 &V1, const Vector3 &V2)
{
	return Vector3( ( V1.y * V2.z) - ( V1.z * V2.y), ( V1.z * V2.x) - ( V1.x * V2.z), ( V1.x * V2.y) - ( V1.y * V2.x));
}

/*------------------------------------------------------------------------------
	2点間の距離
	引数
	const Vector3 &V1
	戻り値
	Vector3
------------------------------------------------------------------------------*/
float Vector3::Distance( const Vector3 &V1) const
{
	return ( *this - V1).Length();
}

/*------------------------------------------------------------------------------
	2点間の距離(引数でベクトルを指定)
	引数
	const Vector3 &V1
	const Vector3 &V2
	戻り値
	Vector3
------------------------------------------------------------------------------*/
float Vector3::Distance( const Vector3 &V1, const Vector3 &V2)
{
	return ( V1 - V2).Length( V1);
}


//演算子オーバーロード
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






