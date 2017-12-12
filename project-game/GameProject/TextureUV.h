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
#include <d3dx9.h>
#include <string>

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class TextureUV
{
public:
	TextureUV();
	TextureUV( const Vector2 &topLeftPosition, const Vector2 &size);

	void Init( void);
	void Init( const Vector2 &topLeftPosition, const Vector2 &size);

	void Scroll( const Vector2 &value);

	const Vector2& GetTopLeftPosition( void) { return m_TopLeftPosition;}
	const Vector2& GetSize( void){ return m_Size;}

	D3DXVECTOR2 GetTopLeft( void);
	D3DXVECTOR2 GetTopRight( void);
	D3DXVECTOR2 GetBottomLeft( void);
	D3DXVECTOR2 GetBottomRight( void);

	std::string ConvertToString();
	int ConvertFromString(std::string& str, int current);

private:
	Vector2 m_TopLeftPosition;
	Vector2 m_Size;
};


#endif