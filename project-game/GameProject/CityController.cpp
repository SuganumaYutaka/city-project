/*==============================================================================
	
	CityController.cpp - ���̎��������[�R���g���[��
														Author : Yutaka Suganuma
														Date   : 2017/12/1
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "CityController.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "HalfEdgeModel.h"
#include "CityRule.h"
#include "CityAttribute.h"

#include "InputKeyboard.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define CITY_WIDTH (500.0f)
#define CITY_HEIGHT (500.0f)


/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* CityController::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<CityController>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
CityController::CityController( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	//���̎��������V�X�e���ݒ�̃n�[�t�G�b�W�f�[�^�\���𐶐�
	m_Model = new Model( new CityRule(), new CityAttributeFactory( m_pGameObject));

	//�͂��߂̖ʂ𐶐�
	Vector3 sizeHalf( CITY_WIDTH * 0.5f, 0.0f, CITY_HEIGHT * 0.5f);
	m_Model->CreateFirstFace( 
		Vector3( -sizeHalf.x, 0.0f, +sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, +sizeHalf.z),
		Vector3( -sizeHalf.x, 0.0f, -sizeHalf.z), Vector3( +sizeHalf.x, 0.0f, -sizeHalf.z));
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void CityController::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void CityController::Update()
{
	if (Manager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		m_Model->DivideAllFaces();
	}
}
