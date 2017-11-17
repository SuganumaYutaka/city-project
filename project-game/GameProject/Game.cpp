/*==============================================================================
	
	Game.cpp - �E�B���h�E�̐ݒ�E�Q�[�����[�v����
														Author : Yutaka Suganuma
														Date   : 2017/3/28
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include <Windows.h>
#include <assert.h>
#include "Manager.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define CLASS_NAME		"Game"				//�E�C���h�E�N���X��
#define WINDOW_NAME		"�t���[���݌v"		//�E�C���h�E�i�^�C�g���j��

/*------------------------------------------------------------------------------
	�ÓI�����o�ϐ��錾
------------------------------------------------------------------------------*/
Game *Game::m_This = NULL;
bool Game::m_bRan = false;

/*------------------------------------------------------------------------------
	�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*------------------------------------------------------------------------------
	�R���X�g���N�^
	����
		HINSTANCE hInstance
		HINSTANCE hPrevInstance
		LPSTR lpCmdLine
		int nCmdShow
------------------------------------------------------------------------------*/
Game::Game(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//�ϐ����g�p���Ȃ��ꍇ�̌x������\�������Ȃ�
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//�J�X�^���E�C���h�E�̒�`
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);			//WNDCLASSEX�̃������T�C�Y�i�o�C�g�j��ݒ�
	wcex.style = CS_CLASSDC;					//�\������E�C���h�E�̃X�^�C����ݒ�
	wcex.lpfnWndProc = WndProc;						//�E�C���h�E�v���V�[�W���̃A�h���X�i�֐����j���w��
	wcex.cbClsExtra = 0;							//�ʏ�͎g�p���Ȃ��̂�"0"���w��
	wcex.cbWndExtra = 0;							//�ʏ�͎g�p���Ȃ��̂�"0"���w��	
	wcex.hInstance = hInstance;					//WinMain�̃p�����[�^�̃C���X�^���X�n���h��
	wcex.hIcon = NULL;							//���g�p����A�C�R�����w��iWindows�������Ă���j
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);	//�}�E�X�J�[�\�����w��
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//���E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
	wcex.lpszMenuName = NULL;							//Window�ɂ��郁�j���[��ݒ�
	wcex.lpszClassName = CLASS_NAME;					//�E�C���h�E�N���X�̖��O
	wcex.hIconSm = NULL;							//���������A�C�R�����ݒ肳�ꂽ�ꍇ�̏����L�q

													//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�X�N���[���𐳂����T�C�Y��
	DWORD style = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);		//�E�C���h�E�X�^�C��
	RECT cr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&cr, style, false);
	int ww = cr.right - cr.left;
	int wh = cr.bottom - cr.top;

	//�f�X�N�g�b�v�̒��S�ɕ\��
	RECT dr;
	GetWindowRect(GetDesktopWindow(), &dr);

	int dw = dr.right - dr.left;
	int dh = dr.bottom - dr.top;

	int wx = ww > dw ? 0 : (dw - ww) / 2;
	int wy = wh > dh ? 0 : (dh - wh) / 2;

	//�E�C���h�E���쐬
	m_hWnd = CreateWindowEx(0,		//�g���E�C���h�E�X�^�C��
		CLASS_NAME,					//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,				//�E�C���h�E�̖��O
		style,						//���E�C���h�E�X�^�C��
		wx,							//�E�C���h�E�̍���w���W
		wy,							//�E�C���h�E�̍���x���W
		ww,							//�E�C���h�E�̕��i�t���[���܂߂�j
		wh,							//�E�C���h�E�̍����i�t���[���܂߂�j
		NULL,						//�e�E�C���h�E�̃n���h��
		NULL,						//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,					//�C���X�^���X�n���h��
		NULL);						//�E�C���h�E�쐬�f�[�^

	//�E�C���h�E���쐬�i�t���X�N���[���j
	//m_hWnd = CreateWindowEx(0,		//�g���E�C���h�E�X�^�C��
	//	CLASS_NAME,					//�E�C���h�E�N���X�̖��O
	//	WINDOW_NAME,				//�E�C���h�E�̖��O
	//	WS_POPUP,						//���E�C���h�E�X�^�C��
	//	CW_USEDEFAULT,
	//	CW_USEDEFAULT,
	//	dw,
	//	dh,
	//	NULL,						//�e�E�C���h�E�̃n���h��
	//	NULL,						//���j���[�n���h���܂��͎q�E�C���h�EID
	//	hInstance,					//�C���X�^���X�n���h��
	//	NULL);						//�E�C���h�E�쐬�f�[�^

									//�E�C���h�E�̕\��
	ShowWindow(m_hWnd, nCmdShow);	//�w�肳�ꂽ�E�C���h�E�̕\����Ԃ�ݒ�i�E�C���h�E��\���j
	UpdateWindow(m_hWnd);			//�E�C���h�E�̏�Ԃ𒼂��ɔ��f

									//this�|�C���^�̐ݒ�
	m_This = this;

	//�C���X�^���X�̐ݒ�
	m_hInstance = hInstance;

	//�t���O�̐ݒ�
	m_bWindow = TRUE;

	//�Q�[�����[�v�t���O�ݒ�
	m_bRan = true;
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Game::~Game()
{

}

