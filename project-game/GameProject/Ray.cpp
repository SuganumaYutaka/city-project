/*==============================================================================

    Ray.cpp - 直線
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/17
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Ray.h"

/*------------------------------------------------------------------------------
	std::stringに変換
------------------------------------------------------------------------------*/
std::string Ray::ConvertToString()
{
	std::string str;
	str += Position.ConvertToString();
	str += '\n';
	str += Direction.ConvertToString();
	return str;
}

/*------------------------------------------------------------------------------
	std::stringから変換
------------------------------------------------------------------------------*/
int Ray::ConvertFromString(std::string& str, int current)
{
	int end = current;

	end = Position.ConvertFromString( str, current);

	end = str.find_first_not_of( " \n\0", current);
	if (end == std::string::npos)
	{
		return -1;
	}
	end = Direction.ConvertFromString( str, current);
	return end;
}
