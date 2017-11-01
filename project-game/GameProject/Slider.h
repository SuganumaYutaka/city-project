/*==============================================================================
	
	Slider.h - スライダー
														Author : Yutaka Suganuma
														Date   : 2017/7/19
==============================================================================*/
#ifndef _SLIDER_H_
#define _SLIDER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class SpriteRenderer;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Slider : public Component
{
public:
	Slider( GameObject* pGameObject);
	void Uninit( void);

	void SetMaxValue( float fMaxValue) { m_fMaxValue = fMaxValue; UpdateSprite();}
	void SetValue( float fValue) { m_fValue = fValue; UpdateSprite();}

	void SetSize( float x, float y) { m_Size = Vector2(x, y); UpdateSprite();}
	
	SpriteRenderer* m_pBack;		//描画部分（背景・フレーム）
	SpriteRenderer* m_pSlider;		//描画部分（バリュー）

private:
	float m_fMaxValue;
	float m_fValue;
	Vector2 m_Size;

	void UpdateSprite();
};


#endif

