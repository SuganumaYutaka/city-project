/*==============================================================================

    ShaderManager.cpp - �V�F�[�_�[�Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "ShaderManager.h"
#include "Shader.h"
#include "DefaultShader.h"
#include "BillboardShader.h"
#include "PerPixelLightingShader.h"
#include "VertexLightingShader.h"
#include "SpriteShader.h"
#include "DepthShader.h"
#include "ShadowVLShader.h"
#include "ParticleShader.h"
#include "SkinMeshShader.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
ShaderManager::ShaderManager()
{
	//�V�F�[�_�[�̃��[�h
	m_vecShaderSet.clear();
	m_vecShaderSet.resize( eNumShader);
	for (int nCnt = 0; nCnt < eNumShader; nCnt++)
	{
		m_vecShaderSet[ nCnt] = NULL;
	}

	//�V�F�[�_�[�̓ǂݍ���
	//�f�t�H���g
	m_mapShaderLoad[ "def_Default"] = new DefaultShader();
	m_mapShaderLoad[ "def_Billboard"] = new BillboardShader();
	m_mapShaderLoad[ "def_XModel"] = new VertexLightingShader();
	m_mapShaderLoad[ "def_Sprite"] = new SpriteShader();
	m_mapShaderLoad[ "particle"] = new ParticleShader();
	m_mapShaderLoad[ "skinmesh"] = new SkinMeshShader();

	//�e����
	m_mapShaderLoad[ "shadowVL"] = new ShadowVLShader();

	//�f�v�X���ݒ�
	m_mapShaderLoad[ "depth"] = new DepthShader();

	//�V�F�[�_�[�Z�b�g
	SetDefault();
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
ShaderManager::~ShaderManager()
{
	for (auto Shader : m_mapShaderLoad)
	{
		 delete Shader.second;
	}
	m_mapShaderLoad.clear();

	m_vecShaderSet.clear();
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�����[�h
	����
	EShaderType Type
	�߂�l
	Shader *
------------------------------------------------------------------------------*/
Shader *ShaderManager::Load( EShaderType Type)
{
	//�͈͂��`�F�b�N
	if (Type < 0 || Type >= m_vecShaderSet.capacity())
	{
		assert(false);
		return NULL;
	}

	//���[�h����Ă��邩
	if (m_vecShaderSet[Type] == NULL)
	{
		MessageBox( NULL, "���̃V�F�[�_�[�̓��[�h����Ă��܂���B\nShaderManager.cpp�̃R���X�g���N�^�ɒǉ����Ă�������\n", "�G���[", MB_OK);
		return NULL;
	}

	return m_vecShaderSet[ Type];
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�؂�ւ��i�f�t�H���g�j
------------------------------------------------------------------------------*/
void ShaderManager::SetDefault(void)
{
	//m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "def_Default"];
	m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "shadowVL"];
	//m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "def_XModel"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "shadowVL"];
	
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "def_Billboard"];
	m_vecShaderSet[ eShaderSprite]		= m_mapShaderLoad[ "def_Sprite"];
	m_vecShaderSet[ eShaderParticle]	= m_mapShaderLoad[ "particle"];
	m_vecShaderSet[ eShaderSky]			= m_mapShaderLoad[ "def_Default"];

	m_vecShaderSet[ eShaderSkinMesh]	= m_mapShaderLoad[ "skinmesh"];
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�؂�ւ��i�e����j
------------------------------------------------------------------------------*/
void ShaderManager::SetOnShadow(void)
{
	/*m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "shadow_Default"];
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "shadow_Billboard"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "shadow_XModel"];
	m_vecShaderSet[ eShaderSprite]		= m_mapShaderLoad[ "shadow_Sprite"];*/
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�؂�ւ��iZ�o�b�t�@�ݒ�j
------------------------------------------------------------------------------*/
void ShaderManager::SetDepth(void)
{
	m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "depth"];
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "depth"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "depth"];
	m_vecShaderSet[ eShaderSkinMesh]	= m_mapShaderLoad[ "depth"];
}

