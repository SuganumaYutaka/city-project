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
D3DXVECTOR4* Light::GetPosition( void)
{
	auto Pos = m_pTransform->GetWorldPosition();
	return &D3DXVECTOR4( Pos.x, Pos.y, Pos.z, 0.0f);
}

/*------------------------------------------------------------------------------
	�����擾�֐�
------------------------------------------------------------------------------*/
D3DXVECTOR4* Light::GetDirection( void)
{
	Vector3 Dir = m_pTransform->GetForward();
	return &D3DXVECTOR4( Dir.x, Dir.y, Dir.z, 0.0f);
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