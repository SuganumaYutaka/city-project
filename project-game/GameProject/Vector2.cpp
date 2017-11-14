/*==============================================================================

    Vector2.cpp - 2Dベクトル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/3/30
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Vector2.h"
#include <assert.h>
#include <math.h>

/*------------------------------------------------------------------------------
	ベクトルの長さ
	引数
	void
	戻り値
	float
------------------------------------------------------------------------------*/
float Vector2::Length( void) const
{
	return (float)sqrt( ( x * x) + ( y * y));
}

/*------------------------------------------------------------------------------
	ベクトルの長さ(引数でベクトルを指定)
	引数
	const Vector2 &V1
	戻り値
	float
------------------------------------------------------------------------------*/
float Vector2::Length( const Vector2 &V1)
{
	return (float)sqrt( ( V1.x * V1.x) + ( V1.y * V1.y));
}

/*------------------------------------------------------------------------------
	ベクトルの正規化
	引数
	void
	戻り値
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
	ベクトルの正規化(引数でベクトルを指定)
	引数
	const Vector2 &V1
	戻り値
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
	ベクトルの内積
	引数
	const Vector2 &V1
	戻り値
	Vector2
------------------------------------------------------------------------------*/
float Vector2::Dot( const Vector2 &V1) const
{
	return ( x * V1.x) + ( y * V1.y);
}

/*------------------------------------------------------------------------------
	ベクトルの内積(引数でベクトルを指定)
	引数
	const Vector2 &V1
	const Vector2 &V2
	戻り値
	Vector2
------------------------------------------------------------------------------*/
float Vector2::Dot( const Vector2 &V1, const Vector2 &V2)
{
	return ( V1.x * V2.x) + ( V1.y * V2.y);
}

/*------------------------------------------------------------------------------
	ベクトルの外積
	引数
	const Vector2 &V1
	戻り値
	Vector2
------------------------------------------------------------------------------*/
float Vector2::Cross( const Vector2 &V1) const
{
	return ( x * V1.y) - ( y * V1.x);
}

/*------------------------------------------------------------------------------
	ベクトルの外積(引数でベクトルを指定)
	引数
	const Vector2 &V1
	const Vector2 &V2
	戻り値
	Vector2
------------------------------------------------------------------------------*/
float Vector2::Cross( const Vector2 &V1, const Vector2 &V2)
{
	return ( V1.x * V2.y) - ( V1.y * V2.x);
}

/*------------------------------------------------------------------------------
	2点間の距離
	引数
	const Vector2 &V1
	戻り値
	Vector2
------------------------------------------------------------------------------*/
float Vector2::Distance( const Vector2 &V1) const
{
	return ( *this - V1).Length();
}

/*------------------------------------------------------------------------------
	2点間の距離(引数でベクトルを指定)
	引数
	const Vector2 &V1
	const Vector2 &V2
	戻り値
	Vector2
------------------------------------------------------------------------------*/
float Vector2::Distance( const Vector2 &V1, const Vector2 &V2)
{
	return ( V1 - V2).Length( V1);
}

/*------------------------------------------------------------------------------
	std::stringに変換
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
	std::stringから変換
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


//演算子オーバーロード
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






