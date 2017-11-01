/*==============================================================================
	
	Manager.h - �E�B���h�E�̐ݒ�E�Q�[�����[�v����
														Author : Yutaka Suganuma
														Date   : 2017/3/28
==============================================================================*/
#ifndef _GAME_H_
#define _GAME_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include <Windows.h>

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Manager;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define SCREEN_WIDTH	(1200)				//�E�C���h�E�̕�
#define SCREEN_HEIGHT	(675)				//�E�C���h�E����

#define SAFE_RELEASE(p) { if(p){ (p)->Release(); (p) = NULL;}}		//�Z�[�t�����[�X�}�N��

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Game
{
public:
	Game(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	~Game();
	static void Ran(void);
	static bool IsRan(void);
	static void Finish(void);

	static int GetFPS( void) {return m_This->m_nFPS;}

private:
	static Game *m_This;				//this�|�C���^
	static bool m_bRan;					//�Q�[�����[�v�t���O
	HINSTANCE m_hInstance;				//�C���X�^���X
	HWND m_hWnd;						//�E�C���h�E�̃n���h��
	BOOL m_bWindow;						//�t���X�N���[���t���O
	MSG m_Msg;							//���b�Z�[�W
	int m_nFPS;							//FPS
	Manager *m_pManager;				//�}�l�[�W���[�|�C���^

	void Init( void);
	void Uninit( void);
	void Update( void);
	void Draw( void);
};




#endif