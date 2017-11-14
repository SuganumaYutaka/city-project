/*==============================================================================
	
	Slider.cpp - スライダー
														Author : Yutaka Suganuma
														Date   : 2017/7/19
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Slider.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "SpriteRenderer.h"

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* Slider::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<Slider>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Slider::Slider( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	//初期値設定
	m_Size = Vector2( 200.0f, 50.0f);
	m_fMaxValue = 1.0f;
	m_fValue = 1.0f;

	//レンダラー追加
	GameObject* obj = new GameObject( pGameObject);
	obj->IsCreatedByOtherComponent = true;
	m_pBack = obj->AddComponent<SpriteRenderer>();
	m_pBack->IsCreatedByOtherComponent = true;
	obj = new GameObject( pGameObject);
	obj->IsCreatedByOtherComponent = true;
	m_pSlider = obj->AddComponent<SpriteRenderer>();
	m_pSlider->IsCreatedByOtherComponent = true;

	//更新
	UpdateSprite();
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void Slider::Uninit( void)
{

}

/*------------------------------------------------------------------------------
	スプライトレンダラー更新
------------------------------------------------------------------------------*/
void Slider::UpdateSprite( void)
{
	Vector3 Size = Vector3( m_Size.x, m_Size.y, 1.0f);
	m_pBack->m_pTransform->SetLocalPosition( m_pTransform->GetLocalPosition());
	m_pBack->m_pTransform->SetLocalScale( Size);

	Size.x = m_Size.x * m_fValue / m_fMaxValue;
	Vector3 Pos = m_pTransform->GetLocalPosition();
	Pos.x += ( -m_Size.x + Size.x) * 0.5f;
	m_pSlider->m_pTransform->SetLocalPosition( Pos);
	m_pSlider->m_pTransform->SetLocalScale( Size);
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void Slider::Load(Text& text)
{
	//textを読み進める
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "MaxValue")
		{
			text.ForwardPositionToNextWord();
			m_fMaxValue = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Value")
		{
			text.ForwardPositionToNextWord();
			m_fValue = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Size")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_Size.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}

		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	セーブ
------------------------------------------------------------------------------*/
void Slider::Save(Text& text)
{
	StartSave(text);

	text += "MaxValue " + std::to_string(m_fMaxValue) + '\n';
	text += "Value " + std::to_string(m_fValue) + '\n';
	text += "Size " + m_Size.ConvertToString() + '\n';

	EndSave( text);
}