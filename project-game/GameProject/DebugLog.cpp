/*==============================================================================

	DebugLog.cpp - �f�o�b�O���O
													   Author : Yutaka Suganuma
													   Date   : 2017/11/15
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "DebugLog.h"
#include <time.h>

#include "imgui.h"
#include "imgui_impl_dx9.h"

/*------------------------------------------------------------------------------
	�ÓI�����o�ϐ��錾
------------------------------------------------------------------------------*/
Text DebugLog::Log;

/*------------------------------------------------------------------------------
	�Z�[�u
------------------------------------------------------------------------------*/
void DebugLog::Save()
{
	if (Log.GetAllText().size() == 0)
	{
		return;
	}

	std::string fileName = "data/LOG/";
	time_t now = time(NULL);
	struct tm *pNow = localtime(&now);
	fileName += std::to_string( (int)(pNow->tm_year + 1900));
	fileName += std::to_string( (int)(pNow->tm_mon + 1));
	fileName += std::to_string( (int)(pNow->tm_mday)) + '_';
	fileName += std::to_string( (int)(pNow->tm_hour));
	fileName += std::to_string( (int)(pNow->tm_min));
	fileName += std::to_string( (int)(pNow->tm_sec)) + ".txt";
	Log.Save(fileName);
}

/*------------------------------------------------------------------------------
	ImGui�̕\�����ݒ�
------------------------------------------------------------------------------*/
void DebugLog::SetImGuiView(void)
{
#ifdef _DEBUG
	
	ImGui::Begin("DebugLog");
	
	ImGui::Text( Log.GetAllText().c_str());

	ImGui::End();

#endif // _DEBUG
}
