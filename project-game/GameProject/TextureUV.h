/*==============================================================================

    TextureUV.h - UVデータ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/4
==============================================================================*/
#ifndef _TEXTURE_UV_H_
#define _TEXTURE_UV_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Vector2.h"

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class TextureUV
{
public:
	TextureUV();
	TextureUV( const Vector2 &Pos, const Vector2 &Size);
	TextureUV( const Vector2 &NumPart, const int nPart);

	Vector2 UV[ 4];
	Vector2 m_Pos;			//UV位置（0.0f ～ 1.0f）
	Vector2 m_Size;			//UVサイズ
	Vector2 m_NumPart;		//分割数
	int m_nPart;			//分割した画像の何番目に当たるか

	void Scroll( const Vector2 &Value);
	void SetScroll( const Vector2 &Pos = Vector2( 0.0f, 0.0f), const Vector2 &Size = Vector2( 1.0f, 1.0f));
	void AddPart( int nValue = 1);
	void SetPart( int NumPartX, int NumPartY, int nPart);

	int GetMaxPart() { return (int)m_NumPart.x * (int)m_NumPart.y; }

private:
	
};


#endif