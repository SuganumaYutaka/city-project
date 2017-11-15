/*==============================================================================

    RenderManager.cpp - �`��i�����_�����O�j�Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/4
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "RenderManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "DebugRenderer.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
	����
		HWND hWnd
		BOOL bWindow
------------------------------------------------------------------------------*/
RenderManager::RenderManager( HWND hWnd, BOOL bWindow)
{
	//Direct3D�C���^�[�t�F�C�X�̎擾
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION);
	if( m_pD3D == NULL){
		MessageBox( NULL, "main.cpp\nDirect3D�C���^�[�t�F�C�X�̎擾�Ɏ��s���܂���\n", "�G���[", MB_OK);
		assert( false);
		return;
	}
	if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm))){
		MessageBox( NULL, "main.cpp\nDirect3D�C���^�[�t�F�C�X�̎擾�Ɏ��s���܂���\n", "�G���[", MB_OK);
		assert( false);
		return;
	}

	//�f�o�C�X�̃v���[���g�p�����[�^�[�̐ݒ�i�f�o�C�X����邽�߂̐ݒ��ݒ�j
	ZeroMemory( &d3dpp, sizeof( d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//�X�e���V���o�b�t�@�i3�N���ɂ�����ҏW����
	d3dpp.Windowed = TRUE;					//�t���X�N���[���ɂ���ەύX
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//��Direct3D�f�o�C�X�̍쐬
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice))){
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice))){
			if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice))){
				MessageBox( NULL, "main.cpp\nDirect3D�f�o�C�X�̍쐬�Ɏ��s���܂���\n", "�G���[", MB_OK);
				assert( false);
				return;
			}
		}
	}

	//�����_�[�X�e�[�g�i�`��ݒ�j
	//���ʃJ�����O�ݒ�
	m_pDevice->SetRenderState( D3DRS_CULLMODE,			//���ʃJ�����O�ݒ�
									D3DCULL_CCW);			//�t���v��肪����
	//�h��Ԃ��ݒ�
	m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);			//�f�t�H���g
	//m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//���C���[�t���[���\��

	//�[�x�o�b�t�@���\��
	m_pDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE);

	//�f�o�b�O�\��
#ifdef _DEBUG
	m_pDebugRenderer = new DebugRenderer( m_pDevice);
#endif // _DEBUG

	m_pMainCamera = NULL;
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
RenderManager::~RenderManager()
{
	//�f�o�b�O�\��
#ifdef _DEBUG
	if (m_pDebugRenderer != NULL)
	{
		delete m_pDebugRenderer;
		m_pDebugRenderer = NULL;
	}
#endif // _DEBUG

	//Direct3D�f�o�C�X�̉��
	if( m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//Direct3D�C���^�[�t�F�C�X�̉��
	if( m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

/*------------------------------------------------------------------------------
	�����_���[���X�g�ɒǉ�
	����
		Renderer *pRenderer			�����_���[
------------------------------------------------------------------------------*/
void RenderManager::AddRenderer( Renderer *pRenderer)
{
	m_listRenderer.push_back( pRenderer);
}

/*------------------------------------------------------------------------------
	�����_���[���X�g����폜
	����
		Renderer *pRenderer			�����_���[
------------------------------------------------------------------------------*/
void RenderManager::ReleaseRenderer( Renderer *pRenderer)
{
	m_listReleaseRenderer.push_back( pRenderer);
}

/*------------------------------------------------------------------------------
	�����_���[���X�g���\�[�g
------------------------------------------------------------------------------*/
void RenderManager::Sort()
{
	
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void RenderManager::Draw()
{
	//�����_���[���X�g����폜
	for (auto Renderer : m_listReleaseRenderer)
	{
		for (auto ite = m_listRenderer.begin(); ite != m_listRenderer.end(); ++ite)
		{
			if( *ite == Renderer)
			{
				m_listRenderer.erase(ite);
				break;
			}
		}
	}
	m_listReleaseRenderer.clear();

	//�\�[�g
	Sort();

	//���C���J���������邩�`�F�b�N
	if (m_pMainCamera == NULL)
	{
		MessageBox( NULL, "���C���J����������܂���\n", "�G���[", MB_OK);
		return;
	}

	//�J������ɂ����`��
	for (auto pCamera : m_listCamera)
	{
		//�J�������A�N�e�B�u��
		if (pCamera->GetActive() == false)
		{
			continue;
		}

		//���C���J�����͍Ō�ɕ`��
		if (pCamera == m_pMainCamera)
		{
			continue;
		}

		//�J�����̃Z�b�g�i�r���[�|�[�g�A�����_�[�^�[�Q�b�g�A�[�x�o�b�t�@�j
		pCamera->SetCamera();

		//�`��̊J�n
		if( SUCCEEDED( m_pDevice->BeginScene()))
		{
			//�`�悷�郌�C���[���擾
			auto List = pCamera->GetRenderLayer();

			//���ׂĕ`��
			if (List.size() == 0)
			{
				//���C���[���Ƃɕ`��
				for (int nCntLayer = 0; nCntLayer < eLayerNum; nCntLayer++)
				{
					//�o�^���ꂽ�����_���[��`��
					for (Renderer* pRenderer : m_listRenderer)
					{
						if (pRenderer->m_nLayer == nCntLayer)
						{
							if( pRenderer->GetActive() == true)
							{
								pRenderer->Draw( pCamera);
							}
						}
					}
				}
			}

			//����̃��C���[�̂ݕ`��
			else
			{
				//�o�^���ꂽ���C���[�̏��ɕ`��
				for (auto nLayer : List)
				{
					//�o�^���ꂽ�����_���[��`��
					for (Renderer* pRenderer : m_listRenderer)
					{
						if (pRenderer->m_nLayer == nLayer)
						{
							if( pRenderer->GetActive() == true)
							{
								pRenderer->Draw( pCamera);
							}
						}
					}
				}
			}

			//�`��̏I��
			m_pDevice->EndScene();	//�P�t���[���ɂP��܂�
		}
	}

	//���C���J�����̕`��
	//�J�����̃Z�b�g�i�r���[�|�[�g�A�����_�[�^�[�Q�b�g�A�[�x�o�b�t�@�j
	m_pMainCamera->SetCamera();
	
	//���(�o�b�N�o�b�t�@)�̃N���A
	m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 255, 150, 0, 255), 1.0f, 0);

	//�`��̊J�n
	if( SUCCEEDED( m_pDevice->BeginScene()))
	{
		//���C���[���Ƃɕ`��
		for (int nCntLayer = 0; nCntLayer < eLayerNum; nCntLayer++)
		{
			//�o�^���ꂽ�����_���[��`��
			for (Renderer* pRenderer : m_listRenderer)
			{
				if (pRenderer->m_nLayer == nCntLayer)
				{
					if( pRenderer->GetActive() == true)
					{
						pRenderer->Draw( m_pMainCamera);
					}
				}
			}
		}

		//�f�o�b�O�\��
#ifdef _DEBUG
		m_pDebugRenderer->DrawText();
		DebugRenderer::ClearText();
#endif // _DEBUG

		ImGui::Render();

		//�`��̏I��
		m_pDevice->EndScene();	//�P�t���[���ɂP��܂�
	}

	//�t���b�v�i�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ��j
	m_pDevice->Present( NULL, NULL, NULL, NULL);
}


