/*==============================================================================

    ShaderManager.h - �V�F�[�_�[�Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/7
==============================================================================*/
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Shader;
class Camera;
class Renderer;
class Material;

/*------------------------------------------------------------------------------
	�񋓌^��`
------------------------------------------------------------------------------*/
typedef enum
{
	eShaderDefault = 0,
	eShaderBillboard,
	eShaderXModel,
	eShaderSprite,
	eShaderParticle,
	eShaderSky,
	eShaderSkinMesh,
	eNumShader
}EShaderType;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	void SetShader( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, EShaderType Type);
	Shader* Load( EShaderType Type);

	void SetDefault( void);
	void SetLow( void);
	void SetHigh( void);
	void SetDepth( void);

private:
	std::vector<Shader*> m_vecShaderSet;							//�Z�b�g����V�F�[�_�[
	std::unordered_map< std::string, Shader*> m_mapShaderLoad;		//�ǂݍ��ݍς݃V�F�[�_�[
	Shader* m_CurrentShader;
	
};



#endif