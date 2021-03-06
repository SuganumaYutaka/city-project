# city-project

## 概要
街の自動生成システムです。  
トポロジーやプロシージャルを学び開発しました。

## 主なシステム
交差点、道路、区画の定義  
土地の定義  
建物の生成  
建物の最適化  
交通システム  

## こだわった点
ハーフエッジデータ構造によって交差点、道路、区画を定義しています。  
建物一つ一つに生成ルールを発行して、プロシージャルに形状、屋根、壁、フロア、タイルが生成されます。  
最適化の際に形状同士で重なった見えない部分のタイルを削除して、一つの建物を一つの頂点バッファで表示しています。  
交通システムではハーフエッジデータ構造を活用して、車が道路や交差点を経由して他の車を認識するように設定し、車の数に応じて各処理が指数関数的に重くなることを防いでいます。    
生成された街を手続き（関数名と引数）単位で保存して復元できます。  

## 今後の課題
手動で街を改変できるようにする  
建物のタイルを3Dモデルにする  

## 操作方法
キーボード  
1キー 区画の分割  
2キー 建物の最適化  
3キー 建物の表示On/Off、土地・建物の生成    
4キー 建物のタイルを１枚ずつ表示    
5キー 車のOn/Off    
6キー リセット    
7キー 建物のワイヤーフレーム表示On/Off    
8キー セーブ    
9キー ロード    
Qキー 低品質シェーダー    
Wキー デフォルトシェーダー    
Eキー 高品質シェーダー    
マウス    
右クリック+移動 カメラ注視点移動    
ホイールクリック+移動 カメラ上下左右移動    
ホイールスクロール カメラ前後移動  

