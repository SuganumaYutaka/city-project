/*==============================================================================

    SkinMeshShader.h - �X�L�����b�V���V�F�[�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/9/21
==============================================================================*/
#ifndef _SKIN_MESH_SHADER_H_
#define _SKIN_MESH_SHADER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shader.h"

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class SkinMeshShader : public Shader
{
public:
	SkinMeshShader();
	~SkinMeshShader();
	void Set( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, bool isAlreadySet);
private:
	D3DXHANDLE m_hTech;
	D3DXHANDLE m_hMtxWorld;
	D3DXHANDLE m_hMtxWorldInv;
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProj;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightAmb;
	D3DXHANDLE m_hLightDif;
	D3DXHANDLE m_hMaterialAmb;
	D3DXHANDLE m_hMaterialDif;
	D3DXHANDLE m_hDirLight;
	D3DXHANDLE m_hClusters;
};


#endif
