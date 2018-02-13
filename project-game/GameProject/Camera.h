/*==============================================================================

    Camera.h - カメラ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/
#ifndef _CAMERA_H_
#define _CAMERA_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class RenderTexture;

/*------------------------------------------------------------------------------
	列挙型定義
------------------------------------------------------------------------------*/
typedef enum
{
	eCameraDefault = 0,
	eCameraLight,
	eNumCameraType
}ECameraType;

/*------------------------------------------------------------------------------
	クラス定義
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
	Vector3 m_PosAt;					//注視点（ワールド座標）
	Vector3 m_VecUp;					//カメラの上方向
	float m_fFovY;						//画角（視野角）
	float m_fAspect;					//アスペクト比
	float m_fNear;						//ニア
	float m_fFar;						//ファー
	
	D3DXMATRIX m_mtxView;				//ビュー座標変換行列
	D3DXMATRIX m_mtxProj;				//プロジェクション座標変換行列
	D3DVIEWPORT9 m_ViewPort;			//ビューポート
	RenderTexture* m_RenderTarget;		//レンダー出力先
	std::list<int> m_listRenderLayer;	//レンダリングするレイヤー（sizeが0ならすべて描画）
	ECameraType m_Type;					//カメラの種類

	Vector3 CalcScreenPointToWorldPosition( Vector2 screenPoint, float positionZ);
};

#endif

