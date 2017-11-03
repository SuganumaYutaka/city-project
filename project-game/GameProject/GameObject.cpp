/*==============================================================================
	
	GameObject.cpp - �I�u�W�F�N�g�m�[�h
														Author : Yutaka Suganuma
														Date   : 2017/4/29
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "GameObject.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�ÓI�����o�ϐ��錾
------------------------------------------------------------------------------*/
std::list<GameObject*> GameObject::m_listReleaseObject;

/*------------------------------------------------------------------------------
	�R���X�g���N�^
	����
		GameObject* pParent		�e�m�[�h
------------------------------------------------------------------------------*/
GameObject::GameObject( GameObject* pParent)
{
	InitGameObject( pParent);
}

/*------------------------------------------------------------------------------
	������
	����
		GameObject* pParent		�e�m�[�h
------------------------------------------------------------------------------*/
void GameObject::InitGameObject( GameObject* pParent)
{
	m_pParent = pParent;
	m_Tag = "UnTag";
	
	if (m_pParent != NULL)
	{
		m_pParent->AddChild( this);
		m_pRoot = m_pParent->m_pRoot;
	}
	else
	{
		m_pRoot = this;
	}

	m_listChild.clear();
	m_listComponent.clear();
	m_pTransform = AddComponent<Transform>();
	m_bRelease = false;
}

/*------------------------------------------------------------------------------
	�q�m�[�h�ǉ�
	����
		GameObject* pChild
------------------------------------------------------------------------------*/
void GameObject::AddChild( GameObject* pChild)
{
	m_listChild.push_back( pChild);
	this;
}

/*------------------------------------------------------------------------------
	�S�I�u�W�F�N�g�X�V
------------------------------------------------------------------------------*/
void GameObject::UpdateAll()
{
	Update();

	//�q���ċA�I�ɍX�V
	std::for_each( m_listChild.begin(), m_listChild.end(), 
		[]( GameObject* pChild) { pChild->UpdateAll();});
}

/*------------------------------------------------------------------------------
	�S�I�u�W�F�N�g�`��
------------------------------------------------------------------------------*/
void GameObject::DrawAll()
{
	Draw();

	//�q���ċA�I�ɕ`��
	std::for_each(m_listChild.begin(), m_listChild.end(), 
		[]( GameObject* pChild) { pChild->DrawAll();});
}

/*------------------------------------------------------------------------------
	�I�u�W�F�N�g�����\��
------------------------------------------------------------------------------*/
void GameObject::ReleaseReserve( void )
{
	if (m_bRelease == true)
	{
		return;
	}

	//���X�g�ɒǉ�
	m_listReleaseObject.push_back( this);

	m_bRelease = true;

	//�R���|�[�l���g�����s�s�\��
	for (Component* pComp : m_listComponent)
	{
		pComp->SetActive( false);
	}

	//�q���ċA�I�ɏ����\��
	std::list< GameObject*>::iterator ite;
	for( ite = m_listChild.begin(); ite != m_listChild.end(); ite++)
	{
		( *ite)->ReleaseReserveChildren();
	}
}

/*------------------------------------------------------------------------------
	�q�̃I�u�W�F�N�g�����\��
------------------------------------------------------------------------------*/
void GameObject::ReleaseReserveChildren( void )
{
	if( m_bRelease == true)
	{
		//���X�g�������
		std::list< GameObject*>::iterator ite;
		for( ite = m_listReleaseObject.begin(); ite != m_listReleaseObject.end(); ite++)
		{
			if( *ite == this)
			{
				ite = m_listReleaseObject.erase( ite);
				break;
			}
		}
	}

	m_bRelease = true;

	//�R���|�[�l���g�����s�s�\��
	for (Component* pComp : m_listComponent)
	{
		pComp->SetActive( false);
	}

	//�q���ċA�I�ɏ����\��
	std::list< GameObject*>::iterator ite;
	for( ite = m_listChild.begin(); ite != m_listChild.end(); ite++)
	{
		( *ite)->ReleaseReserveChildren();
	}
}

/*------------------------------------------------------------------------------
	���X�g�̏��ɃI�u�W�F�N�g����
------------------------------------------------------------------------------*/
void GameObject::ReleaseList(void)
{
	for(GameObject* Obj : m_listReleaseObject)
	{
		Obj->ReleaseObject();
	}

	//���X�g�����Z�b�g
	m_listReleaseObject.clear();
}

/*------------------------------------------------------------------------------
	�I�u�W�F�N�g�������s
------------------------------------------------------------------------------*/
void GameObject::ReleaseObject()
{
	//�e�̃��X�g�������
	std::list< GameObject*>::iterator ite;
	if( m_pParent != NULL && m_pParent->m_Tag != "Root")
	{
		for( ite = m_pParent->m_listChild.begin(); ite != m_pParent->m_listChild.end(); ite++)
		{
			//���X�g�������
			if( *ite == this)
			{
				ite = m_pParent->m_listChild.erase( ite);
				break;
			}
		}
	}

	ReleaseRecursive();
}

/*------------------------------------------------------------------------------
	�ċA�I�ɃI�u�W�F�N�g����
------------------------------------------------------------------------------*/
void GameObject::ReleaseRecursive()
{
	//�q���ċA�I�ɏ���
	std::list< GameObject*>::iterator ite;
	for( ite = m_listChild.begin(); ite != m_listChild.end(); ite++)
	{
		( *ite)->ReleaseRecursive();
	}

	//�R���|�[�l���g�I������
	for( auto pComp : m_listComponent)
	{
		pComp->Uninit();
	}

	//�R���|�[�l���g�̏���
	std::list< Component *>::iterator iteComp;
	for( iteComp = m_listComponent.begin(); iteComp != m_listComponent.end(); iteComp++)
	{
		delete *iteComp;
	}

	//���g�̏����i�K���Ō�Ɂj
	delete this;
}

/*------------------------------------------------------------------------------
	�R���|�[�l���g������
------------------------------------------------------------------------------*/
void GameObject::ReleaseComponent(Component* pComp)
{
	//�R���|�[�l���g�̏���
	for (auto comp : m_listComponent)
	{
		if( comp == pComp)
		{
			pComp->Uninit();
			delete pComp;
			break;
		}
	}

	//���X�g�������
	std::list< Component *>::iterator iteComp;
	for( iteComp = m_listComponent.begin(); iteComp != m_listComponent.end(); iteComp++)
	{
		if( *iteComp == pComp)
		{
			iteComp = m_listComponent.erase( iteComp);
			break;
		}
	}
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void GameObject::Update()
{
	//�R���|�[�l���g�̍X�V
	for (Component *pComponent : m_listComponent)
	{
		if ( pComponent->GetActive() == true)
		{
			pComponent->Update();
		}
	}

	/*std::for_each(m_listComponent.begin(), m_listComponent.end(),
		[]( Component* pComponent) { pComponent->Update();});*/
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void GameObject::Draw()
{
	////�R���|�[�l���g�̕`��iRenderManager�ŊǗ�����j
	//std::for_each(m_listComponent.begin(), m_listComponent.end(), 
	//	[]( Component* pComponent) { pComponent->Draw();});
}

/*------------------------------------------------------------------------------
	�Փˎ��̏���
------------------------------------------------------------------------------*/
void GameObject::OnCollision(Collider *pCollider)
{
	//�R���|�[�l���g�ɃR�[��
	for (Component* pComp : m_listComponent)
	{
		pComp->OnCollision( pCollider);
	}
}

