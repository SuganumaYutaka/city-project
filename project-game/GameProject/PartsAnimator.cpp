/*==============================================================================

    PartsAnimator.cpp - パーツアニメーション
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/6
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "PartsAnimator.h"
#include "GameObject.h"
#include "Transform.h"
#include "XModelRenderer.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define MAX_BUF (1024)
#define TIME_CHANGE (5)					//モーション切り替えにかかるフレーム

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* PartsAnimator::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<PartsAnimator>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
PartsAnimator::PartsAnimator( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_nNowMotion = 0;
	m_nNextMotion = 0;
	m_nNowKey = 0;
	m_nNextKey = 1;
	m_nCntFrame = 0;
	m_nChangeFrame = 0;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void PartsAnimator::Uninit( void)
{
	//モーションデータの解放
	for (MOTION Motion : vecMotion)
	{
		//キーセットの解放
		for (KEYSET Keyset : Motion.vecKeySet)
		{
			//キーの解放
			Keyset.vecKey.clear();
		}
		Motion.vecKeySet.clear();
	}
	vecMotion.clear();
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void PartsAnimator::Update(void)
{
	//モーション設定
	MOTION motNow = vecMotion[ m_nNowMotion];			//現在のモーションデータ
	MOTION motNext = vecMotion[ m_nNextMotion];		//次のモーションデータ

	//キーセット設定
	KEYSET keysetNow =	motNow.vecKeySet[ m_nNowKey];		//現在のキーセットデータ
	KEYSET keysetNext =	motNext.vecKeySet[ m_nNextKey];		//次のキーセットデータ

	//データからパーツの位置・角度更新
	Vector3 Pos;
	D3DXQUATERNION OffsetRot;
	D3DXQUATERNION NowRot;
	D3DXQUATERNION NextRot;
	float hoge = (float)m_nCntFrame / (float)(m_nChangeFrame);
	int sizeParts = vecParts.size();
	for( int nCntParts = 0; nCntParts < sizeParts; nCntParts++)
	{
		//位置
		//Pos = keysetNow.vecKey[ nCntParts].Position + ( keysetNext.vecKey[ nCntParts].Position - keysetNow.vecKey[ nCntParts].Position) * (float)m_nCntFrame / m_nChangeFrame;
		//vecParts[ nCntParts]->m_pTransform->SetLocalPosition( vecOfsetPos[ nCntParts] + Pos);

		//角度
		D3DXQuaternionRotationYawPitchRoll( &OffsetRot, vecOfsetRot[ nCntParts].y, vecOfsetRot[ nCntParts].x, vecOfsetRot[ nCntParts].z);
		D3DXQuaternionRotationYawPitchRoll( &NowRot, keysetNow.vecKey[ nCntParts].Rotation.y, keysetNow.vecKey[ nCntParts].Rotation.x, keysetNow.vecKey[ nCntParts].Rotation.z);
		D3DXQuaternionRotationYawPitchRoll( &NextRot, keysetNext.vecKey[ nCntParts].Rotation.y, keysetNext.vecKey[ nCntParts].Rotation.x, keysetNext.vecKey[ nCntParts].Rotation.z);
		D3DXQuaternionSlerp( &NowRot, &NowRot, &NextRot, (FLOAT)(hoge));
		OffsetRot = NowRot * OffsetRot;
		vecParts[ nCntParts]->m_pTransform->SetLocalRotation( OffsetRot);
	}

	//フレームの更新
	if( motNow.nLoop == 1 || m_nNextKey + 1 < motNow.nNumKey || m_nCntFrame < m_nChangeFrame - 1)
	{
		m_nCntFrame++;
	}

	//キーの変更
	if( m_nCntFrame >= m_nChangeFrame)
	{
		m_nCntFrame = 0;
		if( motNow.nLoop == 1)
		{
			ChangeMotion( m_nNextMotion, ( m_nNextKey + 1) % (motNow.nNumKey));
		}
		else
		{
			ChangeMotion( m_nNextMotion, ( m_nNextKey + 1));
		}
	}
}

/*------------------------------------------------------------------------------
	アニメーションデータをロード
------------------------------------------------------------------------------*/
void PartsAnimator::LoadAnimator( std::string fileName)
{
	//スクリプト読み込み
	FILE *fp;			//ファイルポインタ
	char aBuf[MAX_BUF];	//読み取り用
	int nCnt = 0;		//カウンタ
	std::vector<std::string> vecFileName;		//読み込むファイル名
	int nNumModel = 0;
	int nNumParts = 0;
	
	//ファイルオープン
	fp =fopen( fileName.c_str(), "r");
	if( fp == NULL)
	{
		MessageBox( NULL, "model.cpp\nスクリプトの読み込みに失敗しました\n", "エラー", MB_OK);
		fclose(fp);
		return;
	}

	//スクリプトファイルかチェック(1)
	while( 1)
	{
		if( fscanf( fp, "%s", aBuf) == EOF)
		{
			MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(1)\n", "エラー", MB_OK);
			fclose(fp);
			return;
		}
		//コメント
		if( aBuf[ 0] == '#')
		{
			fgets( aBuf, MAX_BUF, fp);
		}
		if( strcmp( aBuf, "SCRIPT") == 0)
		{
			break;
		}
	}

	//モデル数(2)
	while( 1)
	{
		if( fscanf( fp, "%s", aBuf) == EOF)
		{
			MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(2)\n", "エラー", MB_OK);
			fclose(fp);
			return;
		}
		//コメント
		if( aBuf[ 0] == '#')
		{
			fgets( aBuf, MAX_BUF, fp);
		}
		if( strcmp( aBuf, "NUM_MODEL") == 0)
		{
			fscanf( fp, "%s", aBuf);
			fscanf( fp, "%d", &nNumModel);		//モデル数
			//fscanf( fp, "%s", aFileName[ nCnt]);
			break;
		}
	}

	//モデルファイル名(3)
	nCnt = 0;
	while( nCnt < nNumModel)		//モデル数分
	{
		if( fscanf( fp, "%s", aBuf) == EOF)
		{
			MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(3)\n", "エラー", MB_OK);
			fclose(fp);
			return;
		}
		//コメント
		if( aBuf[ 0] == '#')
		{
			fgets( aBuf, MAX_BUF, fp);
		}
		if( strcmp( aBuf, "MODEL_FILENAME") == 0)
		{
			fscanf( fp, "%s", aBuf);
			fscanf( fp, "%s", aBuf);
			vecFileName.push_back( aBuf);		//モデルのファイル名

			nCnt++;		//モデルカウントアップ
		}
	}

	//キャラクター情報(4)
	while( strcmp( aBuf, "END_CHARACTERSET") != 0)
	{
		if( fscanf( fp, "%s", aBuf) == EOF)
		{
			MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(4)\n", "エラー", MB_OK);
			fclose(fp);
			return;
		}
		//コメント
		if( aBuf[ 0] == '#')
		{
			fgets( aBuf, MAX_BUF, fp);
		}
		if( strcmp( aBuf, "CHARACTERSET") == 0)
		{
			//キャラクター情報(5)
			while( strcmp( aBuf, "END_CHARACTERSET") != 0)
			{
				if( fscanf( fp, "%s", aBuf) == EOF)
				{
					MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(5)\n", "エラー", MB_OK);
					fclose(fp);
					return;
				}
				//コメント
				if( aBuf[ 0] == '#')
				{
					fgets( aBuf, MAX_BUF, fp);
				}
				//パーツ数
				if( strcmp( aBuf, "NUM_PARTS") == 0)
				{
					fscanf( fp, "%s", aBuf);
					fscanf( fp, "%d", &nNumParts);
					break;		//ループ抜ける
				}
			}

			//パーツを生成
			if( nNumParts <= 0)
			{
				MessageBox( NULL, "model.cpp\nパーツ数を設定してください\n", "エラー", MB_OK);
				fclose(fp);
				return;
			}
			for (int i = 0; i < nNumParts; i++)
			{
				GameObject* pObj = new GameObject( m_pGameObject);
				pObj->IsCreatedByOtherComponent = true;
				auto renderer = pObj->AddComponent<XModelRenderer>();
				renderer->IsCreatedByOtherComponent = true;
				vecParts.push_back( pObj);
			}

			//パーツ情報(6)
			nCnt = 0;
			while( strcmp( aBuf, "END_CHARACTERSET") != 0 && nCnt <= nNumParts)
			{
				if( fscanf( fp, "%s", aBuf) == EOF)
				{
					MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(6)\n", "エラー", MB_OK);
					fclose(fp);
					return;
				}
				//コメント
				if( aBuf[ 0] == '#')
				{
					fgets( aBuf, MAX_BUF, fp);
				}
				if( strcmp( aBuf, "PARTSSET") == 0)
				{
					while( strcmp( aBuf, "END_CHARACTERSET") != 0
						&& strcmp( aBuf, "END_PARTSSET") != 0)
					{
						if( fscanf( fp, "%s", aBuf) == EOF)
						{
							MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(6')\n", "エラー", MB_OK);
							fclose(fp);
							return;
						}
						//コメント
						if( aBuf[ 0] == '#')
						{
							fgets( aBuf, MAX_BUF, fp);
						}
						//モデルのインデックス番号
						if( strcmp( aBuf, "INDEX") == 0)
						{
							int nModelIdx = 0;
							fscanf( fp, "%s", aBuf);
							fscanf( fp, "%d", &nModelIdx);

							//モデルを設定
							vecParts[nCnt]->GetComponent<XModelRenderer>()->LoadXModel( vecFileName[nModelIdx]);
						}
						//親のインデックス番号
						if( strcmp( aBuf, "PARENT") == 0)
						{
							int nParent;
							fscanf( fp, "%s", aBuf);
							fscanf( fp, "%d", &nParent);
							if( nParent >= 0)
							{
								vecParts[nCnt]->m_pTransform->SetParent( vecParts[nParent]->m_pTransform);	//親の設定
							}
						}
						//位置
						if( strcmp( aBuf, "POS") == 0)
						{
							Vector3 pos;
							fscanf( fp, "%s", aBuf);
							fscanf( fp, "%f", &pos.x);
							fscanf( fp, "%f", &pos.y);
							fscanf( fp, "%f", &pos.z);
							vecOfsetPos.push_back(pos);
							vecParts[nCnt]->m_pTransform->SetLocalPosition( pos);
						}
						//角度
						if( strcmp( aBuf, "ROT") == 0)
						{
							Vector3 rot;
							fscanf( fp, "%s", aBuf);
							fscanf( fp, "%f", &rot.x);
							fscanf( fp, "%f", &rot.y);
							fscanf( fp, "%f", &rot.z);
							vecOfsetRot.push_back(rot);
							vecParts[nCnt]->m_pTransform->SetLocalRotationEuler( rot);
						}
					}
					nCnt++;		//パーツカウントアップ
				}
			}
		}
	}

	//モーション情報(7)
	nCnt = 0;
	while( strcmp( aBuf, "END_SCRIPT") != 0)
	{
		if( fscanf( fp, "%s", aBuf) == EOF)
		{
			MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(7)\n", "エラー", MB_OK);
			fclose(fp);
			return;
		}
		//コメント
		if( aBuf[ 0] == '#')
		{
			fgets( aBuf, MAX_BUF, fp);
		}
		if( strcmp( aBuf, "MOTIONSET") == 0)
		{
			//モーション追加
			MOTION motion;
			
			//モーション情報(8)
			while( strcmp( aBuf, "END_MOTIONSET") != 0)
			{
				if( fscanf( fp, "%s", aBuf) == EOF)
				{
					MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(8)\n", "エラー", MB_OK);
					fclose(fp);
					return;
				}
				//コメント
				if( aBuf[ 0] == '#')
				{
					fgets( aBuf, MAX_BUF, fp);
				}
				//ループフラグ
				if( strcmp( aBuf, "LOOP") == 0)
				{
					fscanf( fp, "%s", aBuf);
					fscanf( fp, "%d", &motion.nLoop);

				}
				//キー数
				if( strcmp( aBuf, "NUM_KEY") == 0)
				{
					fscanf( fp, "%s", aBuf);
					fscanf( fp, "%d", &motion.nNumKey);

					//キーセット生成
					motion.vecKeySet.resize( motion.nNumKey);

					//キーセット情報(9)
					int nCntKeySet = 0;		//キーセットカウンタ
					while( strcmp( aBuf, "END_MOTIONSET") != 0)
					{
						if( fscanf( fp, "%s", aBuf) == EOF)
						{
							MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(9)\n", "エラー", MB_OK);
							fclose(fp);
							return;
						}
						//コメント
						if( aBuf[ 0] == '#')
						{
							fgets( aBuf, MAX_BUF, fp);
						}
						if( strcmp( aBuf, "KEYSET") == 0)
						{
							//キー生成
							motion.vecKeySet[ nCntKeySet].vecKey.resize( nNumParts);

							while( strcmp( aBuf, "END_KEYSET") != 0)
							{
								if( fscanf( fp, "%s", aBuf) == EOF)
								{
									MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(9)\n", "エラー", MB_OK);
									fclose(fp);
									return;
								}
								//コメント
								if( aBuf[ 0] == '#')
								{
									fgets( aBuf, MAX_BUF, fp);
								}
								//フレーム数
								if( strcmp( aBuf, "FRAME") == 0)
								{
									fscanf( fp, "%s", aBuf);
									fscanf( fp, "%d", &motion.vecKeySet[nCntKeySet].nFrame);
									break;
								}
							}

							//キーセット情報(10)
							int nCntKey = 0;		//キーカウンタ
							while( strcmp( aBuf, "END_KEYSET") != 0)
							{
								if( fscanf( fp, "%s", aBuf) == EOF)
								{
									MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(10)\n", "エラー", MB_OK);
									fclose(fp);
									return;
								}
								//コメント
								if( aBuf[ 0] == '#')
								{
									fgets( aBuf, MAX_BUF, fp);
								}
								//キー情報(11)
								if( strcmp( aBuf, "KEY") == 0)
								{
									while( strcmp( aBuf, "END_KEY") != 0)
									{
										if( fscanf( fp, "%s", aBuf) == EOF)
										{
											MessageBox( NULL, "model.cpp\nスクリプトが正しくありません(11)\n", "エラー", MB_OK);
											fclose(fp);
											return;
										}
										//コメント
										if( aBuf[ 0] == '#')
										{
											fgets( aBuf, MAX_BUF, fp);
										}
										//位置
										if( strcmp( aBuf, "POS") == 0)
										{
											Vector3 pos;
											fscanf( fp, "%s", aBuf);
											fscanf( fp, "%f", &pos.x);
											fscanf( fp, "%f", &pos.y);
											fscanf( fp, "%f", &pos.z);
											motion.vecKeySet[ nCntKeySet].vecKey[nCntKey].Position = pos;
										}
										//角度
										if( strcmp( aBuf, "ROT") == 0)
										{
											Vector3 rot;
											fscanf( fp, "%s", aBuf);
											fscanf( fp, "%f", &rot.x);
											fscanf( fp, "%f", &rot.y);
											fscanf( fp, "%f", &rot.z);
											motion.vecKeySet[ nCntKeySet].vecKey[nCntKey].Rotation = rot;
										}
									}

									nCntKey++;	//キーカウントアップ
								}
							}
							nCntKeySet++;		//キーセットカウントアップ
						}
					}
				}
			}
			vecMotion.push_back(motion);
			nCnt++;		//モーションカウントアップ
		}
	}
	
	//ファイルクローズ
	fclose(fp);

	//初期値代入
	m_nChangeFrame = vecMotion[ m_nNowMotion].vecKeySet[ m_nNowKey].nFrame;

	//角度修正
	vecOfsetRot[0].y += D3DX_PI;

	FileName = fileName;
}

/*------------------------------------------------------------------------------
	最初のモーション設定処理関数
------------------------------------------------------------------------------*/
void PartsAnimator::SetFirstMotion()
{
	//データからパーツの位置・角度設定
	int sizeParts = vecParts.size();
	for( int nCntParts = 0; nCntParts < sizeParts; nCntParts++)
	{
		vecParts[nCntParts]->m_pTransform->SetLocalPosition( vecMotion[ m_nNowMotion].vecKeySet[ m_nNowKey].vecKey[ nCntParts].Position);
		vecParts[nCntParts]->m_pTransform->SetLocalRotationEuler( vecMotion[ m_nNowMotion].vecKeySet[ m_nNowKey].vecKey[ nCntParts].Rotation);
	}
}

/*------------------------------------------------------------------------------
	次のモーション設定処理関数
------------------------------------------------------------------------------*/
void PartsAnimator::SetNextMotion( int nNextMotion, int nNextKey)
{
	//現在のモーション、キーを更新
	m_nNowMotion = m_nNextMotion;
	m_nNowKey = m_nNextKey;

	//次のモーション、キーを設定
	m_nNextMotion = nNextMotion;
	m_nNextKey = nNextKey;

	//フレーム設定
	m_nChangeFrame = m_nCntFrame + TIME_CHANGE;
}

/*------------------------------------------------------------------------------
	モーション切り替え処理関数
------------------------------------------------------------------------------*/
void PartsAnimator::ChangeMotion( int nNextMotion, int nNextKey)
{
	//現在のモーション、キーを更新
	m_nNowMotion = m_nNextMotion;
	m_nNowKey = m_nNextKey;

	//次のモーション、キーを設定
	m_nNextMotion = nNextMotion;
	m_nNextKey = nNextKey;

	//フレーム数リセット
	m_nCntFrame = 0;
	m_nChangeFrame = vecMotion[ m_nNowMotion].vecKeySet[ m_nNowKey].nFrame;
}

/*------------------------------------------------------------------------------
	モーションが終了したか
------------------------------------------------------------------------------*/
bool PartsAnimator::IsEndMotion(void)
{
	auto motNow = vecMotion[ m_nNowMotion];
	if( motNow.nLoop == 1 || m_nNextKey + 1 < motNow.nNumKey || m_nCntFrame < m_nChangeFrame - 1)
	{
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void PartsAnimator::Load(Text& text)
{
	//textを読み進める
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "FileName")
		{
			text.ForwardPositionToNextWord();
			LoadAnimator( text.GetWord());
		}

		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	セーブ
------------------------------------------------------------------------------*/
void PartsAnimator::Save(Text& text)
{
	StartSave(text);

	text += "FileName " + FileName + '\n';

	EndSave( text);
}