#pragma once

#include <FK/FK.h>
#include <FK/Audio.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <thread>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "spWav.h"

class SoundManager
{
private:	

public:
	int	bpm;	//BPM
	int	musicId;		//再生中の曲ID
	bool	musicChangeFlag;
	bool	switchId;		//out1.wavとout2.wavのどちらを再生しているか
	float magnification;	//サンプリング周波数の倍率
	double	playTime;	//再生中の位置
	bool	stretchBooting;	//タイムストレッチ出力が動いているときは曲変更を行わない
	std::vector<int>	bpmList;	//テキストにかかれた曲のBPM格納、楽曲順に入れる	
	
	fk_AudioWavBuffer	music[2];	//out1.wav, out2.wav
	
	SoundManager();	
	///SEを一定間隔で再生、間隔調整自動
//	void	BpmSePlay(bool &, int &);	

///曲選択：引数1:メインループフラグ, 2:BPM, 3:インターバル秒
	void	SelectMusic(bool &,	int &, int);
	void	GenerateAudio();
	///
	void PlayAudio(bool &);
};
