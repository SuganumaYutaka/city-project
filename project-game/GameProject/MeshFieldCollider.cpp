/*==============================================================================

    MeshFieldCollider.h - メッシュフィールドコライダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/31
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "MeshFieldCollider.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "Transform.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define P_POS_X(P_WIDTH, NUM_FIELD_X) (0.0f + P_WIDTH * NUM_FIELD_X * -0.5f)		//開始地点のX座標
#define P_POS_Y (0.0f)																//開始地点のY座標
#define P_POS_Z(P_HEIGHT, NUM_FIELD_Z) (0.0f + P_HEIGHT * NUM_FIELD_Z * 0.5f)		//開始地点のZ座標
#define P_WIDTH( ALL_WIDTH, NUM_FIELD_X)			( ALL_WIDTH / NUM_FIELD_X)		//ポリゴン一枚あたりの幅
#define P_HEIGHT( ALL_HEIGHT, NUM_FIELD_Z)			( ALL_HEIGHT / NUM_FIELD_Z)		//ポリゴン一枚あたりの高さ

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* MeshFieldCollider::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<MeshFieldCollider>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
MeshFieldCollider::MeshFieldCollider( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = pGameObject->GetComponent<Transform>();
	m_ColType = eColMeshField;
	m_IsTrigger = false;

	//デフォルト値設定
	m_nNumBlockX = 8;			//横の分割数
	m_nNumBlockZ = 8;			//縦の分割数
	m_fWidth = 8.0f;			//幅
	m_fHeight = 8.0f;			//高さ
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//ポリゴン1枚あたりの幅
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//ポリゴン1枚あたりの高さ

	//頂点設定
	m_pVtxPos = new Vector3[ ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1)];
	for( int nCntHeight = 0; nCntHeight < m_nNumBlockZ + 1; nCntHeight++)
	{
		for( int nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			//頂点座標の設定
			m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth] = Vector3(
				m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
				0.0f,
				m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
		}
	}

	//ポリゴン設定
	m_pPolygon = new POLYGON[ m_nNumBlockX * m_nNumBlockZ * 2];
	for (int nCntZ = 0; nCntZ < m_nNumBlockZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nNumBlockX; nCntX++)
		{
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].nVtxIndex[ 0] = ( nCntZ + 1) * ( m_nNumBlockX + 1) + nCntX;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].nVtxIndex[ 1] = ( nCntZ + 0) * ( m_nNumBlockX + 1) + nCntX;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].nVtxIndex[ 2] = ( nCntZ + 1) * ( m_nNumBlockX + 1) + nCntX + 1;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].Normal = GetPolygonNormal( (nCntZ * m_nNumBlockX + nCntX) * 2 + 0);

			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].nVtxIndex[ 0] = ( nCntZ + 0) * ( m_nNumBlockX + 1) + nCntX + 1;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].nVtxIndex[ 1] = ( nCntZ + 1) * ( m_nNumBlockX + 1) + nCntX + 1;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].nVtxIndex[ 2] = ( nCntZ + 0) * ( m_nNumBlockX + 1) + nCntX;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].Normal = GetPolygonNormal( (nCntZ * m_nNumBlockX + nCntX) * 2 + 1);
		}
	}

}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void MeshFieldCollider::Uninit( void)
{
	Manager::GetCollisionManager()->ReleaseCollider( this);

	//頂点情報の解放
	delete[] m_pVtxPos;
	m_pVtxPos = NULL;

	//ポリゴン情報の解放
	delete[] m_pPolygon;
	m_pPolygon = NULL;
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void MeshFieldCollider::Update()
{
	
}

/*------------------------------------------------------------------------------
	衝突
	引数
		Collider *pCollider			衝突したコライダー
------------------------------------------------------------------------------*/
void MeshFieldCollider::OnCollision( Collider *pCollider)
{
	
}

