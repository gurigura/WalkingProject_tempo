//時間計測

#pragma once

#include <iostream>
#include <chrono>
#include <vector>

class TimeGet
{
private:
	std::chrono::system_clock::time_point	start,end;//時間取得保存先
	double	init;//rasPi時間"謎"初期値格納
	float	time;//一歩の時間差
	
public:
	TimeGet();//コンストラクタ
	
	int	bpm;	//算出されたBPM格納
	std::vector<float> timeHanger;	//1ターン内の時間差保管庫	*ターン切り替えの際の着地が算出より先に来る場合問題を相談
	
	void	TimeUpdate();
	void	BpmCompute(int , bool &);
	double	GetTime();
	int	GetBpm();
	
	
};


