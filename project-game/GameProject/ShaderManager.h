/*==============================================================================

    ShaderManager.h - シェーダー管理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/7
==============================================================================*/
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Shader;
class Camera;
class Renderer;
class Material;

/*------------------------------------------------------------------------------
	列挙型定義
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
	クラス定義
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
	std::vector<Shader*> m_vecShaderSet;							//セットするシェーダー
	std::unordered_map< std::string, Shader*> m_mapShaderLoad;		//読み込み済みシェーダー
	Shader* m_CurrentShader;
	
};



#endif