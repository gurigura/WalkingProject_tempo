
#include "SoundManager.h"


using	namespace	std;

SoundManager::SoundManager(){
	bpm = 60;	//初期値s
	musicId = 0;	//初期再生曲ID
	switchId = 0; //最初に流すwav
	playTime = 0.0;
	magnification	=	1.0f;	//BPM変化倍率
	stretchBooting = false;
	music[0].setLoopMode(true);
	music[1].setLoopMode(true);
	ifstream	ifs;
	string	line;
	
	if(music[0].open("Music/out1.wav") == false){
		return;
	}
	if(music[1].open("Music/out2.wav") == false){
		return;
	}
	
	ifs.open("MusicList.txt",ios::in);
	int i=0;
	while(!ifs.eof()){
		getline(ifs,line);
		int num	=	stoi(line);
		bpmList.push_back(num);
		cout << bpmList[i] << endl;
		i++;
	}
	
	ifs.close();
}

///曲の変更
void	SoundManager::SelectMusic(bool &argFlag, int &argBpm , int interval){			
	while(argFlag){
		
			int changeCheack = musicId;	//現在曲ID
		
			//次曲の探索(BPMのもっとも近い曲)
			int minBpm = 100;//最低BPM差
			//配列要素数分のループ
			for(int i = 0; i< bpmList.size(); i++){
				int	diff = argBpm - bpmList[i];
				if(minBpm > abs(diff)){
					minBpm = diff;
					musicId = i;
				}
			}
			
			//曲が変わったか確認
			if(musicId == changeCheack) musicChangeFlag = false;
			else    musicChangeFlag = true;
				
			
			//BPMの計算
	//	magnification = (float)argBpm / (float)bpmList[musicId];
			if(argBpm >= 60)magnification = (float)argBpm / (float)bpmList[musicId];
			else if(argBpm<60) magnification = 1.0f;
			
			//現在の再生位置を保存
			playTime = music[switchId].tell();
			GenerateAudio();
				
		//確認用
			cout << "次曲の原曲BPMは" << bpmList[musicId] << "です" << endl;
			cout << "再生中のBPMは" << argBpm << "です" << endl;
	//			cout << "再生中のBPMは" << magnification << "倍です" << endl;
	
			stretchBooting = true;
			std::this_thread::sleep_for(std::chrono::seconds(interval));
		
	}
}

///再生速度を変更した曲の出力
void	SoundManager::GenerateAudio(){
	//曲の選別
	char input[30];
	
	switch(musicId){
			case	0:
				strcpy(input,"Music/Disco_A_Live.wav");
				break;
				
			case 1:
				strcpy(input,"Music/kabuki.wav");
				break;
				
			case 2:
				strcpy(input,"Music/Jingakuyou.wav");
				break;
			
			case	3:
				strcpy(input,"Music/Meat_Break.wav");
				break;
				
			case 4:
				strcpy(input,"Music/RicoMarch.wav");
				break;
				
			case 5:
				strcpy(input,"Music/spaceHop.wav");
				break;
/*			
			case	6:
				strcpy(input,"Music/Yuga.wav");
				break;
				
			case 7:
				strcpy(input,"Music/Stronger.wav");
				break;
				
			case 8:
				strcpy(input,"Music/kabuki.wav");
			break;
*/
	}
	
	//使用中のwavファイル
	char output[30];
	if(switchId == 1){
		strcpy(output,"Music/out1.wav");
		switchId = 0;
	}else{
		strcpy(output,"Music/out2.wav");
		switchId = 1;
	}
	
	float speed = magnification;
	timeStretch_Run(input, output, speed);
	music[switchId].open(output);
}

//曲再生のタイミング管理
void SoundManager::PlayAudio(bool &argFlag){

		while(argFlag){
					if(stretchBooting){			
							music[!(switchId)].stop();
							//再生位置調整
							if( musicChangeFlag ){ 
									music[switchId].seek(0.0); 
									cout << music[switchId].tell()<<"-------------"<< endl;
								}
							else{
								    music[switchId].seek(playTime * magnification);
								    cout << music[switchId].tell()<<"+++++++++++++"<< endl;
								     }
						 
						 stretchBooting	=	false;
					}
					
					music[switchId].play();
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	
}
