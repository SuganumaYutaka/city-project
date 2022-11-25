/*==============================================================================

    BuildingSurfacePattern.cpp - 建物の自動生成ー建物の表面パターン
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/12
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BuildingSurfacePattern.h"
#include "DebugLog.h"
#include "Texture.h"
#include "TextureManager.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
BuildingSurfacePattern::BuildingSurfacePattern(std::string fileName)
{
	m_SizeLevel = eBuildingSizeMiddle;

	//スクリプトを読み込み
	if (!Load(fileName))
	{
		DebugLog::Add( "BuildingSurfacePattern：スクリプトの読み込みに失敗しました。" + fileName + '\n');
	}

	m_Random = new Random();
}

/*------------------------------------------------------------------------------
	デストラクタ
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

	delete m_Random;
}

/*------------------------------------------------------------------------------
	ロード
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

		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return false;
		}
	}

	//チェック
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
	ランダムでUV座標を取得
------------------------------------------------------------------------------*/
const TextureUV& BuildingSurfacePattern::GetWall()
{
	int index = m_Random->GetInt() % m_Walls.size();
	return m_Walls[ index];
}

const TextureUV& BuildingSurfacePattern::GetWindow()
{
	int index = m_Random->GetInt() % m_Windows.size();
	return m_Windows[ index];
}

const TextureUV& BuildingSurfacePattern::GetEntrance()
{
	int index = m_Random->GetInt() % m_Entrances.size();
	return m_Entrances[ index];
}

const TextureUV& BuildingSurfacePattern::GetBorder()
{
	int index = m_Random->GetInt() % m_Borders.size();
	return m_Borders[ index];
}

/*------------------------------------------------------------------------------
	インデックスからUV座標を取得
------------------------------------------------------------------------------*/
const TextureUV& BuildingSurfacePattern::GetWall( unsigned int index)
{
	if (m_Walls.size() <= index)
	{
		DebugLog::Add( "BuildingSurfacePattern： (Wall)読み込まれた数と一致しません\n");
		return m_Walls[0];
	}
	return m_Walls[ index];
}

const TextureUV& BuildingSurfacePattern::GetWindow( unsigned int index)
{
	if (m_Windows.size() <= index)
	{
		DebugLog::Add( "BuildingSurfacePattern： (Window)読み込まれた数と一致しません\n");
		return m_Windows[0];
	}
	return m_Windows[ index];
}

const TextureUV& BuildingSurfacePattern::GetEntrance( unsigned int index)
{
	if (m_Entrances.size() <= index)
	{
		DebugLog::Add( "BuildingSurfacePattern： (Entrance)読み込まれた数と一致しません\n");
		return m_Entrances[0];
	}
	return m_Entrances[ index];
}

const TextureUV& BuildingSurfacePattern::GetBorder( unsigned int index)
{
	if (m_Borders.size() <= index)
	{
		DebugLog::Add( "BuildingSurfacePattern： (Border)読み込まれた数と一致しません\n");
		return m_Borders[0];
	}
	return m_Borders[ index];
}