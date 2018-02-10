/*==============================================================================

    LandParameter.cpp - 町の自動生成ー土地の生成に用いるパラメーター
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/9
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "LandParameter.h"

/*------------------------------------------------------------------------------
	セーブ
------------------------------------------------------------------------------*/
std::string LandParameter::Save(void)
{
	std::string text;

	int vertexCount = (int)m_Vertices.size();
	text += std::to_string( vertexCount) + ' ';
	for (auto vertex : m_Vertices)
	{
		text += vertex.ConvertToString() + ' ';
	}

	int roadCount = (int)m_RoadIDs.size();
	text += std::to_string( roadCount) + ' ';
	for (auto roadId : m_RoadIDs)
	{
		text += std::to_string( roadId) + ' ';
	}

	return text;
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void LandParameter::Load(Text& text)
{
	int vertexCount = std::stoi( text.GetWord());
	for (int i = 0; i < vertexCount; i++)
	{
		text.ForwardPositionToNextWord();
		Vector3 vertex;
		text.SetPosition( vertex.ConvertFromString(text.GetAllText(), text.GetPosition()));
		m_Vertices.push_back( vertex);
	}

	text.ForwardPositionToNextWord();
		
	int roadCount = std::stoi( text.GetWord());
	for (int i = 0; i < roadCount; i++)
	{
		text.ForwardPositionToNextWord();
		int roadID = std::stoi( text.GetWord());
		m_RoadIDs.push_back( roadID);
	}
}

