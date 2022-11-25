/*==============================================================================

    Vector3.h - 3Dベクトル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/3/30
==============================================================================*/
#ifndef _VECTOR3_H_
#define _VECTOR3_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include <d3dx9.h>
#include <string>

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Vector3
{
public:
	float x;	//x座標
	float y;	//y座標
	float z;	//z座標

	Vector3() : x( 0.0f), y( 0.0f), z( 0.0f) {}
	Vector3( float X, float Y, float Z) : x(X), y(Y), z(Z) {}
	Vector3( D3DXVECTOR3 Vec) : x(Vec.x), y(Vec.y), z(Vec.z) {}
	
	float Length( void) const;												//長さ
	static float Length( const Vector3 &V1);								//長さ
	Vector3 Normalize( void) const;											//正規化
	static Vector3 Normalize( const Vector3 &V1);							//正規化
	float Dot( const Vector3 &V1) const;									//内積
	static float Dot( const Vector3 &V1, const Vector3 &V2);				//内積
	Vector3 Cross( const Vector3 &V1) const;								//外積
	static Vector3 Cross( const Vector3 &V1, const Vector3 &V2);			//外積
	float Distance( const Vector3 &V1) const;								//2点間の距離
	static float Distance( const Vector3 &V1, const Vector3 &V2);			//2点間の距離
	D3DXVECTOR3 ConvertToDX() const { return D3DXVECTOR3( x, y, z);}		//D3DXVECTOR3に変換
	static Vector3 ConvertFromDX( D3DXVECTOR3 Vec) 							//D3DXVECTOR3から変換
	{ 
		return Vector3(Vec.x, Vec.y, Vec.z);
	}
	std::string ConvertToString();
	int ConvertFromString(std::string& str, int current);
	
	//演算子オーバーロード
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