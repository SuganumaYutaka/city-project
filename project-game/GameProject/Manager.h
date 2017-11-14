/*==============================================================================
	
	Manager.h - ゲーム全体の管理
														Author : Yutaka Suganuma
														Date   : 2017/5/10
==============================================================================*/
#ifndef _MANAGER_H_
#define _MANAGER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include <Windows.h>
#include <stdio.h>
#include <typeinfo>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <fstream>

//DirectX9
#include <d3dx9.h>	//<d3d9.h>も含まれる
#define DIRECTINPUT_VERSION (0x0800)		//この組み合わせで順番守るように
#include <dinput.h>	//入力
#include <XAudio2.h>	//サウンド

//boost
//serialization
//#include <boost/serialization/export.hpp>
//#include <boost/serialization/string.hpp>
//#include <boost/serialization/vector.hpp>
//#include <boost/serialization/list.hpp>
//#include <boost/serialization/map.hpp>
//#include <boost/serialization/unordered_map.hpp>
//#include <boost/archive/xml_oarchive.hpp>
//#include <boost/archive/xml_iarchive.hpp>
//#include <boost/serialization/nvp.hpp>
//#include <boost/serialization/base_object.hpp>

//汎用クラス
#include "Vector2.h"
#include "Vector3.h"
#include "Text.h"
#include "TextureUV.h"
#include "Game.h"

/*------------------------------------------------------------------------------
	ライブラリのリンク
------------------------------------------------------------------------------*/
#pragma comment(lib, "winmm.lib")		//マルチメディア（システム時刻取得に必要）
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")		//入力

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define RAND_INT( Range) ( Range / -2 + rand() % Range)										//整数ランダムマクロ
#define RAND_FLORT( Range) ( Range * -0.5f + (float)rand() / (float)RAND_MAX * Range)		//フロート値ランダムマクロ

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class RenderManager;
class TextureManager;
class XModelManager;
class InputKeyboard;
class InputMouse;
class CollisionManager;
class SoundManager;
class ShaderManager;
class Mode;
class ComponentFactory;

/*------------------------------------------------------------------------------
	列挙型定義
------------------------------------------------------------------------------*/
typedef enum
{
	eLayerSky = 0,		//スカイ
	eLayerBack,			//背景
	eLayerDefault,		//デフォルト
	eLayerWireFrame,	//ワイヤーフレーム
	eLayerAddBlend,		//加算合成
	eLayerBillboard,	//ビルボード
	eLayerUI,			//UI
	eLayerNum			//レイヤー数
}E_LAYER;

/*------------------------------------------------------------------------------
	構造体定義
------------------------------------------------------------------------------*/
//頂点情報
//typedef struct
//{
//	D3DXVECTOR3 Pos;		//XYZクラス
//	D3DCOLOR Color;			//頂点カラー
//	D3DXVECTOR2 Tex;		//UV
//}VERTEX_DEFAULT;

typedef struct
{
	D3DXVECTOR3 Pos;		//XYZクラス
	D3DXVECTOR3 Normal;		//法線
	D3DCOLOR Color;			//頂点カラー
	D3DXVECTOR2 Tex;		//UV
}VERTEX_3D;

typedef struct
{
	D3DXVECTOR3 Pos;		//頂点座標
	D3DCOLOR Color;			//頂点カラー
	D3DXVECTOR2 Tex;		//テクスチャ座標
} VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 Pos;				//頂点座標
	D3DXVECTOR3 Normal;				//法線
	D3DXVECTOR2 Tex;				//UV
	float Weight[4];				//ウェイト値
	unsigned char BornIndex[4];		//ボーンのインデックス番号
} VERTEX_SKIN_MESH;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Manager
{
public:
	Manager( HINSTANCE hInstance,HWND hWnd, BOOL bWindow);
	~Manager();
	static LPDIRECT3DDEVICE9 GetDevice( void);
	static RenderManager *GetRenderManager( void);
	static TextureManager *GetTextureManager( void);
	static XModelManager *GetXModelManager( void);
	static InputKeyboard *GetInputKeyboard( void);
	static InputMouse *GetInputMouse( void);
	static CollisionManager *GetCollisionManager( void);
	static SoundManager *GetSoundManager( void);
	static ShaderManager *GetShaderManager( void);
	static ComponentFactory *GetComponentFactory( void);
	static void SetMode( Mode *pNextMode);
	static void PushMode( Mode *pPushMode);
	static void PopMode( void);

	void Update(void);
	void Draw(void);

private:
	static RenderManager *m_pRenderManager;			//レンダーマネージャーポインタ
	static TextureManager *m_pTextureManager;		//テクスチャマネージャーポインタ
	static XModelManager *m_pXModelManager;			//Xモデルマネージャーポインタ
	static InputKeyboard *m_pInputKeyboard;			//キーボードポインタ
	static InputMouse *m_pInputMouse;				//マウスポインタ
	static CollisionManager *m_pCollisionManager;	//衝突判定マネージャーポインタ
	static SoundManager *m_pSoundManager;			//サウンドマネージャー
	static ShaderManager *m_pShaderManager;			//シェーダーマネージャー
	static ComponentFactory *m_pComponentFactory;	//コンポーネント生成ファクトリー
	//static Mode *m_pMode;							//モード
	static std::stack<Mode*> m_stackMode;			//モード（スタック）

	void ChangeMode( Mode *pNextMode);
	static Mode *m_pNextMode;

	HINSTANCE m_hInstance;				//インスタンス
	HWND m_hWnd;						//ウインドウのハンドル
};




#endif