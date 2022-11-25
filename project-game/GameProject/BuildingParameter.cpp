/*==============================================================================

    BuildingParameter.cpp - 建物の自動生成ー建物の生成に用いるパラメーター
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BuildingParameter.h"

/*------------------------------------------------------------------------------
	ジオメトリ：セーブ
------------------------------------------------------------------------------*/
std::string GeometryParameter::Save(void)
{
	std::string text;

	text += std::to_string( m_GroundFloorHeight) + ' ';
	text += std::to_string( m_FloorHeight) + ' ';
	text += std::to_string( m_EntranceWidth) + ' ';
	text += std::to_string( m_WindowWidth) + ' ';
	text += std::to_string( m_SurfacePatternID) + ' ';
	text += std::to_string( m_RandomSeed) + ' ';

	int shapeCount = m_ShapeParameters.size();
	text += std::to_string( shapeCount) + ' ';
	for (auto shapeParameter : m_ShapeParameters)
	{
		text += shapeParameter->Save() + ' ';
	}
	
	return text;
}

/*------------------------------------------------------------------------------
	ジオメトリ：ロード
------------------------------------------------------------------------------*/
void GeometryParameter::Load(Text& text)
{
	m_GroundFloorHeight = std::stof( text.GetWord());
	text.ForwardPositionToNextWord();
	m_FloorHeight = std::stof( text.GetWord());
	text.ForwardPositionToNextWord();
	m_EntranceWidth = std::stof( text.GetWord());
	text.ForwardPositionToNextWord();
	m_WindowWidth = std::stof( text.GetWord());
	text.ForwardPositionToNextWord();
	m_SurfacePatternID = std::stoi( text.GetWord());
	text.ForwardPositionToNextWord();
	m_RandomSeed = std::stoi( text.GetWord());
	text.ForwardPositionToNextWord();
	
	int shapeCount = std::stoi( text.GetWord());
	for (int i = 0; i < shapeCount; i++)
	{
		text.ForwardPositionToNextWord();
		auto shapeParameter = new ShapeParameter();
		shapeParameter->Load( text);
		m_ShapeParameters.push_back( shapeParameter);
	}
}

/*------------------------------------------------------------------------------
	形状：セーブ
------------------------------------------------------------------------------*/
std::string ShapeParameter::Save(void)
{
	std::string text;

	text += std::to_string( (int)m_Type) + ' ';
	text += m_Position.ConvertToString() + ' ';
	text += std::to_string( m_Rotation) + ' ';
	text += m_Size.ConvertToString() + ' ';

	return text;
}

/*------------------------------------------------------------------------------
	形状：ロード
------------------------------------------------------------------------------*/
void ShapeParameter::Load(Text& text)
{
	m_Type = (E_SHAPE_TYPE)std::stoi( text.GetWord());
	text.ForwardPositionToNextWord();
	text.SetPosition( m_Position.ConvertFromString(text.GetAllText(), text.GetPosition()));
	text.ForwardPositionToNextWord();
	m_Rotation = std::stof( text.GetWord());
	text.ForwardPositionToNextWord();
	text.SetPosition( m_Size.ConvertFromString(text.GetAllText(), text.GetPosition()));
}
