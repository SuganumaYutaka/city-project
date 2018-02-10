/*==============================================================================

    LandParameter.cpp - ���̎��������[�y�n�̐����ɗp����p�����[�^�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/9
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "LandParameter.h"

/*------------------------------------------------------------------------------
	�Z�[�u
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
	���[�h
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

