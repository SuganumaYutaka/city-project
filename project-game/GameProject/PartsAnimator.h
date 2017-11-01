/*==============================================================================

    PartsAnimator.h - パーツアニメーション
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/6
==============================================================================*/
#ifndef _PARTS_ANIMATOR_H_
#define _PARTS_ANIMATOR_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "GameObject.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class PartsAnimator : public Component
{
public:
	PartsAnimator( GameObject *pGameObject);
	void Uninit( void);

	void Load( const char *pFileName);
	void Update( void);

	void SetNextMotion( int nNextMotion, int nNextKey);
	int GetNowMotion( void) { return m_nNowMotion;}
	bool IsEndMotion(void);

private:
	typedef struct
	{
		Vector3 Position;		//位置
		Vector3 Rotation;		//角度
	}KEY;

	typedef struct
	{
		int nFrame;					//フレーム数
		std::vector<KEY> vecKey;	//キーデータ（パーツ数分動的に確保）
	}KEYSET;

	typedef struct
	{
		int nLoop;						//ループフラグ
		int nNumKey;					//キー数
		std::vector<KEYSET> vecKeySet;	//キー設定データ
	}MOTION;

	std::vector<GameObject*> vecParts;		//パーツデータ（パーツ数分動的確保）
	std::vector<Vector3> vecOfsetPos;		//オフセット（位置）
	std::vector<Vector3> vecOfsetRot;		//オフセット（角度）
	std::vector<MOTION> vecMotion;			//モーションデータ（モーション数分動的確保）
	int m_nNowMotion;							//現在のモーション
	int m_nNextMotion;						//次のモーション
	int m_nNowKey;							//現在のキー
	int m_nNextKey;							//次のキー
	int m_nCntFrame;							//フレームカウンタ
	int m_nChangeFrame;						//キー変更までのフレーム数
	
	void SetFirstMotion();
	void ChangeMotion(int nNextMotion, int nNextKey);
};

#endif

