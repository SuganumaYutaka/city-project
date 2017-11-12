/*==============================================================================

    Text.cpp - �e�L�X�g����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/5
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Text.h"
#include <fstream>

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define SEPARATORS " \n\0"

/*------------------------------------------------------------------------------
	�ÓI�����o�ϐ�
------------------------------------------------------------------------------*/
//�t�@�C���̖���
int Text::EoF = -1;

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Text::Text()
{
	TextStrage.clear();
	CurrentPosition = 0;
}

/*------------------------------------------------------------------------------
	�X�g���[�W������������
------------------------------------------------------------------------------*/
void Text::ClearTextStrage()
{
	TextStrage.clear();
	CurrentPosition = 0;
}

/*------------------------------------------------------------------------------
	�t�@�C���ǂݍ���
	�߂�l : true ����	false ���s
------------------------------------------------------------------------------*/
bool Text::Load(const std::string& fileName)
{
	//�t�@�C���I�[�v��
	std::ifstream ifs( fileName);
	if (!ifs.is_open())
	{
		return false;
	}

	//�X�g���[�W�Ɋi�[
	TextStrage.clear();
	TextStrage = std::string( std::istreambuf_iterator<char>(ifs),std::istreambuf_iterator<char>());
	CurrentPosition = 0;

	ifs.close();

	return true;
}

/*------------------------------------------------------------------------------
	�t�@�C����������
	�߂�l:	true ����	false ���s
------------------------------------------------------------------------------*/
bool Text::Save(const std::string& fileName)
{
	//�t�@�C���I�[�v��
	std::ofstream ofs( fileName);
	if (!ofs.is_open())
	{
		return false;
	}

	//�t�@�C���ɃX�g���[�W����������
	ofs << TextStrage << std::endl;

	ofs.close();

	return true;
}

/*------------------------------------------------------------------------------
	���݈ʒu����w�蕶���������
	�߂�l:	int �������ʒu	EoF ���݂��Ȃ�
------------------------------------------------------------------------------*/
int Text::FindWord(std::string targetWord)
{
	auto position = TextStrage.find( targetWord, CurrentPosition);
	if (position == std::string::npos)
	{
		//������Ȃ�
		return EoF;
	}

	return position;
}

/*------------------------------------------------------------------------------
	�w��ʒu����w�蕶���������
	�߂�l:	int �������ʒu	EoF ���݂��Ȃ�
------------------------------------------------------------------------------*/
int Text::FindWord(std::string targetWord, int startPosition)
{
	auto position = TextStrage.find( targetWord, startPosition);
	if (position == std::string::npos)
	{
		//������Ȃ�
		return EoF;
	}

	return position;
}



/*------------------------------------------------------------------------------
	���݈ʒu����̕�������擾
------------------------------------------------------------------------------*/
std::string Text::GetWord()
{
	auto endPosition = TextStrage.find_first_of( SEPARATORS, CurrentPosition);

	if (endPosition == std::string::npos)
	{
		// ������̍Ō���w��
		endPosition = TextStrage.size();
	}

	return TextStrage.substr( CurrentPosition, endPosition - CurrentPosition);
}

/*------------------------------------------------------------------------------
	�w��ʒu����̕�������擾
------------------------------------------------------------------------------*/
std::string Text::GetWord( int startPosition)
{
	auto endPosition = TextStrage.find_first_of( SEPARATORS, startPosition);

	if (endPosition == std::string::npos)
	{
		// ������̍Ō���w��
		endPosition = TextStrage.size();
	}

	return TextStrage.substr( startPosition, endPosition - startPosition);
}

/*------------------------------------------------------------------------------
	���݈ʒu����̕�������擾�i�w�蕶���ŋ�؂�j
------------------------------------------------------------------------------*/
std::string Text::GetWordSeparatedByArg(const std::string separators)
{
	auto endPosition = TextStrage.find_first_of( separators, CurrentPosition);

	if (endPosition == std::string::npos)
	{
		// ������̍Ō���w��
		endPosition = TextStrage.size();
	}

	return TextStrage.substr( CurrentPosition, endPosition - CurrentPosition);
}

/*------------------------------------------------------------------------------
	�w��ʒu����̕�������擾�i�w�蕶���ŋ�؂�j
------------------------------------------------------------------------------*/
std::string Text::GetWordSeparatedByArg(const std::string separators, int startPosition)
{
	auto endPosition = TextStrage.find_first_of( separators, startPosition);

	if (endPosition == std::string::npos)
	{
		// ������̍Ō���w��
		endPosition = TextStrage.size();
	}

	return TextStrage.substr( startPosition, endPosition - startPosition);
}

/*------------------------------------------------------------------------------
	CurrentPosition��1�P�ꕪ�i�߂�
------------------------------------------------------------------------------*/
int Text::ForwardPositionToNextWord()
{
	auto position = TextStrage.find_first_of( SEPARATORS, CurrentPosition);
	if (position == std::string::npos)
	{
		return EoF;
	}
	position = TextStrage.find_first_not_of( SEPARATORS, position);
	if (position == std::string::npos)
	{
		return EoF;
	}
	CurrentPosition = position;

	return CurrentPosition;
}

/*------------------------------------------------------------------------------
	CurrentPosition��1�P�ꕪ�߂�
------------------------------------------------------------------------------*/
int Text::BackPositionToPrevWord()
{
	auto position = TextStrage.find_last_not_of( SEPARATORS, CurrentPosition - 1);
	if (position == std::string::npos)
	{
		return EoF;
	}

	position = TextStrage.find_last_of( SEPARATORS, position);
	if (position == std::string::npos)
	{
		return EoF;
	}
	
	CurrentPosition = position + 1;

	return CurrentPosition;
}

/*------------------------------------------------------------------------------
	CurrentPosition��1�s���i�߂�
------------------------------------------------------------------------------*/
int Text::ForwardPositionToNextSentence()
{
	auto position = TextStrage.find_first_of( '\n', CurrentPosition);
	if (position == std::string::npos)
	{
		return EoF;
	}
	position = TextStrage.find_first_not_of( SEPARATORS, position);
	if (position == std::string::npos)
	{
		return EoF;
	}

	CurrentPosition = position;

	return CurrentPosition;
}

/*------------------------------------------------------------------------------
	CurrentPosition��1�s���߂�
------------------------------------------------------------------------------*/
int Text::BackPositionToPrevSentence()
{
	auto position = TextStrage.find_last_of( '\n', CurrentPosition);
	if (position == std::string::npos)
	{
		return EoF;
	}

	position = TextStrage.find_last_of( '\n', position - 1);
	if (position == std::string::npos)
	{
		position = -1;
	}
	
	CurrentPosition = position + 1;

	return CurrentPosition;
}

/*------------------------------------------------------------------------------
	String��ǉ�
------------------------------------------------------------------------------*/
int Text::AddString(const std::string& str)
{
	TextStrage += str + ' ';
	CurrentPosition += str.size() + 1;
	return CurrentPosition;
}

/*------------------------------------------------------------------------------
	�I�y���[�^�[
------------------------------------------------------------------------------*/
Text& Text::operator << (const std::string& str)
{
	TextStrage += str + ' ';
	CurrentPosition += str.size() + 1;
	return *this;
}
Text& Text::operator += (const std::string& str)
{
	TextStrage += str + ' ';
	CurrentPosition += str.size() + 1;
	return *this;
}
