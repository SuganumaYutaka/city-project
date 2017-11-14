/*==============================================================================

    File.cpp - ファイル操作
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/5
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "File.h"
#include <fstream>

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define SEPARATORS " \n\0"

/*------------------------------------------------------------------------------
	静的メンバ変数
------------------------------------------------------------------------------*/
//ファイルの末尾
int File::EoF = -1;

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
File::File()
{
	TextStrage.clear();
	CurrentPosition = 0;
}

/*------------------------------------------------------------------------------
	ストレージを初期化する
------------------------------------------------------------------------------*/
void File::ClearTextStrage()
{
	TextStrage.clear();
	CurrentPosition = 0;
}

/*------------------------------------------------------------------------------
	ファイル読み込み
	戻り値 : true 成功	false 失敗
------------------------------------------------------------------------------*/
bool File::Load(const std::string& fileName)
{
	//ファイルオープン
	std::ifstream ifs( fileName);
	if (!ifs.is_open())
	{
		return false;
	}

	//ストレージに格納
	TextStrage.clear();
	TextStrage = std::string( std::istreambuf_iterator<char>(ifs),std::istreambuf_iterator<char>());
	CurrentPosition = 0;

	ifs.close();

	return true;
}

/*------------------------------------------------------------------------------
	ファイル書き込み
	戻り値:	true 成功	false 失敗
------------------------------------------------------------------------------*/
bool File::Save(const std::string& fileName)
{
	//ファイルオープン
	std::ofstream ofs( fileName);
	if (!ofs.is_open())
	{
		return false;
	}

	//ファイルにストレージを書き込む
	ofs << TextStrage << std::endl;

	ofs.close();

	return true;
}

/*------------------------------------------------------------------------------
	現在位置から指定文字列を検索
	戻り値:	int 見つけた位置	-1 存在しない
------------------------------------------------------------------------------*/
int File::FindWord(std::string targetWord)
{
	auto position = TextStrage.find( targetWord, CurrentPosition);
	if (position == std::string::npos)
	{
		//見つからない
		return EoF;
	}

	return position;
}

/*------------------------------------------------------------------------------
	指定位置から指定文字列を検索
	戻り値:	int 見つけた位置	-1 存在しない
------------------------------------------------------------------------------*/
int File::FindWord(std::string targetWord, int startPosition)
{
	auto position = TextStrage.find( targetWord, startPosition);
	if (position == std::string::npos)
	{
		//見つからない
		return EoF;
	}

	return position;
}



/*------------------------------------------------------------------------------
	現在位置からの文字列を取得
------------------------------------------------------------------------------*/
std::string File::GetWord()
{
	auto endPosition = TextStrage.find_first_of( SEPARATORS, CurrentPosition);

	if (endPosition == std::string::npos)
	{
		// 文字列の最後を指定
		endPosition = TextStrage.size();
	}

	return TextStrage.substr( CurrentPosition, endPosition - CurrentPosition);
}

/*------------------------------------------------------------------------------
	指定位置からの文字列を取得
------------------------------------------------------------------------------*/
std::string File::GetWord( int startPosition)
{
	auto endPosition = TextStrage.find_first_of( SEPARATORS, startPosition);

	if (endPosition == std::string::npos)
	{
		// 文字列の最後を指定
		endPosition = TextStrage.size();
	}

	return TextStrage.substr( startPosition, endPosition - startPosition);
}

/*------------------------------------------------------------------------------
	現在位置からの文字列を取得（指定文字で区切る）
------------------------------------------------------------------------------*/
std::string File::GetWordSeparatedByArg(const std::string separators)
{
	auto endPosition = TextStrage.find_first_of( separators, CurrentPosition);

	if (endPosition == std::string::npos)
	{
		// 文字列の最後を指定
		endPosition = TextStrage.size();
	}

	return TextStrage.substr( CurrentPosition, endPosition - CurrentPosition);
}

/*------------------------------------------------------------------------------
	指定位置からの文字列を取得（指定文字で区切る）
------------------------------------------------------------------------------*/
std::string File::GetWordSeparatedByArg(const std::string separators, int startPosition)
{
	auto endPosition = TextStrage.find_first_of( separators, startPosition);

	if (endPosition == std::string::npos)
	{
		// 文字列の最後を指定
		endPosition = TextStrage.size();
	}

	return TextStrage.substr( startPosition, endPosition - startPosition);
}

/*------------------------------------------------------------------------------
	CurrentPositionを1単語分進める
------------------------------------------------------------------------------*/
int File::ForwardPositionToNextWord()
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
	CurrentPositionを1単語分戻す
------------------------------------------------------------------------------*/
int File::BackPositionToPrevWord()
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
	CurrentPositionを1行分進める
------------------------------------------------------------------------------*/
int File::ForwardPositionToNextSentence()
{
	auto position = TextStrage.find_first_of( '\n', CurrentPosition);
	if (position == std::string::npos)
	{
		return EoF;
	}
	CurrentPosition = position + 1;

	return CurrentPosition;
}

/*------------------------------------------------------------------------------
	CurrentPositionを1行分戻す
------------------------------------------------------------------------------*/
int File::BackPositionToPrevSentence()
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
	Stringを追加
------------------------------------------------------------------------------*/
int File::AddString(const std::string& str)
{
	TextStrage += str+ '\n';
	CurrentPosition += str.size() + 1;
	return CurrentPosition;
}

/*------------------------------------------------------------------------------
	オペレーター
------------------------------------------------------------------------------*/
File& File::operator << (const std::string& str)
{
	TextStrage += str + '\n';
	CurrentPosition += str.size() + 1;
	return *this;
}
File& File::operator += (const std::string& str)
{
	TextStrage += str + '\n';
	CurrentPosition += str.size() + 1;
	return *this;
}
