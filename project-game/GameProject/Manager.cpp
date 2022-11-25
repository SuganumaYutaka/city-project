/*==============================================================================
	
	Manager.cpp - ゲーム全体の管理
														Author : Yutaka Suganuma
														Date   : 2017/5/10
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
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
	静的メンバ変数宣言
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
	コンストラクタ
------------------------------------------------------------------------------*/
Manager::Manager( HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//引数の保存
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	//ランダム関数初期化
	srand((unsigned)time(NULL));

	//レンダラー
	m_pRenderManager = new RenderManager( hWnd, bWindow);

	//キーボード
	m_pInputKeyboard = new InputKeyboard( hInstance, hWnd);

	//マウス
	m_pInputMouse = new InputMouse( hInstance, hWnd);

	//テクスチャマネージャー
	m_pTextureManager = new TextureManager();

	//Xモデルマネージャー
	m_pXModelManager = new XModelManager();

	//衝突判定マネージャー
	m_pCollisionManager = new CollisionManager();

	//サウンドマネージャー
	m_pSoundManager = new SoundManager( hWnd);

	//シェーダーマネージャー
	m_pShaderManager = new ShaderManager();

	//コンポーネントファクトリー
	m_pComponentFactory = new ComponentFactory();

	//モード
	ChangeMode( new ModeGame());
	m_pNextMode = NULL;
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Manager::~Manager()
{
	//モードの破棄
	int Size = m_stackMode.size();
	for (int nCnt = 0; nCnt < Size; nCnt++)
	{
		m_stackMode.top()->Uninit();
		GameObject::ReleaseList();

		delete m_stackMode.top();
		m_stackMode.pop();
	}

	//シェーダーマネージャーの破棄
	if (m_pShaderManager != NULL)
	{
		delete m_pShaderManager;
		m_pShaderManager = NULL;
	}
	
	//Xモデルマネージャーの破棄
	if( m_pXModelManager != NULL)
	{
		delete m_pXModelManager;
		m_pXModelManager = NULL;
	}

	//テクスチャマネージャーの破棄
	if( m_pTextureManager != NULL)
	{
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	//レンダラーの破棄
	if( m_pRenderManager != NULL)
	{
		delete m_pRenderManager;
		m_pRenderManager = NULL;
	}

	//衝突判定マネージャーの破棄
	if( m_pCollisionManager != NULL)
	{
		delete m_pCollisionManager;
		m_pCollisionManager = NULL;
	}

	//サウンドマネージャーの破棄
	if( m_pSoundManager != NULL)
	{
		delete m_pSoundManager;
		m_pSoundManager = NULL;
	}

	//キーボードの破棄
	if( m_pInputKeyboard != NULL)
	{
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//マウスの破棄
	if( m_pInputMouse != NULL)
	{
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	//コンポーネント生成ファクトリーの破棄
	if (m_pComponentFactory != NULL)
	{
		delete m_pComponentFactory;
		m_pComponentFactory = NULL;
	}

	//デバッグログ保存
	DebugLog::Save();
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void Manager::Update(void)
{
	m_pInputKeyboard->Update( m_hWnd);		//キーボードの更新
	m_pInputMouse->Update( m_hWnd);			//マウスの更新
	m_stackMode.top()->Update();			//モードの更新
	m_pCollisionManager->Collision();		//衝突判定
	Component::ReleaseList();				//コンポーネントの消去
	GameObject::ReleaseList();				//オブジェクトの消去
	DebugLog::SetImGuiView();				//デバッグログ出力

	if (m_pNextMode != NULL)
	{
		ChangeMode( m_pNextMode);
		m_pNextMode = NULL;
	}
}

/*------------------------------------------------------------------------------
	描画処理
------------------------------------------------------------------------------*/
void Manager::Draw(void)
{
	m_pRenderManager->Draw();
}

/*------------------------------------------------------------------------------
	デバイスの取得(static)
	戻り値
		LPDIRECT3DDEVICE9
------------------------------------------------------------------------------*/
LPDIRECT3DDEVICE9 Manager::GetDevice( void)
{
	if( m_pRenderManager != NULL)
	{
		return m_pRenderManager->GetDevice();
	}

	MessageBox(NULL, "ManagerにDeviceが登録されていません", "エラー", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	レンダーマネージャーポインタの取得(static)
	戻り値
		RenderManager *
------------------------------------------------------------------------------*/
RenderManager *Manager::GetRenderManager( void)
{
	if( m_pRenderManager != NULL)
	{
		return m_pRenderManager;
	}

	MessageBox(NULL, "ManagerにRenderManagerが登録されていません", "エラー", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	テクスチャマネージャーポインタの取得(static)
	戻り値
		TextureManager *
------------------------------------------------------------------------------*/
TextureManager *Manager::GetTextureManager( void)
{
	if( m_pTextureManager != NULL)
	{
		return m_pTextureManager;
	}

	MessageBox(NULL, "ManagerにTextureManagerが登録されていません", "エラー", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	Xモデルマネージャーポインタの取得(static)
	戻り値
		XModelManager *
------------------------------------------------------------------------------*/
XModelManager *Manager::GetXModelManager( void)
{
	if( m_pXModelManager != NULL)
	{
		return m_pXModelManager;
	}

	MessageBox(NULL, "ManagerにXModelManagerが登録されていません", "エラー", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	キーボードポインタの取得(static)
	戻り値
		InputKeyboard *
------------------------------------------------------------------------------*/
InputKeyboard *Manager::GetInputKeyboard( void)
{
	if( m_pInputKeyboard != NULL)
	{
		return m_pInputKeyboard;
	}

	MessageBox(NULL, "ManagerにInputKeyboardが登録されていません", "エラー", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	マウスポインタの取得(static)
	戻り値
		InputMouse *
------------------------------------------------------------------------------*/
InputMouse *Manager::GetInputMouse( void)
{
	if( m_pInputMouse != NULL)
	{
		return m_pInputMouse;
	}

	MessageBox(NULL, "ManagerにInputMouseが登録されていません", "エラー", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	衝突判定マネージャーポインタの取得(static)
	戻り値
		CollisionManager *
------------------------------------------------------------------------------*/
CollisionManager *Manager::GetCollisionManager( void)
{
	if( m_pCollisionManager != NULL)
	{
		return m_pCollisionManager;
	}

	MessageBox(NULL, "ManagerにCollisionManagerが登録されていません", "エラー", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	サウンドマネージャーポインタの取得(static)
	戻り値
		SoundManager *
------------------------------------------------------------------------------*/
SoundManager *Manager::GetSoundManager( void)
{
	if( m_pSoundManager != NULL)
	{
		return m_pSoundManager;
	}

	MessageBox(NULL, "ManagerにSoundManagerが登録されていません", "エラー", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	シェーダーマネージャーポインタの取得(static)
	戻り値
		ShaderManager *
------------------------------------------------------------------------------*/
ShaderManager *Manager::GetShaderManager( void)
{
	if( m_pShaderManager != NULL)
	{
		return m_pShaderManager;
	}

	MessageBox(NULL, "ManagerにShaderManagerが登録されていません", "エラー", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	コンポーネント生成ファクトリーポインタの取得(static)
	戻り値
		ComponentFactory *
------------------------------------------------------------------------------*/
ComponentFactory *Manager::GetComponentFactory( void)
{
	if( m_pComponentFactory != NULL)
	{
		return m_pComponentFactory;
	}

	MessageBox(NULL, "ManagerにComponentFactoryが登録されていません", "エラー", NULL);
	return NULL;
}

/*------------------------------------------------------------------------------
	モードの設定
	引数
		Mode *pNextMode
------------------------------------------------------------------------------*/
void Manager::SetMode( Mode *pNextMode)
{
	m_pNextMode = pNextMode;

	m_stackMode.top()->Uninit();
}

/*------------------------------------------------------------------------------
	モード切り替え
	引数
		Mode *pNextMode
------------------------------------------------------------------------------*/
void Manager::ChangeMode( Mode *pNextMode)
{
	//前モードの終了
	if( m_stackMode.size() > 0)
	{
		delete m_stackMode.top();
		m_stackMode.pop();
	}
	m_pCollisionManager->ClearCollider();
	m_pRenderManager->ClearRenderer();

	//モードの変更
	m_stackMode.push( pNextMode);

	//次モードの開始
	if (pNextMode != NULL)
	{
		m_stackMode.top()->Init();
	}

	pNextMode = false;
}

/*------------------------------------------------------------------------------
	モードの追加
	引数
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

	//モードの追加
	m_stackMode.push( pNextMode);

	//モードの開始
	pNextMode->Init();
}

/*------------------------------------------------------------------------------
	モードの破棄
------------------------------------------------------------------------------*/
void Manager::PopMode( void)
{
	if (m_stackMode.size() <= 1)
	{
		return;
	}

	//モードの終了
	delete m_stackMode.top();
	m_stackMode.pop();
}
