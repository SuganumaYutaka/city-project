/*==============================================================================

    Light.h - ���C�g
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/10
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Light.h"
#include "GameObject.h"
#include "Transform.h"

/*------------------------------------------------------------------------------
	�ÓI�����o�ϐ��錾
------------------------------------------------------------------------------*/
std::list<Light*> Light::m_listLight;		//���C�g���X�g
const D3DXVECTOR4 Light::DefAmb( 0.3f, 0.3f, 0.3f, 1.0f);
const D3DXVECTOR4 Light::DefDif( 1.0f, 1.0f, 1.0f, 1.0f);
const D3DXVECTOR4 Light::DefSpe( 1.0f, 1.0f, 1.0f, 1.0f);
const D3DXVECTOR4 Light::DefEmi( 0.5f, 0.5f, 0.5f, 0.5f);

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* Light::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<Light>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
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
	�I������
------------------------------------------------------------------------------*/
void Light::Uninit( void)
{
	//���X�g����폜
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
	�X�V
------------------------------------------------------------------------------*/
void Light::Update()
{
	
}

/*------------------------------------------------------------------------------
	�ʒu�擾�֐�
------------------------------------------------------------------------------*/
Vector3 Light::GetPosition( void)
{
	return m_pTransform->GetWorldPosition();
}

/*------------------------------------------------------------------------------
	�����擾�֐�
------------------------------------------------------------------------------*/
Vector3 Light::GetDirection( void)
{
	return m_pTransform->GetForward();
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void Light::Load(Text& text)
{
	//text��ǂݐi�߂�
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

		//text��ǂݐi�߂�
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	�Z�[�u
------------------------------------------------------------------------------*/
void Light::Save(Text& text)
{
	StartSave(text);

	text += "Type " + std::to_string((int)m_Type) + '\n';

	EndSave( text);
}