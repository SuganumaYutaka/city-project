/*==============================================================================

    CollisionManager.h - 衝突判定（コライダー）管理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "CollisionManager.h"
#include "GameObject.h"
#include "ComponentInclude.h"

/*------------------------------------------------------------------------------
	コンストラクタ
	引数
		HWND hWnd
		BOOL bWindow
------------------------------------------------------------------------------*/
CollisionManager::CollisionManager()
{
	
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
CollisionManager::~CollisionManager()
{
	
}

/*------------------------------------------------------------------------------
	コライダーリストに追加
	引数
		Collider *pCollider			コライダー
------------------------------------------------------------------------------*/
void CollisionManager::AddCollider( Collider *pCollider)
{
	m_listCollider.push_back( pCollider);
}

/*------------------------------------------------------------------------------
	コライダーリストから消去
	引数
		Collider *pCollider			コライダー
------------------------------------------------------------------------------*/
void CollisionManager::ReleaseCollider( Collider *pCollider)
{
	m_listReleaseCollider.push_back( pCollider);
}

/*------------------------------------------------------------------------------
	コライダーリストをソート
------------------------------------------------------------------------------*/
void CollisionManager::Sort()
{

}

/*------------------------------------------------------------------------------
	衝突判定
------------------------------------------------------------------------------*/
void CollisionManager::Collision()
{
	//コライダーリストから削除
	for (auto Col : m_listReleaseCollider)
	{
		for (auto ite = m_listCollider.begin(); ite != m_listCollider.end(); ++ite)
		{
			if( *ite == Col)
			{
				m_listCollider.erase(ite);
				break;
			}
		}
	}
	m_listReleaseCollider.clear();

	//動的オブジェクト同士
	for( std::list< Collider *>::iterator Source = m_listCollider.begin(); Source != m_listCollider.end(); ++Source)
	{
		for( std::list< Collider *>::iterator Dest = Source; Dest != m_listCollider.end(); ++Dest)
		{
			if( *Source != NULL && *Dest != NULL && Source != Dest)
			{
				if( IsCollision( Source, Dest) == true)
				{
					//お互いのコライダーに衝突を知らせる
					dynamic_cast< Collider *>( *Source)->GetGameObject()->OnCollision( *Dest);
					dynamic_cast< Collider *>( *Dest)->GetGameObject()->OnCollision( *Source);
				}
			}
		}
	}

	//位置を更新
	for (auto Col : m_listCollider)
	{
		if (Col->GetActive() == true)
		{
			Col->m_OldPos = Col->m_pTransform->GetWorldPosition();
		}
	}
}

/*------------------------------------------------------------------------------
	衝突しているか
	引数
		std::list< Collider *>::iterator Source		イテレータ（１つ目）
		std::list< Collider *>::iterator Dest		イテレータ（２つ目）
	戻り値
		true		衝突あり
		false		衝突なし
------------------------------------------------------------------------------*/
bool CollisionManager::IsCollision( std::list< Collider *>::iterator Source, std::list< Collider *>::iterator Dest)
{
	Collider *pSource = *Source;
	Collider *pDest = *Dest;

	if (pSource->GetActive() == false || pDest->GetActive() == false)
	{
		return false;
	}

	//ボックスコライダー同士
	if( pSource->GetColType() == eColBox && pDest->GetColType() == eColBox)
	{
		if (BoxBox((BoxCollider*)pSource, (BoxCollider*)pDest))
		{
			//衝突あり
			return true;
		}
	}

	//ボックスとメッシュフィールド
	else if (pSource->GetColType() == eColBox && pDest->GetColType() == eColMeshField)
	{
		if (BoxMeshField((BoxCollider*)pSource, (MeshFieldCollider*)pDest))
		{
			//衝突あり
			return true;
		}
	}

	else if (pSource->GetColType() == eColMeshField && pDest->GetColType() == eColBox)
	{
		if (BoxMeshField((BoxCollider*)pDest, (MeshFieldCollider*)pSource))
		{
			//衝突あり
			return true;
		}
	}

	//衝突なし
	return false;
}

/*------------------------------------------------------------------------------
	ボックスとボックス
	引数
		BoxCollider *pSource
		BoxCollider *pDest
	戻り値
		true		衝突あり
		false		衝突なし
------------------------------------------------------------------------------*/
bool CollisionManager::BoxBox( BoxCollider *pSource, BoxCollider *pDest)
{
	//変数宣言
	Vector3 SourcePos = pSource->GetTransform()->GetWorldPosition();
	Vector3 SourceSize = pSource->GetSize() * 0.5f;
	Vector3 DestPos = pDest->GetTransform()->GetWorldPosition();
	Vector3 DestSize = pDest->GetSize() * 0.5f;

	//中心位置をずらす
	D3DXVECTOR3 center;
	D3DXMATRIX mtxRot;
	center = pSource->GetCenter().ConvertToDX();
	D3DXMatrixRotationQuaternion( &mtxRot, &pSource->m_pTransform->GetWorldRotation());
	D3DXVec3TransformCoord( &center, &center, &mtxRot);
	SourcePos += Vector3::ConvertFromDX( center);
	center = pDest->GetCenter().ConvertToDX();
	D3DXMatrixRotationQuaternion( &mtxRot, &pDest->m_pTransform->GetWorldRotation());
	D3DXVec3TransformCoord( &center, &center, &mtxRot);
	DestPos += Vector3::ConvertFromDX( center);

	//衝突判定
	if( SourcePos.x + SourceSize.x > DestPos.x - DestSize.x && SourcePos.x - SourceSize.x < DestPos.x + DestSize.x &&
		SourcePos.y + SourceSize.y > DestPos.y - DestSize.y && SourcePos.y - SourceSize.y < DestPos.y + DestSize.y &&
		SourcePos.z + SourceSize.z > DestPos.z - DestSize.z && SourcePos.z - SourceSize.z < DestPos.z + DestSize.z)
	{
		//オブジェクトをはじく処理
		//pSourceが静的オブジェクトでpDestが動的オブジェクト
		if( pSource->m_IsTrigger == false && pDest->m_IsTrigger == true)
		{
			Vector3 Pos = pDest->m_pTransform->GetWorldPosition();

			//手前
			if ( SourcePos.z - SourceSize.z > pDest->m_OldPos.z + DestSize.z)
			{
				Pos.z = SourcePos.z - SourceSize.z - DestSize.z;
			}

			//奥
			else if( SourcePos.z + SourceSize.z < pDest->m_OldPos.z - DestSize.z)
			{
				Pos.z = SourcePos.z + SourceSize.z + DestSize.z;
			}

			//左
			if ( SourcePos.x - SourceSize.x > pDest->m_OldPos.x + DestSize.x)
			{
				Pos.x = SourcePos.x - SourceSize.x - DestSize.x;
			}

			//右
			else if( SourcePos.x + SourceSize.x < pDest->m_OldPos.x - DestSize.x)
			{
				Pos.x = SourcePos.x + SourceSize.x + DestSize.x;
			}

			//下
			if ( SourcePos.y - SourceSize.y > pDest->m_OldPos.y + DestSize.y)
			{
				Pos.y = SourcePos.y - SourceSize.y - DestSize.y;
			}

			//上
			else if( SourcePos.y + SourceSize.y < pDest->m_OldPos.y - DestSize.y)
			{
				Pos.y = SourcePos.y + SourceSize.y + DestSize.y;
			}

			Vector3 move = Pos - pDest->m_pTransform->GetWorldPosition();
			move *= 1.1f;
			pDest->m_pTransform->Move( move);

		}

		//pSourceが動的オブジェクトでpDestが静的オブジェクト
		else if (pSource->m_IsTrigger == true && pDest->m_IsTrigger == false)
		{
			Vector3 Pos = pSource->m_pTransform->GetWorldPosition();

			//手前
			if ( DestPos.z - DestSize.z > pSource->m_OldPos.z + SourceSize.z)
			{
				Pos.z = DestPos.z - DestSize.z - SourceSize.z;
			}

			//奥
			else if( DestPos.z + DestSize.z < pSource->m_OldPos.z - SourceSize.z)
			{
				Pos.z = DestPos.z + DestSize.z + SourceSize.z;
			}

			//左
			if ( DestPos.x - DestSize.x > pSource->m_OldPos.x + SourceSize.x)
			{
				Pos.x = DestPos.x - DestSize.x - SourceSize.x;
			}

			//右
			else if( DestPos.x + DestSize.x < pSource->m_OldPos.x - SourceSize.x)
			{
				Pos.x = DestPos.x + DestSize.x + SourceSize.x;
			}

			//下
			if ( DestPos.y - DestSize.y > pSource->m_OldPos.y + SourceSize.y)
			{
				Pos.y = DestPos.y - DestSize.y - SourceSize.y;
			}

			//上
			else if( DestPos.y + DestSize.y < pSource->m_OldPos.y - SourceSize.y)
			{
				Pos.y = DestPos.y + DestSize.y + SourceSize.y;
			}

			Vector3 move = Pos - pSource->m_pTransform->GetWorldPosition();
			move *= 1.1f;
			pSource->m_pTransform->Move( move);
		}

		else
		{
			//未対応
			//assert(false);
		}

		//衝突あり
		return true;
	}

	//衝突なし
	return false;
}

/*------------------------------------------------------------------------------
	ボックスとメッシュフィールド
	引数
		BoxCollider *pBox
		MeshFieldCollider *pMeshField
	戻り値
		true		衝突あり
		false		衝突なし
------------------------------------------------------------------------------*/
bool CollisionManager::BoxMeshField(BoxCollider *pBox, MeshFieldCollider *pMeshField)
{
	Vector3 Pos = pBox->GetTransform()->GetWorldPosition();
	Pos.y -= pBox->GetSize().y * 0.5f;
	float Height = pMeshField->GetHeight(Pos);

	//中心位置をずらす
	D3DXVECTOR3 center;
	D3DXMATRIX mtxRot;
	center = pBox->GetCenter().ConvertToDX();
	D3DXMatrixRotationQuaternion( &mtxRot, &pBox->m_pTransform->GetWorldRotation());
	D3DXVec3TransformCoord( &center, &center, &mtxRot);
	Pos += Vector3::ConvertFromDX( center);

	if (Pos.y < Height)
	{
		//上に移動
		if (pMeshField->m_IsTrigger == false)
		{
			pBox->m_pTransform->SetWorldPositionY( Height + pBox->GetSize().y * 0.5f - pBox->GetCenter().y);
		}

		//衝突あり
		return true;
	}

	//衝突なし
	return false;
}

/*------------------------------------------------------------------------------
	直線と立方体の衝突判定処理関数
	引数
	D3DXVECTOR3 posWorld	直線の始点(ワールド空間)
	D3DXVECTOR3 dirWorld	直線の方向(ワールド空間)
	D3DXVECTOR3 size		立方体の大きさ
	D3DXMATRIX mtxWorld		立方体のワールド座標変換行列
	float *fDist			始点から立方体までの距離（出力）
	D3DXVECTOR3 *colPos		衝突判定位置（出力）
	戻り値
	bool
------------------------------------------------------------------------------*/
bool CollisionManager::CollisionRayBox( D3DXVECTOR3 posWorld, D3DXVECTOR3 dirWorld, D3DXVECTOR3 size, D3DXMATRIX mtxWorld, float &Dist, D3DXVECTOR3 *colPos)
{
	//直線を立方体の位置へ移動
	D3DXMATRIX mtxInv;			//ワールド座標変換行列の逆行列
	D3DXMatrixInverse( &mtxInv, 0, &mtxWorld);

	D3DXVECTOR3 posLocal, dirLocal;		//直線のローカル座標・方向
	D3DXVec3TransformCoord( &posLocal, &posWorld, &mtxInv);
	mtxInv._41 = 0.0f;
	mtxInv._42 = 0.0f;
	mtxInv._43 = 0.0f;
	D3DXVec3TransformCoord( &dirLocal, &dirWorld, &mtxInv);

	//交差判定
	//変数宣言
	float pos[ 3 ], dir[ 3 ], min[ 3 ], max[ 3 ];		//for文で回す用
	memcpy( pos, &posLocal, sizeof( D3DXVECTOR3 ) );
	memcpy( dir, &dirLocal, sizeof( D3DXVECTOR3 ) );
	memcpy( min, &D3DXVECTOR3( -size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f), sizeof( D3DXVECTOR3));
	memcpy( max, &D3DXVECTOR3( size.x * 0.5f, size.y * 0.5f, size.z * 0.5f), sizeof( D3DXVECTOR3));

	//最大値・最小値代入
	float fDistMin = -FLT_MAX;	//最も近い距離
	float fDistMax = FLT_MAX;	//最も遠い距離
	
	//XYZの交差判定
	for( int nCnt = 0; nCnt < 3; nCnt++)
	{
		if ( abs( dir[ nCnt]) < FLT_EPSILON)
		{
			if ( pos[ nCnt] < min[ nCnt] || pos[ nCnt] > max[ nCnt] )
			{
				//衝突なし
				return false;
			}
		}
		else
		{
			//スラブとの距離を算出
			float odd = 1.0f / dir[ nCnt];
			float t1 = ( min[ nCnt] - pos[ nCnt]) * odd;
			float t2 = ( max[ nCnt] - pos[ nCnt]) * odd;
			if ( t1 > t2)
			{
				float tmp = t1; t1 = t2; t2 = tmp;		//最大・最小を入れ替え
			}
	
			//距離を更新
			if ( t1 > fDistMin)
			{
				fDistMin = t1;
			}
			if ( t2 < fDistMax)
			{
				fDistMax = t2;
			}

			// スラブ交差チェック
			if ( fDistMin >= fDistMax )
			{
				//衝突なし
				return false;
			}
		}
	}

	//衝突位置出力
	if( colPos)
	{
		if( Dist != NULL)
		{
			Dist = fDistMin;
		}
		if( colPos != NULL)
		{
			*colPos = posWorld + fDistMin * dirWorld;
		}
	}
	
	//衝突あり
	return true;
}


