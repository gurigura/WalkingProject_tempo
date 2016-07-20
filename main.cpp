/*
 * 注意：中間発表でのBPM算出間隔は"4"秒！
 */

#include <iostream>
#include <FK/FK.h>
#include <thread>
#include <chrono>


#include "TimeMeasurement.h"
#include "SoundManager.h"
#include <wiringPi.h>

#define WRITE_PIN 0	//dpio_num
#define INTERVAL 4	//BPM算出間隔秒数
using namespace std;



int main(int,char **)
{
	//本システムの動作フラグ
	bool	mainFlag = true;
	
	///ラズベリーパイの処理
	if(wiringPiSetup() == -1){	//wiringPiを読み込めなかったときのエラー処理
		cout << "not_wiringPi_road" << endl;
		return 0;
	}
	pinMode(WRITE_PIN, INPUT); //ピンを入力待ちに変更
	///ラズパイここまで
	
	///FK処理
	fk_AppWindow	window;
		
	window.setSize(800,600);
	window.setBGColor(0.3,0.6,0.8);
	window.open();

	fk_Sphere	sphere(16,2.0);
	fk_Block	block(1.0,1.0,1.0);
	fk_Model	model,camera;
	fk_Vector	origin(0.0,0.0,0.0);
	model.setShape(&block);
	model.setMaterial(Yellow);
	window.setCameraModel(&camera);
	window.entry(&model);
	camera.glMoveTo(0,0,0);
	model.glMoveTo(0,-2.5,-10);
	
	float	y =2;//ブロックのサイズ
	int 	bpmNum = 60;

	
///FK処理ここまで

	bool		vol,flag;	//ラズパイスイッチ判定、状態確認
	vol	=	flag	=	false;
	thread	timeThread;
	thread	soundThread;

	TimeGet	timeMeasure;	//時間計算クラス
	SoundManager	soundManage;	//音再生クラス
	
//メモ* オブジェクト関数のスレッド化は (アドレス, オブジェクト,引数...)でいける、場合によってbindする？
	timeThread	=	thread (&TimeGet::BpmCompute,  &timeMeasure, INTERVAL, ref(mainFlag));//BPM算出をスレッド化
	soundThread	=	thread (&SoundManager::BpmSePlay, &soundManage,ref(mainFlag),ref(timeMeasure.bpm));	//音再生機能をスレッド化

	
	//メインループ
	while(window.update() == true)
	{
		
		///演出
		model.loRotateWithVec(origin, fk_Y,FK_PI/90);
		model.setScale(y,fk_Y);
		if(y >= 0){
			y -= 0.02;
		}
		///ここまで
		
		vol = digitalRead(WRITE_PIN);
		
		if(!vol && flag)	flag = false;//押されたときだけ
		
		if(vol && !flag)
		{
			y+=0.4;///演出
			timeMeasure.TimeUpdate();

			if(  bpmNum	!=	timeMeasure.GetBpm() ){
				fk_Window::printf("- -");
				fk_Window::printf("現在のBPM : %d",timeMeasure.GetBpm());
				bpmNum = timeMeasure.GetBpm();
			}
			flag =true;
		}
		
	}
	
	//終了時処理
	mainFlag = false;
	timeThread.join();
	soundThread.join();


	return 0;
}