/*------------------------------------------------------------------------------
	�E�C���h�E�v���V�[�W��
	�iCALLBACK : Win32API�֐����Ăяo�����̋K��j
	����
		hWnd	: �E�C���h�E�̃n���h��
		uMsg	: ���b�Z�[�W�̎��ʎq
		wParam	: ���b�Z�[�W�̍ŏ��̃p�����[�^
		lParam	: ���b�Z�[�W��2�Ԗڂ̃p�����[�^�i�⑫���j

	�߂�l
		DefWindowProc( hWnd, uMsg, wParam, lParam)
------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;
	
	//���b�Z�[�W
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		//�I�����b�Z�[�W
			UINT nID;
			nID = MessageBox(NULL, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON2);
			if (nID == IDYES)
			{
				Game::Finish();
				//DestroyWindow( hWnd);
			}
			else
			{
				return 0;
			}
		break;

	case WM_KEYDOWN:
		//�L�[�̎擾
		switch (wParam)
		{
		case VK_ESCAPE:
			//�I�����b�Z�[�W
			UINT nID;
			nID = MessageBox(NULL, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON2);
			if (nID == IDYES)
			{
				Game::Finish();
				//DestroyWindow( hWnd);
			}
			break;

		default:
			break;
		}
		break;

	//���j���[����
	case WM_COMMAND:
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*------------------------------------------------------------------------------
	�Q�[�����[�v����
------------------------------------------------------------------------------*/
void Game::Ran(void)
{
	//�Q�[�����[�v���ݒ肳��Ă��邩
	assert(IsRan());

	//fps�ݒ�p
	DWORD dwFrameCount = 0;		//
	DWORD dwCurrentTime = 0;	//���݂̎���
	DWORD dwExecLastTime = 0;	//�O�̃t���[���̎��s�I�����̎���
	DWORD dwFPSLastTime = 0;	//

	//����\��ݒ�
	timeBeginPeriod(1);

	//�e�J�E���^�[�̏�����
	dwFrameCount = 0;
	dwCurrentTime = 0;
	dwExecLastTime =				//���Ԃ����S�Ɉ�v�����邽�߂܂Ƃ߂�
		dwFPSLastTime = timeGetTime();	//�V�X�e���������~���b�P�ʂŎ擾

	//����������
	m_This->Init();

	//���b�Z�[�W���[�v
	for (;;)
	{
		if (PeekMessage(&m_This->m_Msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			//Windows�̏���
			if (m_This->m_Msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&m_This->m_Msg);		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&m_This->m_Msg);		//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else if (!IsRan())
		{
			break;
		}
		else {
			//fps����
			dwCurrentTime = timeGetTime();		//�V�X�e���������擾

												//0.5�b���ƂɎ��s
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
#ifdef _DEBUG
				m_This->m_nFPS = ( dwFrameCount * 1000) / ( dwCurrentTime - dwFPSLastTime);	//�b�Ԃ̕���fps���擾
#endif	//_DEBUG

				dwFPSLastTime = dwCurrentTime;	//fps�v�����Ԃ����݂̎��Ԃ�
				dwFrameCount = 0;		//�t���[���J�E���^���Z�b�g
			}

			//60fps�Ŏ��s
			if (dwCurrentTime - dwExecLastTime >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;		//(�O���)�����������ԍX�V

													//�X�V����
				m_This->Update();

				//�`�揈��
				m_This->Draw();

				//�t���[���J�E���^
				dwFrameCount++;
			}
		}
	}

	//�I������
	m_This->Uninit();

	//�Q�[�����[�v�I��
	delete m_This;
}

/*------------------------------------------------------------------------------
	�Q�[�����[�v���@�\���Ă��邩
------------------------------------------------------------------------------*/
bool Game::IsRan(void)
{
	return (m_bRan);
}

/*------------------------------------------------------------------------------
	�Q�[�����[�v�I������
------------------------------------------------------------------------------*/
void Game::Finish(void)
{
	//�Q�[�����[�v���ݒ肳��Ă��邩
	assert(IsRan());

	//�t���O�̐ݒ�
	m_bRan = false;
}

/*------------------------------------------------------------------------------
	����������
------------------------------------------------------------------------------*/
void Game::Init(void)
{
	//�}�l�[�W���[����
	m_pManager = new Manager( m_hInstance, m_hWnd, m_bWindow);

	//imgui
	ImGui_ImplDX9_Init(m_hWnd, m_pManager->GetDevice());
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void Game::Uninit(void)
{
	//imgui
	ImGui_ImplDX9_Shutdown();

	//�}�l�[�W���[�̔j��
	if( m_pManager != NULL)
	{
		delete m_pManager;
		m_pManager = NULL;
	}
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void Game::Update(void)
{
	//imgui
	ImGui_ImplDX9_NewFrame();

	m_pManager->Update();

	//static bool show_test_window = true;
 //   static bool show_another_window = true;
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//// 1. Show a simple window
 //   // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
 //   {
 //       static float f = 0.0f;
	//	static Vector3 vec;
	//	vec.y = 3.0f;
	//	static float pos[3] = {0,0,0};
 //       ImGui::Text("Hello, world!");
 //       ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
 //       ImGui::ColorEdit3("clear color", (float*)&clear_color);
 //       if (ImGui::Button("Test Window")) show_test_window ^= 1;
 //       if (ImGui::Button("Another Window")) show_another_window ^= 1;
 //       ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//	ImGui::DragFloat3( "Position", &vec.x);
 //   }

 //   // 2. Show another simple window, this time using an explicit Begin/End pair
 //   if (show_another_window)
 //   {
 //       ImGui::Begin("Another Window", &show_another_window);
 //       ImGui::Text("Hello from another window!");
 //       ImGui::End();
 //   }

 //   // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
 //   if (show_test_window)
 //   {
 //       ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
 //       ImGui::ShowTestWindow(&show_test_window);
 //   }
}

/*------------------------------------------------------------------------------
	�`�揈��
------------------------------------------------------------------------------*/
void Game::Draw(void)
{
	m_pManager->Draw();
}
