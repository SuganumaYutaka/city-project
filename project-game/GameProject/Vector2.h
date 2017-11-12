/*==============================================================================

    Vector2.h - 2Dベクトル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/3/30
==============================================================================*/
#ifndef _VECTOR2_H_
#define _VECTOR2_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include <d3dx9.h>
#include <string>

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Vector2
{
public:
	float x;	//x座標
	float y;	//y座標

	Vector2() : x( 0.0f), y( 0.0f) {}
	Vector2( const float& X, const float& Y) : x( X), y( Y) {}
	Vector2( D3DXVECTOR2 Vec) : x(Vec.x), y(Vec.y) {}
	
	float Length( void) const;												//長さ
	static float Length( const Vector2 &V1);								//長さ
	Vector2 Normalize( void) const;											//正規化
	static Vector2 Normalize( const Vector2 &V1);							//正規化
	float Dot( const Vector2 &V1) const;									//内積
	static float Dot( const Vector2 &V1, const Vector2 &V2);				//内積
	float Cross( const Vector2 &V1) const;									//外積
	static float Cross( const Vector2 &V1, const Vector2 &V2);				//外積
	float Distance( const Vector2 &V1) const;								//2点間の距離
	static float Distance( const Vector2 &V1, const Vector2 &V2);			//2点間の距離
	D3DXVECTOR2 ConvertToDX() const { return D3DXVECTOR2( x, y);}			//D3DXVECTOR2に変換
	static Vector2 ConvertFromDX( D3DXVECTOR2 Vec) 							//D3DXVECTOR2から変換
	{ 
		return Vector2(Vec.x, Vec.y);
	}
	std::string ConvertToString();
	
	//演算子オーバーロード
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