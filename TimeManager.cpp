#include "TimeManager.h"

//初期値
TimeGet::TimeGet(){
	bpm	=	60;
	start	= std::chrono::system_clock::now();
	end	=	std::chrono::system_clock::now();
	init 	= std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();//count()は数値の取得処理
}

///一歩にかかるの時間を取得
void TimeGet::TimeUpdate()
{
	end   = std::chrono::system_clock::now();
	time	= std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count(); //一歩の計測時間(現在の時間-最後に踏んだ時間)	
	time -= init; // 稀に現在時刻がメモリ位置を足した数値になるため、それの回避、様子を見て外す
	timeHanger.push_back(time);
			
	std::cout << "今押したステップ間隔  ::  " << time / 1000 << "秒" << std::endl;
	start = std::chrono::system_clock::now();
	return;
}

///ターン終了時にBPMを算出する 引数:第1.BPM算出間隔秒数
void  TimeGet::BpmCompute(int	sec, bool& argFlag)
{
	float	argBpm	=	0;
	while(argFlag){

//ここから変後部
		float	stepAvgTime	=	std::accumulate(timeHanger.begin(), timeHanger.end(), 0.0) / timeHanger.size(); //1ターンの歩数
		
//		argBpm	=	(60/sec) * steps; //BPM算出間隔秒数*1ターンの歩数
			argBpm	=	60/(stepAvgTime/1000); //BPM算出間隔秒数*1ターンの歩数
		bpm = (int)argBpm;	//算出したBPMを代入
		std::cout << bpm << "Check"<< stepAvgTime << std::endl;
//	ここまで変後部
		timeHanger.erase(timeHanger.begin(),timeHanger.end());	//要素全削除
		std::this_thread::sleep_for(std::chrono::seconds(sec));
	 }
	 return;
 }
	 
///一歩にかかる時間差を返り値で送る
double TimeGet::GetTime()
{
	return time;
}
	
///BPMを送る	
int TimeGet::GetBpm()
{
	return	bpm;
}	
