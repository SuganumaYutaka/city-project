/*==============================================================================

    SkinMeshRenderer.cpp - �X�L�����b�V���̕`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/9/21
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "SkinMeshRenderer.h"
#include "Material.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define MAX_CLUSTER (58)		//�N���X�^�[�̍ő吔�i�V�F�[�_�[�ɓn�����ߌŒ蒷�j

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* SkinMeshRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<SkinMeshRenderer>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
SkinMeshRenderer::SkinMeshRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 0;

	m_NumPolygon = 0;
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void SkinMeshRenderer::Uninit( void)
{
	Manager::GetRenderManager()->ReleaseRenderer( this);

	//���_�o�b�t�@�̉��
	SAFE_RELEASE( m_pVtxBuff);
	
	//�}�e���A���̉��
	m_vecMaterial.clear();

	//�A�j���[�V�����f�[�^�̉��
	m_vecAnimation.clear();
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void SkinMeshRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void SkinMeshRenderer::Draw( Camera* pCamera)
{
	if (m_NumPolygon == 0)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾
																	
	//���f���̕`��
	int NumMaterial = m_vecMaterial.size();
	for( int nCntMaterial = 0; nCntMaterial < NumMaterial; nCntMaterial++)
	{
		auto Mat = m_vecMaterial[ nCntMaterial];

		//�}�e���A���̐ݒ�
		Mat.Set( pCamera, this);

		//���_���ݒ�
		pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_SKIN_MESH));

		//�e�N�j�b�N�J�n
		Mat.Begin( m_nPass);

		//�|���S���̕`��
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_NumPolygon);

		//�e�N�j�b�N�I��
		Mat.End();	
	}
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�쐬
------------------------------------------------------------------------------*/
void SkinMeshRenderer::CreateVtxBuffer()
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//���_�o�b�t�@����
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_SKIN_MESH) * m_vecPositionIndex.size(),			//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,						//���_�o�b�t�@�̎g�p���@(�����ɉe��)
		0,										//FVF(���_�t�H�[�}�b�g)
		D3DPOOL_MANAGED,						//�������̊Ǘ�(MANAGED�̓f�o�C�X�ɂ��܂���)
		&m_pVtxBuff,							//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X
		NULL)))	
	{
		//�G���[
		assert( false);
		return;
	}

	//���_�o�b�t�@�ݒ�
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�ݒ�
------------------------------------------------------------------------------*/
void SkinMeshRenderer::SetVtxBuffer()
{
	//���b�N
	VERTEX_SKIN_MESH* pVtx;		//���z�A�h���X�p�|�C���^
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	int NumVtx = m_vecPositionIndex.size();
	for( int nCnt = 0; nCnt < NumVtx; nCnt++)
	{
		//���_���W
		pVtx[ nCnt].Pos = m_vecPoint[ m_vecPositionIndex[nCnt]].Position;

		//�@��
		pVtx[ nCnt].Normal = m_vecNormal[ m_vecNormalIndex[ nCnt]];

		//�e�N�X�`�����W
		pVtx[ nCnt].Tex = m_vecTexcoord[ m_vecTexcoordIndex[ nCnt]];
	
		//�{�[���̃C���f�b�N�X�ԍ��ƃE�F�C�g�l
		for (int i = 0; i < 4; i++)
		{
			pVtx[ nCnt].Weight[i] = (float)m_vecPoint[ m_vecPositionIndex[ nCnt]].BornRefarence[ i].Weight;
			pVtx[ nCnt].BornIndex[i] = (unsigned char)m_vecPoint[ m_vecPositionIndex[ nCnt]].BornRefarence[ i].Index;
		}
	}

	//�A�����b�N
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	���b�V���f�[�^��ǂݍ���
------------------------------------------------------------------------------*/
void SkinMeshRenderer::LoadMeshData(FILE *pFile, SkinMeshModel* pModel)
{
	//�t�@�C���`�F�b�N
	if (pFile == NULL)
	{
		return;
	}

	//���f���|�C���^�ݒ�
	m_pSkinMeshModel = pModel;

	int size;					//�i�[�T�C�Y
	char fileName[MAX_PATH];	//�t�@�C����
	D3DXMATRIX mtxIdentitiy;
	D3DXMatrixIdentity(&mtxIdentitiy);

	//�|���S����
	fread( &size, sizeof(int), 1, pFile);
	m_NumPolygon = size;

	//�ʒu���W�ƃ{�[�����
	fread( &size, sizeof( int), 1, pFile);
	m_vecPoint.resize(size);
	fread( m_vecPoint.data(), sizeof(Point), size, pFile);
	
	//���_�C���f�b�N�X
	fread( &size, sizeof( int), 1, pFile);
	m_vecPositionIndex.resize(size);
	fread( m_vecPositionIndex.data(), sizeof(unsigned short), size, pFile);

	//�@��
	fread( &size, sizeof( int), 1, pFile);
	m_vecNormal.resize(size);
	fread( m_vecNormal.data(), sizeof( D3DXVECTOR3), size, pFile);

	//�@���C���f�b�N�X
	fread( &size, sizeof(int), 1, pFile);
	m_vecNormalIndex.resize(size);
	fread( m_vecNormalIndex.data(), sizeof(unsigned short), size, pFile);

	//UV���W
	fread( &size, sizeof(int), 1, pFile);
	m_vecTexcoord.resize(size);
	fread( m_vecTexcoord.data(), sizeof(D3DXVECTOR2), size, pFile);

	//UV���W�C���f�b�N�X
	fread( &size, sizeof(int), 1, pFile);
	m_vecTexcoordIndex.resize(size);
	fread( m_vecTexcoordIndex.data(), sizeof(unsigned short), size, pFile);

	//�}�e���A��
	fread(&size, sizeof(int), 1, pFile);
	m_vecMaterial.resize(size);
	int sizeMat = m_vecMaterial.size();
	for( int i = 0; i < sizeMat; i++)
	{
		Material& mat = m_vecMaterial[i];

		//�}�e���A���̐ݒ�
		mat.SetShader( eShaderSkinMesh);

		//�e�N�X�`����
		fread(&size, sizeof(int), 1, pFile);
		fread(&fileName, sizeof(char), size, pFile);
		mat.SetTexture(fileName);

		//�J���[�f�[�^
		D3DXVECTOR4 color;
		fread(&color, sizeof(D3DXVECTOR4), 1, pFile);
		mat.SetAmbient(color.x, color.y, color.z, color.w);
		fread(&color, sizeof(D3DXVECTOR4), 1, pFile);
		mat.SetDiffuse(color.x, color.y, color.z, color.w);
		fread(&color, sizeof(D3DXVECTOR4), 1, pFile);
		mat.SetSpecular(color.x, color.y, color.z, color.w);
		fread(&color, sizeof(D3DXVECTOR4), 1, pFile);
		mat.SetEmissive(color.x, color.y, color.z, color.w);
	}

	//�A�j���[�V����
	fread(&size, sizeof(int), 1, pFile);
	m_vecAnimation.resize(size);
	int sizeAnim = m_vecAnimation.size();
	for( int i = 0; i < sizeAnim; i++)
	{
		Animation& anim = m_vecAnimation[i];

		fread(&size, sizeof(int), 1, pFile);
		fread(&fileName, sizeof(char), size, pFile);
		anim.Name = fileName;
		
		//fread(&anim.StartTime, sizeof(FbxTime), 1, pFile);
		//fread(&anim.EndTime, sizeof(FbxTime), 1, pFile);
		fread(&anim.AllFrame, sizeof(int), 1, pFile);
		anim.vecMatrix = 
			std::vector<std::vector<D3DXMATRIX>>( anim.AllFrame, std::vector<D3DXMATRIX>(MAX_CLUSTER, mtxIdentitiy));
		for( int nCnt = 0; nCnt < anim.AllFrame; nCnt++)
		{
			for( int nCntCluster = 0; nCntCluster < MAX_CLUSTER; nCntCluster++)
			{
				fread( &anim.vecMatrix[nCnt][nCntCluster], sizeof(D3DXMATRIX), 1, pFile);
			}
		}
	}

	//���_�o�b�t�@�ݒ�
	CreateVtxBuffer();
}

/*------------------------------------------------------------------------------
	�X�P�[���̐ݒ�
------------------------------------------------------------------------------*/
void SkinMeshRenderer::SetScale(float rate)
{
	//�X�P�[���s��̐ݒ�
	D3DXMATRIX mtxScale;
	D3DXMatrixIdentity( &mtxScale);
	D3DXMatrixScaling( &mtxScale, rate, rate, rate);

	//�ʒu���W
	int sizePoint = m_vecPoint.size();
	for (int nCnt = 0; nCnt < sizePoint; nCnt++)
	{
		Point& point = m_vecPoint[nCnt];
		D3DXVec3Scale( &point.Position, &point.Position, rate);
	}

	//�A�j���[�V�������
	int sizeAnim = m_vecAnimation.size();
	for (int nCntAnim = 0; nCntAnim < sizeAnim; nCntAnim++)
	{
		for (int nCntFrame = 0; nCntFrame < m_vecAnimation[nCntAnim].AllFrame; nCntFrame++)
		{
			for (int nCntCluster = 0; nCntCluster < MAX_CLUSTER; nCntCluster++)
			{
				m_vecAnimation[nCntAnim].vecMatrix[nCntFrame][nCntCluster] *= mtxScale;
			}
		}
	}
}
