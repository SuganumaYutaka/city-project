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
#include "ShadowPLShader.h"
#include "ParticleShader.h"
#include "SkinMeshShader.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
ShaderManager::ShaderManager() : m_CurrentShader( NULL)
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
	m_mapShaderLoad[ "default"] = new DefaultShader();
	m_mapShaderLoad[ "vertexLighting"] = new VertexLightingShader();
	m_mapShaderLoad[ "perpixel"] = new PerPixelLightingShader();
	m_mapShaderLoad[ "billboard"] = new BillboardShader();
	m_mapShaderLoad[ "sprite"] = new SpriteShader();
	m_mapShaderLoad[ "particle"] = new ParticleShader();
	m_mapShaderLoad[ "skinmesh"] = new SkinMeshShader();
	m_mapShaderLoad[ "shadowVL"] = new ShadowVLShader();
	m_mapShaderLoad[ "shadowPL"] = new ShadowPLShader();
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
	�V�F�[�_�[���Z�b�g
------------------------------------------------------------------------------*/
void ShaderManager::SetShader( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, EShaderType Type)
{
	auto shader = Load( Type);
	if (m_CurrentShader == shader)
	{
		shader->Set( pCamera, pRenderer, pMaterial, true);
	}
	else
	{
		shader->Set( pCamera, pRenderer, pMaterial, false);
		m_CurrentShader = shader;
	}
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
	if (Type < 0 || Type >= (int)m_vecShaderSet.capacity())
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
	m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "vertexLighting"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "perpixel"];
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "billboard"];
	m_vecShaderSet[ eShaderSkinMesh]	= m_mapShaderLoad[ "skinmesh"];
	m_vecShaderSet[ eShaderSprite]		= m_mapShaderLoad[ "sprite"];
	m_vecShaderSet[ eShaderParticle]	= m_mapShaderLoad[ "particle"];
	m_vecShaderSet[ eShaderSky]			= m_mapShaderLoad[ "default"];
	
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�؂�ւ��i��i���j
------------------------------------------------------------------------------*/
void ShaderManager::SetLow(void)
{
	m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "default"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "default"];
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "billboard"];
	m_vecShaderSet[ eShaderSkinMesh]	= m_mapShaderLoad[ "skinmesh"];
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�؂�ւ��i���i���j
------------------------------------------------------------------------------*/
void ShaderManager::SetHigh(void)
{
	m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "shadowPL"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "shadowPL"];
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "billboard"];
	m_vecShaderSet[ eShaderSkinMesh]	= m_mapShaderLoad[ "skinmesh"];
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�؂�ւ��iZ�o�b�t�@�ݒ�j
------------------------------------------------------------------------------*/
void ShaderManager::SetDepth(void)
{
	m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "depth"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "depth"];
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "depth"];
	m_vecShaderSet[ eShaderSkinMesh]	= m_mapShaderLoad[ "depth"];
}