/*------------------------------------------------------------------------------
	フィールドを設定
------------------------------------------------------------------------------*/
void MeshFieldCollider::SetField( int X, int Z, float BlockWidth, float BlockHeight, float *pVertexHeight)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//設定
	m_nNumBlockX = X;
	m_nNumBlockZ = Z;
	m_fBlockWidth = BlockWidth;
	m_fBlockHeight = BlockHeight;
	m_fWidth = m_nNumBlockX * m_fBlockWidth;
	m_fHeight = m_nNumBlockZ * m_fBlockHeight;
	
	//頂点情報の解放
	delete[] m_pVtxPos;
	m_pVtxPos = NULL;

	//ポリゴン情報の解放
	delete[] m_pPolygon;
	m_pPolygon = NULL;

	//頂点設定
	m_pVtxPos = new Vector3[ ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1)];
	for( int nCntHeight = 0; nCntHeight < m_nNumBlockZ + 1; nCntHeight++)
	{
		for( int nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			if( pVertexHeight != NULL)
			{
				//頂点座標の設定
				m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth] = Vector3(
					m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
					pVertexHeight[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth],
					m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
			}
			else
			{
				//頂点座標の設定
				m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth] = Vector3(
					m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
					0.0f,
					m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
			}
		}
	}

	//ポリゴン設定
	m_pPolygon = new POLYGON[ m_nNumBlockX * m_nNumBlockZ * 2];
	for (int nCntZ = 0; nCntZ < m_nNumBlockZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nNumBlockX; nCntX++)
		{
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].nVtxIndex[ 0] = ( nCntZ + 1) * ( m_nNumBlockX + 1) + nCntX;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].nVtxIndex[ 1] = ( nCntZ + 0) * ( m_nNumBlockX + 1) + nCntX;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].nVtxIndex[ 2] = ( nCntZ + 1) * ( m_nNumBlockX + 1) + nCntX + 1;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].Normal = GetPolygonNormal((nCntZ * m_nNumBlockX + nCntX) * 2 + 0);

			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].nVtxIndex[ 0] = ( nCntZ + 0) * ( m_nNumBlockX + 1) + nCntX + 1;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].nVtxIndex[ 1] = ( nCntZ + 1) * ( m_nNumBlockX + 1) + nCntX + 1;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].nVtxIndex[ 2] = ( nCntZ + 0) * ( m_nNumBlockX + 1) + nCntX;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].Normal = GetPolygonNormal((nCntZ * m_nNumBlockX + nCntX) * 2 + 1);
		}
	}
}

/*------------------------------------------------------------------------------
	ポリゴン一枚あたりの幅・高さを設定
------------------------------------------------------------------------------*/
void MeshFieldCollider::SetBlockSize( float Width, float Height)
{
	//設定
	m_fBlockWidth = Width;
	m_fBlockHeight = Height;
	m_fWidth = m_nNumBlockX * m_fBlockWidth;
	m_fHeight = m_nNumBlockZ * m_fBlockHeight;
	
	//頂点設定
	for( int nCntHeight = 0; nCntHeight < m_nNumBlockZ + 1; nCntHeight++)
	{
		for( int nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			//頂点座標の設定
			m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth] = Vector3(
				m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
				m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth].y,
				m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
		}
	}
}

/*------------------------------------------------------------------------------
	幅・高さを設定
------------------------------------------------------------------------------*/
void MeshFieldCollider::SetSize( float Width, float Height)
{
	//設定
	m_fWidth = Width;
	m_fHeight = Height;
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//ポリゴン1枚あたりの幅
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//ポリゴン1枚あたりの高さ
	
	//頂点設定
	for( int nCntHeight = 0; nCntHeight < m_nNumBlockZ + 1; nCntHeight++)
	{
		for( int nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			//頂点座標の設定
			m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth] = Vector3(
				m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
				m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth].y,
				m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
		}
	}
}

/*------------------------------------------------------------------------------
	高さを取得
------------------------------------------------------------------------------*/
float MeshFieldCollider::GetHeight( const Vector3& Pos)
{
	int nIndexPolygon = GetIndexPolygon( Pos);			//ポリゴンのインデックス番号

	//Posがフィールド上か
	if (nIndexPolygon == -1)
	{
		return -5.0f;
	}

	//法線取得
	Vector3 Normal = m_pPolygon[ nIndexPolygon].Normal;

	//高さを算出
	Vector3 PosZero = m_pVtxPos[ m_pPolygon[ nIndexPolygon].nVtxIndex[ 0]];
	float Height = PosZero.y - (( Pos.x - PosZero.x) * Normal.x + ( Pos.z - PosZero.z) * Normal.z) / Normal.y;
	
	return Height;
}

/*------------------------------------------------------------------------------
	Posをと重なるポリゴン（インデックス）を取得
------------------------------------------------------------------------------*/
int MeshFieldCollider::GetIndexPolygon( const Vector3& Pos)
{
	float PosX = Pos.x;
	float PosZ = Pos.z;
	int nBlockX, nBlockZ;

	PosX += m_fWidth * 0.5f;
	nBlockX = (int)( PosX / m_fBlockWidth);
	PosZ = m_fHeight * 0.5f - PosZ;
	nBlockZ = (int)( PosZ / m_fBlockHeight);

	//フィールド上のとき
	//if (PosX >= 0 && nBlockX < m_nNumBlockX && PosZ >= 0 && nBlockZ < m_nNumBlockZ)
	//{
	//	//外積による判定
	//	if (IsPointInPolygon((nBlockZ * m_nNumBlockX + nBlockX) * 2, Pos) == true)
	//	{
	//		return (nBlockZ * m_nNumBlockX + nBlockX) * 2;
	//	}
	//	//else if (IsPointInPolygon((nBlockZ * m_nNumBlockX + nBlockX) * 2 + 1, Pos) == true)
	//	else
	//	{
	//		return (nBlockZ * m_nNumBlockX + nBlockX) * 2 + 1;
	//	}
	//}

	for (int nCnt = 0; nCnt < m_nNumBlockX * m_nNumBlockZ * 2; nCnt++)
	{
		if (IsPointInPolygon(nCnt, Pos) == true)
		{
			return nCnt;
		}
	}

	return -1;
}

