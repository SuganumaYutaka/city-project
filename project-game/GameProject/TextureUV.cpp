/*==============================================================================

    TextureUV.cpp - UVデータ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/4
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "TextureUV.h"

/*------------------------------------------------------------------------------
	コンストラクタ
	引数
	void
------------------------------------------------------------------------------*/
TextureUV::TextureUV()
{
	m_Pos = Vector2();
	m_Size = Vector2( 1.0f, 1.0f);
	m_NumPart = Vector2( 1.0f, 1.0f);
	m_nPart = 0;

	UV[ 0] = Vector2( 0.0f, 0.0f);
	UV[ 1] = Vector2( 1.0f, 0.0f);
	UV[ 2] = Vector2( 0.0f, 1.0f);
	UV[ 3] = Vector2( 1.0f, 1.0f);
}

/*------------------------------------------------------------------------------
	コンストラクタ
	引数
	const Vector2 &Pos
	const Vector2 &Size
------------------------------------------------------------------------------*/
TextureUV::TextureUV( const Vector2 &Pos, const Vector2 &Size) : m_Pos( Pos), m_Size( Size)
{
	assert( m_Size.x >= 0.0f && m_Size.y >= 0.0f);

	m_NumPart = Vector2( 1.0f, 1.0f);
	m_nPart = 0;

	m_Pos.x -= (int)m_Pos.x;
	m_Pos.y -= (int)m_Pos.y;

	UV[ 0] = Vector2( m_Pos.x, m_Pos.y);
	UV[ 1] = Vector2( m_Pos.x + m_Size.x, m_Pos.y);
	UV[ 2] = Vector2( m_Pos.x, m_Pos.y + m_Size.y);
	UV[ 3] = Vector2( m_Pos.x + m_Size.x, m_Pos.y + m_Size.y);
}

/*------------------------------------------------------------------------------
	コンストラクタ
	引数
	const Vector2 &NumPart
	const int nPart
------------------------------------------------------------------------------*/
TextureUV::TextureUV( const Vector2 &NumPart, const int nPart) : m_NumPart( NumPart), m_nPart( nPart)
{
	assert( m_NumPart.x > 0.0f);
	assert( m_NumPart.y > 0.0f);
	assert( m_nPart < m_NumPart.x * m_NumPart.y);

	m_Pos = Vector2();
	m_Size = Vector2( 1.0f / m_NumPart.x, 1.0f / m_NumPart.y);
	
	UV[ 0] = Vector2( ( m_nPart % (int)m_NumPart.x) * m_Size.x, ( m_nPart / (int)m_NumPart.x) * m_Size.y);
	UV[ 1] = Vector2( ( m_nPart % (int)m_NumPart.x + 1) * m_Size.x, ( m_nPart / (int)m_NumPart.x) * m_Size.y);
	UV[ 2] = Vector2( ( m_nPart % (int)m_NumPart.x) * m_Size.x, ( m_nPart / (int)m_NumPart.x + 1) * m_Size.y);
	UV[ 3] = Vector2( ( m_nPart % (int)m_NumPart.x + 1) * m_Size.x, ( m_nPart / (int)m_NumPart.x + 1) * m_Size.y);
}

/*------------------------------------------------------------------------------
	スクロール
	引数
	const Vector2 &Value
	戻り値
	void
------------------------------------------------------------------------------*/
void TextureUV::Scroll( const Vector2 &Value)
{
	m_Pos += Value;

	m_Pos.x -= (int)m_Pos.x;
	m_Pos.y -= (int)m_Pos.y;

	UV[ 0] = Vector2( m_Pos.x, m_Pos.y);
	UV[ 1] = Vector2( m_Pos.x + m_Size.x, m_Pos.y);
	UV[ 2] = Vector2( m_Pos.x, m_Pos.y + m_Size.y);
	UV[ 3] = Vector2( m_Pos.x + m_Size.x, m_Pos.y + m_Size.y);
}

/*------------------------------------------------------------------------------
	スクロール設定
	引数
	const Vector2 &Pos
	const Vector2 &Size
	戻り値
	void
------------------------------------------------------------------------------*/
void TextureUV::SetScroll( const Vector2 &Pos, const Vector2 &Size)
{
	assert( m_Size.x >= 0.0f && m_Size.y >= 0.0f);

	m_Pos = Pos;
	m_Size = Size;

	m_Pos.x -= (int)m_Pos.x;
	m_Pos.y -= (int)m_Pos.y;

	UV[ 0] = Vector2( m_Pos.x, m_Pos.y);
	UV[ 1] = Vector2( m_Pos.x + m_Size.x, m_Pos.y);
	UV[ 2] = Vector2( m_Pos.x, m_Pos.y + m_Size.y);
	UV[ 3] = Vector2( m_Pos.x + m_Size.x, m_Pos.y + m_Size.y);
}

/*------------------------------------------------------------------------------
	選択番号切り替え
	引数
	int nValue
	戻り値
	void
------------------------------------------------------------------------------*/
void TextureUV::AddPart( int nValue)
{
	m_nPart = ( m_nPart + nValue) % (int)( m_NumPart.x * m_NumPart.y);

	UV[ 0] = Vector2( ( m_nPart % (int)m_NumPart.x) * m_Size.x, ( m_nPart / (int)m_NumPart.x) * m_Size.y);
	UV[ 1] = Vector2( ( m_nPart % (int)m_NumPart.x + 1) * m_Size.x, ( m_nPart / (int)m_NumPart.x) * m_Size.y);
	UV[ 2] = Vector2( ( m_nPart % (int)m_NumPart.x) * m_Size.x, ( m_nPart / (int)m_NumPart.x + 1) * m_Size.y);
	UV[ 3] = Vector2( ( m_nPart % (int)m_NumPart.x + 1) * m_Size.x, ( m_nPart / (int)m_NumPart.x + 1) * m_Size.y);
}

/*------------------------------------------------------------------------------
	選択番号設定
	引数
	int nPart
	戻り値
	void
------------------------------------------------------------------------------*/
void TextureUV::SetPart( int NumPartX, int NumPartY, int nPart)
{
	assert( NumPartX >= 0);
	assert( NumPartY >= 0);
	assert( nPart >= 0);

	m_NumPart = Vector2( NumPartX, NumPartY);
	m_nPart = nPart;
	m_Pos = Vector2();
	m_Size = Vector2( 1.0f / m_NumPart.x, 1.0f / m_NumPart.y);
	
	UV[ 0] = Vector2( ( m_nPart % (int)m_NumPart.x) * m_Size.x, ( m_nPart / (int)m_NumPart.x) * m_Size.y);
	UV[ 1] = Vector2( ( m_nPart % (int)m_NumPart.x + 1) * m_Size.x, ( m_nPart / (int)m_NumPart.x) * m_Size.y);
	UV[ 2] = Vector2( ( m_nPart % (int)m_NumPart.x) * m_Size.x, ( m_nPart / (int)m_NumPart.x + 1) * m_Size.y);
	UV[ 3] = Vector2( ( m_nPart % (int)m_NumPart.x + 1) * m_Size.x, ( m_nPart / (int)m_NumPart.x + 1) * m_Size.y);
}

