/*==============================================================================

    Camera.h - �J����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/
#ifndef _CAMERA_H_
#define _CAMERA_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class RenderTexture;

/*------------------------------------------------------------------------------
	�񋓌^��`
------------------------------------------------------------------------------*/
typedef enum
{
	eCameraDefault = 0,
	eCameraLight,
	eNumCameraType
}ECameraType;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Camera : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	Camera( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void SetCamera( void);

	static Camera* GetMainCamera();
	void SetMainCamera( void);
	bool IsMainCamera( void);

	void SetPosAt( const Vector3& Pos) { m_PosAt = Pos;}
	Vector3 GetPosAt( void) const { return m_PosAt;}
	void SetVecUp( const Vector3& Vec) { m_VecUp = Vec;}
	Vector3 GetVecUp( void) const { return m_VecUp;}
	Vector3 GetCameraVec( void) const;

	float GetNear( void){ return m_fNear;}
	float GetFar( void){ return m_fFar;}
	
	D3DXMATRIX* GetViewMatrix( void){ return &m_mtxView;}
	D3DXMATRIX* GetProjectionMatrix( void){ return &m_mtxProj;}
	D3DVIEWPORT9* GetViewPort( void) {return &m_ViewPort;}
	void SetViewPort(DWORD X, DWORD Y, DWORD Width, DWORD Height, float MinZ, float MaxZ){ m_ViewPort = { X, Y, Width, Height, MinZ, MaxZ};}

	RenderTexture* GetRenderTarget( void) { return m_RenderTarget;}
	void SetRenderTarget( std::string TextureName, bool isBuckBuffer);

	std::list<int> GetRenderLayer( void) { return m_listRenderLayer;}
	void ClearRenderLayer( void) { m_listRenderLayer.clear(); }
	void SetRenderLayer(int nLayer)
	{
		for (auto layer : m_listRenderLayer)
		{
			if (nLayer == layer)
			{
				return;
			}
		}

		m_listRenderLayer.push_back( nLayer);
	}
	void SetType( ECameraType Type) { m_Type = Type;}

	void Move( const Vector3& Value);

	void SetNear( float Near);
	void SetFar( float Far);

	void SetPerspective( float FovY, float Aspect, float Near, float Far);
	void SetOrtho( float Width, float Height, float Near, float Far);

	Ray CalcScreenPointToRay( Vector2 screenPoint);

	bool ChangeLightCameraFormat( Vector3 fieldSize);

	virtual void Save( Text& text);
	virtual void Load( Text& text);

private:
	Vector3 m_PosAt;					//�����_�i���[���h���W�j
	Vector3 m_VecUp;					//�J�����̏����
	float m_fFovY;						//��p�i����p�j
	float m_fAspect;					//�A�X�y�N�g��
	float m_fNear;						//�j�A
	float m_fFar;						//�t�@�[
	
	D3DXMATRIX m_mtxView;				//�r���[���W�ϊ��s��
	D3DXMATRIX m_mtxProj;				//�v���W�F�N�V�������W�ϊ��s��
	D3DVIEWPORT9 m_ViewPort;			//�r���[�|�[�g
	RenderTexture* m_RenderTarget;		//�����_�[�o�͐�
	std::list<int> m_listRenderLayer;	//�����_�����O���郌�C���[�isize��0�Ȃ炷�ׂĕ`��j
	ECameraType m_Type;					//�J�����̎��

	Vector3 CalcScreenPointToWorldPosition( Vector2 screenPoint, float positionZ);
};

#endif

