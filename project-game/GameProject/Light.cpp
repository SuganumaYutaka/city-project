/*==============================================================================

    Light.h - ライト
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/10
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Light.h"
#include "GameObject.h"
#include "Transform.h"

/*------------------------------------------------------------------------------
	静的メンバ変数宣言
------------------------------------------------------------------------------*/
std::list<Light*> Light::m_listLight;		//ライトリスト
const D3DXVECTOR4 Light::DefAmb( 0.3f, 0.3f, 0.3f, 1.0f);
const D3DXVECTOR4 Light::DefDif( 1.0f, 1.0f, 1.0f, 1.0f);
const D3DXVECTOR4 Light::DefSpe( 1.0f, 1.0f, 1.0f, 1.0f);
const D3DXVECTOR4 Light::DefEmi( 0.5f, 0.5f, 0.5f, 0.5f);

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* Light::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<Light>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Light::Light( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Type = eLightDirectional;
	m_listLight.push_back(this);

	m_Ambient = DefAmb;
	m_Diffuse = DefDif;
	m_Specular = DefSpe;
	m_Emissive = DefEmi;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void Light::Uninit( void)
{
	//リストから削除
	for (auto ite = m_listLight.begin(); ite != m_listLight.end(); ++ite)
	{
		if (*ite == this)
		{
			m_listLight.erase(ite);
			break;
		}
	}
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void Light::Update()
{
	
}

/*------------------------------------------------------------------------------
	位置取得関数
------------------------------------------------------------------------------*/
D3DXVECTOR4* Light::GetPosition( void)
{
	auto Pos = m_pTransform->GetWorldPosition();
	return &D3DXVECTOR4( Pos.x, Pos.y, Pos.z, 0.0f);
}

/*------------------------------------------------------------------------------
	方向取得関数
------------------------------------------------------------------------------*/
D3DXVECTOR4* Light::GetDirection( void)
{
	Vector3 Dir = m_pTransform->GetForward();
	return &D3DXVECTOR4( Dir.x, Dir.y, Dir.z, 0.0f);
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void Light::Load(Text& text)
{
	//textを読み進める
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "Type")
		{
			text.ForwardPositionToNextWord();
			m_Type = (ELightType)std::stoi(text.GetWord());
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
void Light::Save(Text& text)
{
	StartSave(text);

	text += "Type " + std::to_string((int)m_Type) + '\n';

	EndSave( text);
}