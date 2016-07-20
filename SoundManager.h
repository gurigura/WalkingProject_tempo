#pragma once

#include <FK/FK.h>
#include <FK/Audio.h>
#include <iostream>
#include <chrono>
#include <vector>

class SoundManager
{
private:	
	
public:
	int	bpm;	//BPM
	int	beat;	//拍
	int	dramPattern;	//ドラムパターン

	fk_AudioOggBuffer		se[2];

	SoundManager();
	///SEを一定間隔で再生、間隔調整自動
	void	BpmSePlay(bool &, int &);	

};
