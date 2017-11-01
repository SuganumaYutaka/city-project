/*==============================================================================

    TextureUV.h - UV�f�[�^
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/4
==============================================================================*/
#ifndef _TEXTURE_UV_H_
#define _TEXTURE_UV_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Vector2.h"

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class TextureUV
{
public:
	TextureUV();
	TextureUV( const Vector2 &Pos, const Vector2 &Size);
	TextureUV( const Vector2 &NumPart, const int nPart);

	Vector2 UV[ 4];
	Vector2 m_Pos;			//UV�ʒu�i0.0f �` 1.0f�j
	Vector2 m_Size;			//UV�T�C�Y
	Vector2 m_NumPart;		//������
	int m_nPart;			//���������摜�̉��Ԗڂɓ����邩

	void Scroll( const Vector2 &Value);
	void SetScroll( const Vector2 &Pos = Vector2( 0.0f, 0.0f), const Vector2 &Size = Vector2( 1.0f, 1.0f));
	void AddPart( int nValue = 1);
	void SetPart( int NumPartX, int NumPartY, int nPart);

	int GetMaxPart() { return (int)m_NumPart.x * (int)m_NumPart.y; }

private:
	
};


#endif