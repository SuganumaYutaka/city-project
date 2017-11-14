/*==============================================================================

    SkinMeshModel.h - スキンメッシュモデル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/10/09
==============================================================================*/
#ifndef _SKIN_MESH_MODEL_H_
#define _SKIN_MESH_MODEL_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"
#include "SkinMeshDataFormat.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Material;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class SkinMeshModel : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	SkinMeshModel( GameObject *pGameObject);
	void Uninit( void);

	void LoadModel( std::string FileName, float scaleRate = 1.0f);
	void Update( void);
	
	int GetAnimationState() const { return m_AnimationState;}
	int GetFrame() const {return m_CntFrame;}

	void SetScale( float rate);

	virtual void Save( Text& text);
	virtual void Load( Text& text);

private:
	std::string m_FileName;
	std::vector<AnimationClip> vecAnimationClip;
	bool m_bLoad;
	
	int m_CntFrame;			//現在のフレーム
	int m_AnimationState;	//現在のアニメーション

	float ScaleRate;
};

#endif

