
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spWav.h"

/*
 *wavデータ書き込み
 */
 int wavDataWrite(FILE *fpIn, FILE *fpOut, SpParam* sp)
 {
 	int rVal=0;
 	void* pMem=NULL;

 	fseek(fpIn, sp->posOfData, SEEK_SET);

 	if((pMem=malloc(sp->sizeOfData))==NULL)
 	{
 		printf("メモリが確保できないよー.\n");
 		return -1;
 	}
 	if(fread(pMem, sp->sizeOfData,1,fpIn) != 1)	//一回で全部読み込むよ
 	{
 		printf("読み込みに失敗.\n");
 		free(pMem);
 		return -1;
 	}

 	if(fwrite(pMem, sp->sizeOfData,1,fpOut) != 1)	//一回で全部読み込むよ
 	{
 		printf("読み込みに失敗.\n");
 		free(pMem);
 		return -1;
 	}
 	free(pMem);

 	return rVal;
 }


/*ファイル内容書き出し
 *
 *
 *sp:引数へのポインタ
 *
 */
int wavWrite(char *inFile, char *outFile, SpParam* sp)
{
	FILE *fpIn, *fpOut;

	if((fpOut = fopen(outFile,"wb"))==NULL)
	{
		printf("%s をオープンできませんでした.\n", outFile);
		return -1;
	}

	//wavヘッダ書き込み
	if(wavHeaderWrite(fpOut,sp) != 44)
	{
		printf("ヘッダを書き込めません : %s\n", outFile);
		fclose(fpOut);
		return -1;
	}

	if((fpIn = fopen(inFile,"rb")) == NULL)
	{
		fprintf(stderr, "%s をオープンできまんがな.\n", inFile);
		fclose(fpOut);
		return -1;
	}

	//wavデータ書き込み
	if(wavDataWrite(fpIn, fpOut, sp) != 0)
	{
		printf("waveDataWriteでエラー発生.\n");
		fclose(fpIn);
		fclose(fpOut);
		return -1;
	}

	fclose(fpIn);
	fclose(fpOut);

	return 0;
}


/*
 * usage
 */
void usage(void)
{
	fprintf(stderr,
		 "サンプリング周波数を変更します(へっだのみ).\n"
		 	"引数に<入力ファイル名> <出力ファイル名> <速度の倍率>\n"
		 	"を指定してください.\n\n"
		 	"例 : pgm in.wav out.wav 1.2\n");
}


/*
 * main
 */
 void timeStretch_Run(char *input,char *output, float speed)
 {
 	SpParam sp;
/*
 	if(argc != 4)
 	{
 		usage();
 		return -1;
 	}
*/
 	//read header of input wav file
// 	if( wavHdrRead(argv[0], &sp)!=0)
 	if( wavHdrRead(input, &sp)!=0)
 		printf("ここでエラー1");

 //	sp.samplesPerSec=(int)(atof(argv[2])*sp.samplesPerSec);
 	sp.samplesPerSec=(int)(speed*sp.samplesPerSec);
 	printf("\n%d [Hz]へ変換しました.\n", sp.samplesPerSec);
 
 	// write L,R wav files.
 //	if(wavWrite(argv[0],argv[1],&sp)!=0)
 	if(wavWrite(input,output,&sp)!=0)
 		printf("ここでエラー2");
 		
 	printf("\n%s を %s へ変換しました.\n", input, output);
 }
