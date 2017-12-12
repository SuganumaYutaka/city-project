/*==============================================================================

    BuildingSurfacePattern.cpp - �����̎��������[�����̕\�ʃp�^�[��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/12
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BuildingSurfacePattern.h"
#include "DebugLog.h"
#include "Texture.h"
#include "TextureManager.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BuildingSurfacePattern::BuildingSurfacePattern(std::string fileName)
{
	m_SizeLevel = eBuildingSizeMiddle;

	//�X�N���v�g��ǂݍ���
	if (!Load(fileName))
	{
		DebugLog::Add( "BuildingSurfacePattern�F�X�N���v�g�̓ǂݍ��݂Ɏ��s���܂����B" + fileName + '\n');
	}
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
BuildingSurfacePattern::~BuildingSurfacePattern()
{
	m_Walls.clear();
	m_Walls.shrink_to_fit();

	m_Windows.clear();
	m_Windows.shrink_to_fit();

	m_Entrances.clear();
	m_Entrances.shrink_to_fit();

	m_Borders.clear();
	m_Borders.shrink_to_fit();
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
bool BuildingSurfacePattern::Load(std::string fileName)
{
	Text text;
	text.Load( fileName);

	if (text.GetWord() != "BuildingSurfacePatternScript")
	{
		return false;
	}

	TextureUV texUV;

	while ( text.GetWord() != "EndScript")
	{
		if (text.GetWord() == "Texture")
		{
			text.ForwardPositionToNextWord();

			m_TextureFileName = text.GetWord();
		}

		if (text.GetWord() == "SizeLevel")
		{
			text.ForwardPositionToNextWord();

			if (text.GetWord() == "Small" || text.GetWord() == "small" || text.GetWord() == "SMALL")
			{
				m_SizeLevel = eBuildingSizeSmall;
			}
			else if (text.GetWord() == "Middle" || text.GetWord() == "middle" || text.GetWord() == "MIDDLE")
			{
				m_SizeLevel = eBuildingSizeMiddle;
			}
			else if (text.GetWord() == "Large" || text.GetWord() == "large" || text.GetWord() == "LARGE")
			{
				m_SizeLevel = eBuildingSizeLarge;
			}

		}

		if (text.GetWord() == "Wall")
		{
			text.ForwardPositionToNextWord();

			text.SetPosition( texUV.ConvertFromString( text.GetAllText(), text.GetPosition()));
			m_Walls.push_back( texUV);
		}
		
		if (text.GetWord() == "Window")
		{
			text.ForwardPositionToNextWord();

			text.SetPosition( texUV.ConvertFromString( text.GetAllText(), text.GetPosition()));
			m_Windows.push_back( texUV);
		}

		if (text.GetWord() == "Entrance")
		{
			text.ForwardPositionToNextWord();

			text.SetPosition( texUV.ConvertFromString( text.GetAllText(), text.GetPosition()));
			m_Entrances.push_back( texUV);
		}

		if (text.GetWord() == "Border")
		{
			text.ForwardPositionToNextWord();

			text.SetPosition( texUV.ConvertFromString( text.GetAllText(), text.GetPosition()));
			m_Borders.push_back( texUV);
		}

		//text��ǂݐi�߂�
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return false;
		}
	}

	//�`�F�b�N
	if (m_TextureFileName.size() == 0)
	{
		return false;
	}
	if (m_Walls.size() == 0)
	{
		return false;
	}
	if (m_Windows.size() == 0)
	{
		return false;
	}
	if (m_Entrances.size() == 0)
	{
		return false;
	}
	if (m_Borders.size() == 0)
	{
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------
	�C���f�b�N�X����UV���W���擾
------------------------------------------------------------------------------*/
const TextureUV& BuildingSurfacePattern::GetWall( unsigned int index)
{
	if (m_Walls.size() <= index)
	{
		DebugLog::Add( "BuildingSurfacePattern�F (Wall)�ǂݍ��܂ꂽ���ƈ�v���܂���\n");
		return m_Walls[0];
	}
	return m_Walls[ index];
}

const TextureUV& BuildingSurfacePattern::GetWindow( unsigned int index)
{
	if (m_Windows.size() <= index)
	{
		DebugLog::Add( "BuildingSurfacePattern�F (Window)�ǂݍ��܂ꂽ���ƈ�v���܂���\n");
		return m_Windows[0];
	}
	return m_Windows[ index];
}

const TextureUV& BuildingSurfacePattern::GetEntrance( unsigned int index)
{
	if (m_Entrances.size() <= index)
	{
		DebugLog::Add( "BuildingSurfacePattern�F (Entrance)�ǂݍ��܂ꂽ���ƈ�v���܂���\n");
		return m_Entrances[0];
	}
	return m_Entrances[ index];
}

const TextureUV& BuildingSurfacePattern::GetBorder( unsigned int index)
{
	if (m_Borders.size() <= index)
	{
		DebugLog::Add( "BuildingSurfacePattern�F (Border)�ǂݍ��܂ꂽ���ƈ�v���܂���\n");
		return m_Borders[0];
	}
	return m_Borders[ index];
}
