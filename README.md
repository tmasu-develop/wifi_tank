# Wi-Fi Tank

## Overview
ESP-WROOM-02 マイコンを利用した、Wi-Fiでコントロールするリモコンタンクです。

## Description
ESP-WROOM-02 マイコンを利用した、Wi-Fiでコントロールするリモコンタンクです。 

ESP-WROOM-02 がアクセスポイントになり、スマートフォンを接続、192.168.10.1 にスマートフォンのブラウザでアクセスすると、コントロール画面が表示されます。

画面は html/css/javascript で記述されており、WebSocket を使用してリアルタイムにモーターを回すことができます。

### HW構成

- マイコン 秋月電子 ESP-WROOM-02開発ボード
- モータードライバ 秋月電子 DRV8835使用ステッピング&DCモータードライバモジュール
- ミニブレッドボードx2
- 秋月電子 電源用マイクロＵＳＢコネクタＤＩＰ化キット
- LEDx1
- モバイルバッテリー Anker  PowerCore Fusion 5000
- USBケーブルx2

メカ

- タミヤ ユニバーサルプレート 2枚セット
- タミヤ ツインモーターギアボックス
- タミヤ トラック&ホイールセット
- 樹脂スペーサー メスxメス 30mm x 4
- M3 10mm 鍋ネジ x4 



## Demo
最近はアニメGIFなどを貼付けて実際の動作例を見せる

## Requirement


## Usage

以下の手順でWi-Fi tankをコントロールすることができます。
1. 電源を on にする
1. LEDが点灯するまで1秒程度待つ
1. スマートフォンで Wi-Fi アクセスポイントに接続する。 SSIDは「ESP8266-XXXX」(XXXXはマイコンボードにより異なる)パスワードはソースコードに記述のものを使用すること
1. スマートフォンのブラウザにて、`http://192.168.10.1` にアクセスすると、コントロール画面が表示される

コントロール画面には四つのボタンがあり、それぞれ

- RF 右クローラー前進
- RR 右クローラー後進
- LF 左クローラー前進
- LR 左クローラー後進

となっています。
左右のボタンを同時に押すことで前進、後進、右折、左折ができるようになっています。

※前後のボタンを同時に押してもどちらかしか反応しません。


## Install
### hardware

配線は以下の通りです

|ESP-WROOM-02||
|:--|:--|
|PIN3|DRV8835-BIN1|
|PIN4|DRV8835-AIN1|
|PIN5|DRV8835- AIN2|
|PIN6|DRV8835-BIN2|
|PIN19|LED+|
|PIN20|LED-|
|MicroUSB|電源|

|DRV8835||
|:--|:--|
|VM|電源+|
|AOUT1|Aモーター1|
|AOUT2|Aモーター2|
|BOUT1|Bモーター2|
|BOUT2|Bモーター1|
|GND|電源-|
|BIN2|ESP-WROOM-02 PIN6|
|BIN1|ESP-WROOM-02 PIN3|
|AIN2|ESP-WROOM-02 PIN5|
|AIN1|ESP-WROOM-02 PIN4|
|MODE||
|VCC|電源+|

### Software
前提条件

- Arduino IDEがインストールされていること
- Arduino IDEにESP8266開発環境がインストールされていること
- ESP-WROOM-02用SPIFFSアップローダーがインストールされていること

インストール方法

- ESP8266ボードが設定されたArduino開発環境にてプロジェクトを開く
- ESP-WROOM-02にプログラムを書き込む
- SPIFFSアップローダーでSPIFFSにhtmlファイルを書き込む


### Licence
This software is released under the MIT License, see LICENSE.txt

### Author
tmasu-develop
