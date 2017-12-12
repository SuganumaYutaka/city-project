/*==============================================================================

    TextureUV.cpp - UV�f�[�^
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/4
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "TextureUV.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
TextureUV::TextureUV()
{
	Init();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^�i�e�N�X�`�����̈ʒu���w��j
------------------------------------------------------------------------------*/
TextureUV::TextureUV(const Vector2 &topLeftPosition, const Vector2 &size)
{
	Init( topLeftPosition, size);
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void TextureUV::Init( void)
{
	m_TopLeftPosition = Vector2( 0.0f, 0.0f);
	m_Size = Vector2( 1.0f, 1.0f);
}

/*------------------------------------------------------------------------------
	�������i�e�N�X�`�����̈ʒu���w��j
------------------------------------------------------------------------------*/
void TextureUV::Init( const Vector2 &topLeftPosition, const Vector2 &size)
{
	m_TopLeftPosition = topLeftPosition;
	m_Size = size;
}

/*------------------------------------------------------------------------------
	�X�N���[��
------------------------------------------------------------------------------*/
void TextureUV::Scroll( const Vector2 &value)
{
	m_TopLeftPosition += value;
}

/*------------------------------------------------------------------------------
	UV���W���擾
------------------------------------------------------------------------------*/
D3DXVECTOR2 TextureUV::GetTopLeft(void)
{
	return m_TopLeftPosition.ConvertToDX();
}
D3DXVECTOR2 TextureUV::GetTopRight(void)
{
	D3DXVECTOR2 topRight = m_TopLeftPosition.ConvertToDX();
	topRight.x += m_Size.x;
	return topRight;
}
D3DXVECTOR2 TextureUV::GetBottomLeft(void)
{
	D3DXVECTOR2 bottomLeft = m_TopLeftPosition.ConvertToDX();
	bottomLeft.y += m_Size.y;
	return bottomLeft;
}
D3DXVECTOR2 TextureUV::GetBottomRight(void)
{
	return ( m_TopLeftPosition + m_Size).ConvertToDX();
}

/*------------------------------------------------------------------------------
	std::string�ɕϊ�
------------------------------------------------------------------------------*/
std::string TextureUV::ConvertToString()
{
	std::string str;
	str += m_TopLeftPosition.ConvertToString();
	str += ' ';
	str += m_Size.ConvertToString();
	return str;
}

/*------------------------------------------------------------------------------
	std::string����ϊ�
------------------------------------------------------------------------------*/
int TextureUV::ConvertFromString(std::string& str, int current)
{
	int end = current;
	end = m_TopLeftPosition.ConvertFromString( str, current);
	if (end == -1)
	{
		return -1;
	}
	current = end + 1;
	end = m_Size.ConvertFromString( str, current);
	if (end == -1)
	{
		return -1;
	}
	
	return end;
}
