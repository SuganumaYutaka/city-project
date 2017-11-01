/*==============================================================================
	
	Slider.h - �X���C�_�[
														Author : Yutaka Suganuma
														Date   : 2017/7/19
==============================================================================*/
#ifndef _SLIDER_H_
#define _SLIDER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class SpriteRenderer;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Slider : public Component
{
public:
	Slider( GameObject* pGameObject);
	void Uninit( void);

	void SetMaxValue( float fMaxValue) { m_fMaxValue = fMaxValue; UpdateSprite();}
	void SetValue( float fValue) { m_fValue = fValue; UpdateSprite();}

	void SetSize( float x, float y) { m_Size = Vector2(x, y); UpdateSprite();}
	
	SpriteRenderer* m_pBack;		//�`�敔���i�w�i�E�t���[���j
	SpriteRenderer* m_pSlider;		//�`�敔���i�o�����[�j

private:
	float m_fMaxValue;
	float m_fValue;
	Vector2 m_Size;

	void UpdateSprite();
};


#endif

