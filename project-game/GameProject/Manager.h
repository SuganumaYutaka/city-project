/*==============================================================================
	
	Manager.h - �Q�[���S�̂̊Ǘ�
														Author : Yutaka Suganuma
														Date   : 2017/5/10
==============================================================================*/
#ifndef _MANAGER_H_
#define _MANAGER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
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
#include <d3dx9.h>	//<d3d9.h>���܂܂��
#define DIRECTINPUT_VERSION (0x0800)		//���̑g�ݍ��킹�ŏ��Ԏ��悤��
#include <dinput.h>	//����
#include <XAudio2.h>	//�T�E���h

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

//�ėp�N���X
#include "Vector2.h"
#include "Vector3.h"
#include "Text.h"
#include "TextureUV.h"
#include "Game.h"

/*------------------------------------------------------------------------------
	���C�u�����̃����N
------------------------------------------------------------------------------*/
#pragma comment(lib, "winmm.lib")		//�}���`���f�B�A�i�V�X�e�������擾�ɕK�v�j
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")		//����

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define RAND_INT( Range) ( Range / -2 + rand() % Range)										//���������_���}�N��
#define RAND_FLORT( Range) ( Range * -0.5f + (float)rand() / (float)RAND_MAX * Range)		//�t���[�g�l�����_���}�N��

/*------------------------------------------------------------------------------
	�O���錾
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
	�񋓌^��`
------------------------------------------------------------------------------*/
typedef enum
{
	eLayerSky = 0,		//�X�J�C
	eLayerBack,			//�w�i
	eLayerDefault,		//�f�t�H���g
	eLayerWireFrame,	//���C���[�t���[��
	eLayerAddBlend,		//���Z����
	eLayerBillboard,	//�r���{�[�h
	eLayerUI,			//UI
	eLayerNum			//���C���[��
}E_LAYER;

/*------------------------------------------------------------------------------
	�\���̒�`
------------------------------------------------------------------------------*/
//���_���
//typedef struct
//{
//	D3DXVECTOR3 Pos;		//XYZ�N���X
//	D3DCOLOR Color;			//���_�J���[
//	D3DXVECTOR2 Tex;		//UV
//}VERTEX_DEFAULT;

typedef struct
{
	D3DXVECTOR3 Pos;		//XYZ�N���X
	D3DXVECTOR3 Normal;		//�@��
	D3DCOLOR Color;			//���_�J���[
	D3DXVECTOR2 Tex;		//UV
}VERTEX_3D;

typedef struct
{
	D3DXVECTOR3 Pos;		//���_���W
	D3DCOLOR Color;			//���_�J���[
	D3DXVECTOR2 Tex;		//�e�N�X�`�����W
} VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 Pos;				//���_���W
	D3DXVECTOR3 Normal;				//�@��
	D3DXVECTOR2 Tex;				//UV
	float Weight[4];				//�E�F�C�g�l
	unsigned char BornIndex[4];		//�{�[���̃C���f�b�N�X�ԍ�
} VERTEX_SKIN_MESH;

/*------------------------------------------------------------------------------
	�N���X��`
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
	static RenderManager *m_pRenderManager;			//�����_�[�}�l�[�W���[�|�C���^
	static TextureManager *m_pTextureManager;		//�e�N�X�`���}�l�[�W���[�|�C���^
	static XModelManager *m_pXModelManager;			//X���f���}�l�[�W���[�|�C���^
	static InputKeyboard *m_pInputKeyboard;			//�L�[�{�[�h�|�C���^
	static InputMouse *m_pInputMouse;				//�}�E�X�|�C���^
	static CollisionManager *m_pCollisionManager;	//�Փ˔���}�l�[�W���[�|�C���^
	static SoundManager *m_pSoundManager;			//�T�E���h�}�l�[�W���[
	static ShaderManager *m_pShaderManager;			//�V�F�[�_�[�}�l�[�W���[
	static ComponentFactory *m_pComponentFactory;	//�R���|�[�l���g�����t�@�N�g���[
	//static Mode *m_pMode;							//���[�h
	static std::stack<Mode*> m_stackMode;			//���[�h�i�X�^�b�N�j

	void ChangeMode( Mode *pNextMode);
	static Mode *m_pNextMode;

	HINSTANCE m_hInstance;				//�C���X�^���X
	HWND m_hWnd;						//�E�C���h�E�̃n���h��
};




#endif