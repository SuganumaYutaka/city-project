/*==============================================================================

    BuildingSurfacePattern.h - 建物の自動生成ー建物の表面パターン
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/12
==============================================================================*/
#ifndef _BUILDING_SURFACE_PATTERN_H_
#define _BUILDING_SURFACE_PATTERN_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Texture;

/*------------------------------------------------------------------------------
	列挙型定義
------------------------------------------------------------------------------*/
//大きさの段階
enum E_BUILDING_SIZE_LEVEL
{
	eBuildingSizeSmall = 0,
	eBuildingSizeMiddle,
	eBuildingSizeLarge,
	eBuildingSizeLevelCount
};

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BuildingSurfacePattern
{
public:
	BuildingSurfacePattern( std::string fileName);
	~BuildingSurfacePattern();

	const std::vector<TextureUV>& GetWalls( void) { return m_Walls;}
	const std::vector<TextureUV>& GetWindows( void) { return m_Windows;}
	const std::vector<TextureUV>& GetEntrances( void) { return m_Entrances;}
	const std::vector<TextureUV>& GetBorders( void) { return m_Borders;}

	const TextureUV& GetWall();
	const TextureUV& GetWindow();
	const TextureUV& GetEntrance();
	const TextureUV& GetBorder();
	const TextureUV& GetWall( unsigned int index);
	const TextureUV& GetWindow( unsigned int index);
	const TextureUV& GetEntrance( unsigned int index);
	const TextureUV& GetBorder( unsigned int index);

	int GetWallCount( void) { m_Walls.size(); }
	int GetWindowCount( void) { m_Windows.size(); }
	int GetEntranceCount( void) { m_Entrances.size(); }
	int GetBorderCount( void) { m_Borders.size(); }

	std::string GetTextureFileName( void) { return m_TextureFileName;}
	void SetSeed( int seed){ m_Random->SetSeed( seed); }
	
private:
	bool Load( std::string fileName);

	std::string m_TextureFileName;
	E_BUILDING_SIZE_LEVEL m_SizeLevel;
	std::vector<TextureUV> m_Walls;
	std::vector<TextureUV> m_Windows;
	std::vector<TextureUV> m_Entrances;
	std::vector<TextureUV> m_Borders;

	//乱数
	Random* m_Random;
};

#endif

