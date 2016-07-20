
#include "SoundManager.h"
#include <FK/FK.h>
#include <FK/Audio.h>
#include <chrono>
#include <thread>


SoundManager::SoundManager(){
	bpm = 60;	//初期値
	beat = 1;
	dramPattern = 2;
	if(se[0].open("Cowbel.ogg") == false){	//音声読み込み
		return;
	}
	if(se[1].open("sunare.ogg") == false){	//音声読み込み
		return;
	}
	se[0].setGain(1.0);	//音量初期設定
	se[1].setGain(1.0);
}

void	SoundManager::BpmSePlay(bool &argFlag, int &argBpm )
{
			
	while(argFlag){
		bpm = argBpm;
		
		//4回に1回別音を鳴らす
		if(beat != 4)
		{
		se[0].seek(0.0);
		se[0].play();
		}else{
		se[1].seek(0.0);
		se[1].play();
		beat = 0;	
		}	
		beat++;
		
		if(bpm == 0) bpm = 60;
		//仮ドラムパターン決定ポイント
		if(bpm <=100 )dramPattern = 1;
		else    dramPattern = 2;
		
		
		//待ち時間算出	　 1000ミリ秒  / (BPM/60秒)秒間拍→ ミリ秒あたりでの待ち時間算出
		int	waitTime=0;
		if(dramPattern == 1)
		{
			waitTime	=	1000 / (bpm / 60.0);
		}
		else if(dramPattern == 2)
		{
			if(beat == 1)	waitTime	=	1000 / (bpm / 60.0);
			if(beat == 2)	waitTime	=	1000 / (bpm/ 60.0);
			if(beat == 3)	waitTime	=	1000 / (bpm/ 90.0);
			if(beat == 4)	waitTime	=	1000 / (bpm / 30.0);
		}
			std::this_thread::sleep_for(std::chrono::milliseconds(waitTime))	;
		
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000))	;
	}
}
