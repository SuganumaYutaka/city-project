/*==============================================================================

    SkinMeshDataFormat.h - スキンメッシュのデータフォーマット
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/10/9
==============================================================================*/
#ifndef _SKIN_MESH_DATA_FORMAT_H_
#define _SKIN_MESH_DATA_FORMAT_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Material;

/*------------------------------------------------------------------------------
	構造体定義
------------------------------------------------------------------------------*/
//ボーン情報
struct BornRefarence
{
	BornRefarence() : Index(0), Weight(0.0f) {}
	BornRefarence(unsigned char index, float weight) : Index(index), Weight(weight) {}
	unsigned char Index;		//参照するボーンのインデックス番号
	float Weight;				//ウェイト値
};

//頂点情報（位置座標とボーン情報）
struct Point
{
	Point() : Position(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), nCntBornRefarence(0) {}
	Point(const D3DXVECTOR3& position) : Position(position), nCntBornRefarence(0) {}
	D3DXVECTOR3 Position;
	BornRefarence BornRefarence[4];
	int nCntBornRefarence;

	void SetBornRefarence(unsigned char index, float weight)
	{
		BornRefarence[nCntBornRefarence].Index = index;
		BornRefarence[nCntBornRefarence].Weight = weight;
		nCntBornRefarence++;
	}

};

//アニメーション情報
struct Animation
{
	std::vector<std::vector<D3DXMATRIX>> vecMatrix;		//アニメーション情報（フレーム数×クラスター数）
	std::string Name;
	int AllFrame;
};

//アニメーションクリップ
struct AnimationClip
{
	std::string Name;
	int AllFrame;
};

#endif