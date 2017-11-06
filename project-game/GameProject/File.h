/*==============================================================================

    File.h - ファイル操作
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/5
==============================================================================*/
#ifndef _FILE_H_
#define _FILE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include <string>

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class File
{
public:
	static int EoF;

	File();
	
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

	File& operator << ( const std::string& str);
	File& operator += ( const std::string& str);

private:
	std::string TextStrage;
	int CurrentPosition;

};




#endif