/*==============================================================================

    Text.h - �e�L�X�g����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/5
==============================================================================*/
#ifndef _TEXT_H_
#define _TEXT_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include <string>

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Text
{
public:
	static int EoF;

	Text();
	
	bool Load( const std::string& fileName);
	bool Save( const std::string& fileName);
	void ClearTextStrage();

	int FindWord( std::string targetWord);
	int FindWord( std::string targetWord, int startPosition);
	std::string GetWord();
	std::string GetWord(int startPosition);
	std::string GetWordSeparatedByArg(const std::string separators);
	std::string GetWordSeparatedByArg(const std::string separators, int startPosition);
	int ForwardPositionToNextWord();
	int BackPositionToPrevWord();
	int ForwardPositionToNextSentence();
	int BackPositionToPrevSentence();
	int AddString( const std::string& str);
	void SetPosition( int position) { CurrentPosition = position;}
	int GetPosition() const { return CurrentPosition;}

	Text& operator << ( const std::string& str);
	Text& operator += ( const std::string& str);

private:
	std::string TextStrage;
	int CurrentPosition;

};




#endif