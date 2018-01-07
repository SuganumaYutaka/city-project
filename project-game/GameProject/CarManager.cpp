/*==============================================================================
	
	CarManager.cpp - ��ʃV�X�e���[�ԃ}�l�[�W���[
														Author : Yutaka Suganuma
														Date   : 2017/12/28
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "CarManager.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "CarController.h"
#include "TrafficRoad.h"
#include "TrafficJunction.h"
#include "TrafficBuilding.h"

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* CarManager::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<CarManager>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
CarManager::CarManager( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();


}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void CarManager::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void CarManager::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	���ׂĂ�CarController���擾
------------------------------------------------------------------------------*/
std::list<CarController*> CarManager::GetAllCarControllers(void)
{
	return m_pGameObject->GetComponentListInChildren<CarController>();
}

/*------------------------------------------------------------------------------
	CarController����
------------------------------------------------------------------------------*/
CarController* CarManager::CreateCarController(const Vector3& position, const D3DXQUATERNION& rotation
	, TrafficRoad* currentRoad, TrafficJunction* nextJunction, TrafficBuilding* targetBuilding)
{
	auto carObject = new GameObject( m_pGameObject);
	carObject->m_pTransform->SetWorldPosition( position);
	carObject->m_pTransform->SetWorldRotation( rotation);
	auto carController = carObject->AddComponent<CarController>();
	carController->Init( currentRoad, nextJunction, targetBuilding);

	return carController;
}
