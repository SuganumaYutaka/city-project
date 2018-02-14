/*==============================================================================

    RenderManager.h - �`��i�����_�����O�j�Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/4
==============================================================================*/
#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Renderer;
class Camera;
class Light;
class DebugRenderer;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class RenderManager
{
public:
	RenderManager( HWND hWnd, BOOL bWindow);
	~RenderManager();

	void AddRenderer( Renderer *pRenderer);
	void ReleaseRenderer( Renderer *pRenderer);
	void Sort( void);
	void Draw( void);
	LPDIRECT3DDEVICE9 GetDevice( void) const { return m_pDevice;}
	
	void ClearRenderer( void) { m_listRenderer.clear(); m_listReleaseRenderer.clear(); m_listCamera.clear();}

	void SetMainCamera( Camera *pCamera) { m_pMainCamera = pCamera;}
	Camera* GetMainCamera( void) const { return m_pMainCamera;}
	std::list< Camera *> GetCameraList( void) const { return m_listCamera;}

	void SetCamera( Camera *pCamera) 
	{ 
		m_listCamera.push_back( pCamera);
		
		if (m_pMainCamera == NULL)
		{
			m_pMainCamera = pCamera;
		}
	}

	void ReleaseCamera(Camera *pCamera)
	{
		for (auto ite = m_listCamera.begin(); ite != m_listCamera.end(); ++ite)
		{
			if (*ite == pCamera)
			{
				m_listCamera.erase( ite);
				break;
			}
		}

		if (m_pMainCamera == pCamera)
		{
			m_pMainCamera = NULL;
		}
	}

private:
	LPDIRECT3D9 m_pD3D;								//Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pDevice;					//Device�I�u�W�F�N�g
	std::list< Renderer *> m_listRenderer;			//�����_���[���X�g
	std::list< Renderer *> m_listReleaseRenderer;	//�������郌���_���[���X�g
	Camera *m_pMainCamera;							//���C���J����

	std::list< Camera *> m_listCamera;				//�J�������X�g

	void DrawRenderer( Camera* camera, int layer);
	
#ifdef _DEBUG
	DebugRenderer *m_pDebugRenderer;				//�f�o�b�O�\��
#endif // _DEBUG
};

#endif

