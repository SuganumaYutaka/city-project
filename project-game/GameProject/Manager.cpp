/*==============================================================================
	
	Manager.cpp - �Q�[���S�̂̊Ǘ�
														Author : Yutaka Suganuma
														Date   : 2017/5/10
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include <time.h>
#include "RenderManager.h"
#include "TextureManager.h"
#include "XModelManager.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "ShaderManager.h"
#include "Mode.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "ComponentFactory.h"
#include "ModeGame.h"
#include "ModeTitle.h"

/*------------------------------------------------------------------------------
	�ÓI�����o�ϐ��錾
------------------------------------------------------------------------------*/
RenderManager *Manager::m_pRenderManager = NULL;		
TextureManager *Manager::m_pTextureManager = NULL;
XModelManager *Manager::m_pXModelManager = NULL;	
InputKeyboard *Manager::m_pInputKeyboard = NULL;		
InputMouse *Manager::m_pInputMouse = NULL;			
CollisionManager *Manager::m_pCollisionManager = NULL;
SoundManager *Manager::m_pSoundManager = NULL;
ShaderManager *Manager::m_pShaderManager = NULL;
ComponentFactory *Manager::m_pComponentFactory = NULL;
std::stack<Mode*> Manager::m_stackMode;
Mode *Manager::m_pNextMode = NULL;

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Manager::Manager( HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//�����̕ۑ�
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	//�����_���֐�������
	srand((unsigned)time(NULL));

	//�����_���[
	m_pRenderManager = new RenderManager( hWnd, bWindow);

	//�L�[�{�[�h
	m_pInputKeyboard = new InputKeyboard( hInstance, hWnd);

	//�}�E�X
	m_pInputMouse = new InputMouse( hInstance, hWnd);

	//�e�N�X�`���}�l�[�W���[
	m_pTextureManager = new TextureManager();

	//X���f���}�l�[�W���[
	m_pXModelManager = new XModelManager();

	//�Փ˔���}�l�[�W���[
	m_pCollisionManager = new CollisionManager();

	//�T�E���h�}�l�[�W���[
	m_pSoundManager = new SoundManager( hWnd);

	//�V�F�[�_�[�}�l�[�W���[
	m_pShaderManager = new ShaderManager();

	//�R���|�[�l���g�t�@�N�g���[
	m_pComponentFactory = new ComponentFactory();

	//���[�h
	ChangeMode( new ModeGame());
	m_pNextMode = NULL;
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Manager::~Manager()
{
	//���[�h�̔j��
	int Size = m_stackMode.size();
	for (int nCnt = 0; nCnt < Size; nCnt++)
	{
		m_stackMode.top()->Uninit();
		GameObject::ReleaseList();

		delete m_stackMode.top();
		m_stackMode.pop();
	}

	//�V�F�[�_�[�}�l�[�W���[�̔j��
	if (m_pShaderManager != NULL)
	{
		delete m_pShaderManager;
		m_pShaderManager = NULL;
	}
	
	//X���f���}�l�[�W���[�̔j��
	if( m_pXModelManager != NULL)
	{
		delete m_pXModelManager;
		m_pXModelManager = NULL;
	}

	//�e�N�X�`���}�l�[�W���[�̔j��
	if( m_pTextureManager != NULL)
	{
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	//�����_���[�̔j��
	if( m_pRenderManager != NULL)
	{
		delete m_pRenderManager;
		m_pRenderManager = NULL;
	}

	//�Փ˔���}�l�[�W���[�̔j��
	if( m_pCollisionManager != NULL)
	{
		delete m_pCollisionManager;
		m_pCollisionManager = NULL;
	}

	//�T�E���h�}�l�[�W���[�̔j��
	if( m_pSoundManager != NULL)
	{
		delete m_pSoundManager;
		m_pSoundManager = NULL;
	}

	//�L�[�{�[�h�̔j��
	if( m_pInputKeyboard != NULL)
	{
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//�}�E�X�̔j��
	if( m_pInputMouse != NULL)
	{
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	//�R���|�[�l���g�����t�@�N�g���[�̔j��
	if (m_pComponentFactory != NULL)
	{
		delete m_pComponentFactory;
		m_pComponentFactory = NULL;
	}

	//�f�o�b�O���O�ۑ�
	DebugLog::Save();
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void Manager::Update(void)
{
	m_pInputKeyboard->Update( m_hWnd);		//�L�[�{�[�h�̍X�V
	m_pInputMouse->Update( m_hWnd);			//�}�E�X�̍X�V
	m_stackMode.top()->Update();			//���[�h�̍X�V
	m_pCollisionManager->Collision();		//�Փ˔���
	Component::ReleaseList();				//�R���|�[�l���g�̏���
	GameObject::ReleaseList();				//�I�u�W�F�N�g�̏���
	DebugLog::SetImGuiView();				//�f�o�b�O���O�o��

	if (m_pNextMode != NULL)
	{
		ChangeMode( m_pNextMode);
		m_pNextMode = NULL;
	}
}

/*------------------------------------------------------------------------------
	�`�揈��
------------------------------------------------------------------------------*/
void Manager::Draw(void)
{
	m_pRenderManager->Draw();
}

/*------------------------------------------------------------------------------
	�f�o�C�X�̎擾(static)
	�߂�l
		LPDIRECT3DDEVICE9
------------------------------------------------------------------------------*/
LPDIRECT3DDEVICE9 Manager::GetDevice( void)
{
	if( m_pRenderManager != NULL)
	{
		return m_pRenderManager->GetDevice();
	}

	MessageBox(NULL, "Manager��Device���o�^����Ă��܂���", "�G���[", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	�����_�[�}�l�[�W���[�|�C���^�̎擾(static)
	�߂�l
		RenderManager *
------------------------------------------------------------------------------*/
RenderManager *Manager::GetRenderManager( void)
{
	if( m_pRenderManager != NULL)
	{
		return m_pRenderManager;
	}

	MessageBox(NULL, "Manager��RenderManager���o�^����Ă��܂���", "�G���[", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	�e�N�X�`���}�l�[�W���[�|�C���^�̎擾(static)
	�߂�l
		TextureManager *
------------------------------------------------------------------------------*/
TextureManager *Manager::GetTextureManager( void)
{
	if( m_pTextureManager != NULL)
	{
		return m_pTextureManager;
	}

	MessageBox(NULL, "Manager��TextureManager���o�^����Ă��܂���", "�G���[", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	X���f���}�l�[�W���[�|�C���^�̎擾(static)
	�߂�l
		XModelManager *
------------------------------------------------------------------------------*/
XModelManager *Manager::GetXModelManager( void)
{
	if( m_pXModelManager != NULL)
	{
		return m_pXModelManager;
	}

	MessageBox(NULL, "Manager��XModelManager���o�^����Ă��܂���", "�G���[", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	�L�[�{�[�h�|�C���^�̎擾(static)
	�߂�l
		InputKeyboard *
------------------------------------------------------------------------------*/
InputKeyboard *Manager::GetInputKeyboard( void)
{
	if( m_pInputKeyboard != NULL)
	{
		return m_pInputKeyboard;
	}

	MessageBox(NULL, "Manager��InputKeyboard���o�^����Ă��܂���", "�G���[", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	�}�E�X�|�C���^�̎擾(static)
	�߂�l
		InputMouse *
------------------------------------------------------------------------------*/
InputMouse *Manager::GetInputMouse( void)
{
	if( m_pInputMouse != NULL)
	{
		return m_pInputMouse;
	}

	MessageBox(NULL, "Manager��InputMouse���o�^����Ă��܂���", "�G���[", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	�Փ˔���}�l�[�W���[�|�C���^�̎擾(static)
	�߂�l
		CollisionManager *
------------------------------------------------------------------------------*/
CollisionManager *Manager::GetCollisionManager( void)
{
	if( m_pCollisionManager != NULL)
	{
		return m_pCollisionManager;
	}

	MessageBox(NULL, "Manager��CollisionManager���o�^����Ă��܂���", "�G���[", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	�T�E���h�}�l�[�W���[�|�C���^�̎擾(static)
	�߂�l
		SoundManager *
------------------------------------------------------------------------------*/
SoundManager *Manager::GetSoundManager( void)
{
	if( m_pSoundManager != NULL)
	{
		return m_pSoundManager;
	}

	MessageBox(NULL, "Manager��SoundManager���o�^����Ă��܂���", "�G���[", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�}�l�[�W���[�|�C���^�̎擾(static)
	�߂�l
		ShaderManager *
------------------------------------------------------------------------------*/
ShaderManager *Manager::GetShaderManager( void)
{
	if( m_pShaderManager != NULL)
	{
		return m_pShaderManager;
	}

	MessageBox(NULL, "Manager��ShaderManager���o�^����Ă��܂���", "�G���[", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	�R���|�[�l���g�����t�@�N�g���[�|�C���^�̎擾(static)
	�߂�l
		ComponentFactory *
------------------------------------------------------------------------------*/
ComponentFactory *Manager::GetComponentFactory( void)
{
	if( m_pComponentFactory != NULL)
	{
		return m_pComponentFactory;
	}

	MessageBox(NULL, "Manager��ComponentFactory���o�^����Ă��܂���", "�G���[", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	���[�h�̐ݒ�
	����
		Mode *pNextMode
------------------------------------------------------------------------------*/
void Manager::SetMode( Mode *pNextMode)
{
	m_pNextMode = pNextMode;

	m_stackMode.top()->Uninit();
}

/*------------------------------------------------------------------------------
	���[�h�؂�ւ�
	����
		Mode *pNextMode
------------------------------------------------------------------------------*/
void Manager::ChangeMode( Mode *pNextMode)
{
	//�O���[�h�̏I��
	if( m_stackMode.size() > 0)
	{
		delete m_stackMode.top();
		m_stackMode.pop();
	}
	m_pCollisionManager->ClearCollider();
	m_pRenderManager->ClearRenderer();

	//���[�h�̕ύX
	m_stackMode.push( pNextMode);

	//�����[�h�̊J�n
	if (pNextMode != NULL)
	{
		m_stackMode.top()->Init();
	}

	pNextMode = false;
}

/*------------------------------------------------------------------------------
	���[�h�̒ǉ�
	����
		Mode *pNextMode
------------------------------------------------------------------------------*/
void Manager::PushMode( Mode *pNextMode)
{
	if (pNextMode == m_stackMode.top())
	{
		return;
	}

	m_pCollisionManager->ClearCollider();
	m_pRenderManager->ClearRenderer();

	//���[�h�̒ǉ�
	m_stackMode.push( pNextMode);

	//���[�h�̊J�n
	pNextMode->Init();
}

/*------------------------------------------------------------------------------
	���[�h�̔j��
------------------------------------------------------------------------------*/
void Manager::PopMode( void)
{
	if (m_stackMode.size() <= 1)
	{
		return;
	}

	//���[�h�̏I��
	delete m_stackMode.top();
	m_stackMode.pop();
}