/*------------------------------------------------------------------------------
	ある点がポリゴン内にあるか
------------------------------------------------------------------------------*/
bool MeshFieldCollider::IsPointInPolygon( int nIndex, const Vector3& Pos)
{
	Vector3 Vec01 = m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[1]] - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[0]];
	Vector3 Vec0P = Pos - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[0]];
	//Vec01.y = 0.0f;
	//Vec0P.y = 0.0f;
	Vector3 Cross0 = Vector3::Cross(Vec01, Vec0P);
	if (Cross0.y < 0.0f)
	{
		return false;
	}

	Vector3 Vec12 = m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[2]] - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[1]];
	Vector3 Vec1P = Pos - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[1]];
	//Vec12.y = 0.0f;
	//Vec1P.y = 0.0f;
	Vector3 Cross1 = Vector3::Cross(Vec12, Vec1P);
	if (Cross1.y < 0.0f)
	{
		return false;
	}
	
	Vector3 Vec20 = m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[0]] - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[2]];
	Vector3 Vec2P = Pos - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[2]];
	//Vec20.y = 0.0f;
	//Vec2P.y = 0.0f;
	Vector3 Cross2 = Vector3::Cross(Vec20, Vec2P);
	if (Cross2.y < 0.0f)
	{
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------
	ポリゴンの面法線を取得
------------------------------------------------------------------------------*/
Vector3 MeshFieldCollider::GetPolygonNormal( int nIndex)
{
	Vector3 Vec1 = m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[1]] - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[0]];
	Vector3 Vec2 = m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[2]] - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[0]];
	
	return Vector3::Cross( Vec1, Vec2);
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void MeshFieldCollider::Load(Text& text)
{
	//textを読み進める
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	float* pVertexHeight = NULL;

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "NumBlockX")
		{
			text.ForwardPositionToNextWord();
			m_nNumBlockX = std::stoi(text.GetWord());
		}
		else if (text.GetWord() == "NumBlockZ")
		{
			text.ForwardPositionToNextWord();
			m_nNumBlockZ = std::stoi(text.GetWord());
		}
		else if (text.GetWord() == "Width")
		{
			text.ForwardPositionToNextWord();
			m_fWidth = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Height")
		{
			text.ForwardPositionToNextWord();
			m_fHeight = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "BlockWidth")
		{
			text.ForwardPositionToNextWord();
			m_fBlockWidth = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "BlockHeight")
		{
			text.ForwardPositionToNextWord();
			m_fBlockHeight = std::stof(text.GetWord());
		}

		else if (text.GetWord() == "VertexHeight")
		{
			pVertexHeight = new float[ ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1)];
			for (int nCnt = 0; nCnt < ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1); nCnt++)
			{
				text.ForwardPositionToNextWord();
				pVertexHeight[ nCnt] = std::stof(text.GetWord());
			}
		}

		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

	SetField( m_nNumBlockX, m_nNumBlockZ, m_fBlockWidth, m_fBlockHeight, pVertexHeight);
}

/*------------------------------------------------------------------------------
	セーブ
------------------------------------------------------------------------------*/
void MeshFieldCollider::Save(Text& text)
{
	StartSave(text);

	text += "NumBlockX " + std::to_string(m_nNumBlockX) + ' ';
	text += "NumBlockZ " + std::to_string(m_nNumBlockZ) + ' ';
	text += "Width " + std::to_string(m_fWidth) + ' ';
	text += "Height " + std::to_string(m_fHeight) + ' ';
	text += "BlockWidth " + std::to_string(m_fBlockWidth) + ' ';
	text += "BlockHeight " + std::to_string(m_fBlockHeight) + '\n';
	text += "VertexHeight ";
	for (int nCnt = 0; nCnt < ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1); nCnt++)
	{
		text += std::to_string(m_pVtxPos[ nCnt].y) + ' ';
	}
	text += "\n";

	EndSave( text);
}