/*==============================================================================

    WallRenderer.h - �����̎��������[�Ǖ`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _WALL_RENDERER_H_
#define _WALL_RENDERER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Renderer.h"
#include "ShaderManager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Material;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class WallRenderer : public Renderer
{
public:
	static Component* Create( GameObject* gameObject);

	WallRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);
	void LoadTexture( std::string fileName);

	void SetShader( EShaderType Type);

	VERTEX_3D* StartSetVertexBuffer( int countVertex, int countPolygon);
	void EndSetVertexBuffer( void);

	bool ChangeRenderPolygon();
	bool AddRenderPolygon();

	void ChangeWireFrame( bool isWireFrame);

private:
	Material *m_pMaterial;
	int m_CountVertex;
	int m_CountPolygon;
	int m_CountRenderPolygon;
};

#endif

