# robosys_device_driver

Device driver that can handle LED on Raspberry Pi.

## 動画
* URL - https://www.youtube.com/watch?v=E9WvXxnSEjY

## 使うもの
* Raspberry Pi 3 Model B
  * Raspbian
* LED
  * GPIO25, GNDに接続

## 使い方
1. リポジトリをcloneし、build
```
$ git clone https://github.com/takumo3o/robosys_device_driver.git
$ cd robosys_device_driver
$ bash maker.bash
```

2. 0~2の内の好きな数字を入力
```
$ echo 0 > /dev/myled0
$ echo 1 > /dev/myled0
$ echo 2 > /dev/myled0
```
  * 0の場合:チクワ
  * 1の場合:ダイコン
  * 2の場合:ギュウスジ

3. clean
```
$ bash cleaner.bash
```
  * build 後の led.c への変更の反映や、環境をきれいにしたい際に clean
