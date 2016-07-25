/*
 * 注意：中間発表でのBPM算出間隔は"4"秒！
 * 学会にて8秒
 * 本番24秒(仮)
 */

#include <iostream>
#include <FK/FK.h>
//#include <sstream>
#include <thread>
#include <chrono>

#include "TimeMeasurement.h"
#include "SoundManager.h"
#include "spWav.h"
#include <wiringPi.h>

#define WRITE_PIN_R 0	//dpio_num
#define WRITE_PIN_L 2	//dpio_num
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
	pinMode(WRITE_PIN_R, INPUT); //ピンを入力待ちに変更
	pinMode(WRITE_PIN_L, INPUT); //ピンを入力待ちに変更
	///ラズパイここまで
	
	///FK処理
	fk_AppWindow	window;
		
	window.setSize(1440,800);
	window.setBGColor(0.3,0.6,0.8);
	window.open();
	
	fk_Block	block(1.0,1.0,1.0);
	fk_Model	model, camera;
	fk_Vector	origin(0.0,0.0,0.0);
	model.setShape(&block);
	model.setMaterial(Yellow);
	window.setCameraModel(&camera);
	window.entry(&model);
	camera.glMoveTo(0,0,0);
	model.glMoveTo(0,-1.0,-10);
	
	float	y =1.0;//ブロックのサイズ
	int 	bpmNum = 60;

	
///FK処理ここまで

	bool		vol[2],flag[2];	//ラズパイスイッチ判定、状態確認
	vol[0]	=	vol[1]	=	flag[0]	=	flag[1]	=	false;
	

	
	//スレッド作成
	thread	timeThread;
	thread	selectThread;
	thread	playThread;

	TimeGet	timeMeasure;	//時間計算クラス
	SoundManager	soundManage;	//音再生クラス
	
//メモ* オブジェクト関数のスレッド化は (アドレス, オブジェクト,引数...)でいける、場合によってbindする？
	timeThread	=	thread (&TimeGet::BpmCompute,  &timeMeasure, INTERVAL, ref(mainFlag));//BPM算出をスレッド化
	selectThread	=	thread (&SoundManager::SelectMusic, &soundManage,ref(mainFlag), ref(timeMeasure.bpm), INTERVAL);	//曲機能をスレッド化
	playThread	=	thread (&SoundManager::PlayAudio, &soundManage,ref(mainFlag));	//音再生機能をスレッド化

	
	//メインループ
	while(window.update() == true)
	{
		
		///演出
		model.loRotateWithVec(origin, fk_Y,FK_PI/180);
		model.setScale(y * timeMeasure.GetBpm() / 100,fk_Y);
		///ここまで
		
		///状態の確認
		vol[0] = digitalRead(WRITE_PIN_R);
		vol[1] = digitalRead(WRITE_PIN_L);
		
		if(!vol[0] && flag[0])	flag[0] = false;//押されたときだけ
		if(!vol[1] && flag[1])	flag[1] = false;//押されたときだけ
		
		if(vol[0] && !flag[0]	||	vol[1] && !flag[1])
		{
//			y+=0.4;///演出
			timeMeasure.TimeUpdate();

			if(  bpmNum	!=	timeMeasure.GetBpm() ){
			
				fk_Window::printf("- -");
				fk_Window::printf("現在の走行テンポBPM : %d",timeMeasure.GetBpm());
				bpmNum = timeMeasure.GetBpm();
			}
			if (flag[0] == false)	flag[0] =true;
			if (flag[1] == false)	flag[1] =true;
		}
		
	}
	
	//終了時処理
	mainFlag = false;
	timeThread.join();
	selectThread.join();
	playThread.join();


	return 0;
}
