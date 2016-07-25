






#include <stdio.h>
#include <string.h>

#include "spWav.h"


/*
 *
 */
 static int readfmtChunk(FILE *fp,tWaveFormatPcm* waveFmtPcm)
 {
    if(fread(waveFmtPcm, sizeof(tWaveFormatPcm), 1, fp) != 1)
        return -1;
   
    printf( "           データ形式: %u (1 = PCU)\n", waveFmtPcm->formatTag);
   
    printf( "         チャンネル数: %u\n", waveFmtPcm->channels);
   
    printf( "   サンプリング周波数: %lu [Hz]\n", waveFmtPcm->samplesPerSec);
   
    printf( "          バイト数/秒: %lu [bytes/sec]\n", waveFmtPcm->bytesPerSec);
   
    printf( "バイト数×チャンネル数: %u [bytes]\n", waveFmtPcm->blockAlign);
   
    printf( "    ビット数/サんプル: %u [bits/sample]\n",waveFmtPcm->bitsPerSample);
    
    if(waveFmtPcm->channels != 2)
    {
         fprintf(stderr,"\nこのプログラムはステレオのファイルを対象とします");
         fprintf(stderr," このwavファイルのチャンネル数は %d です. \n", waveFmtPcm->channels);
         return -1;
    }
    if(waveFmtPcm->formatTag != 1)
    {
        fprintf(stderr, "\nこのプログラムは無圧縮PCMのみを対象とします");
        fprintf(stderr, " このwavファイルのチャンネル数は %04X です. \n", waveFmtPcm->formatTag);
        return -1;
    }
    if(waveFmtPcm->bitsPerSample != 8 && waveFmtPcm->bitsPerSample != 16)
    {
        fprintf(stderr, "\nこのプログラムは8/16ビットサンプリングされたものを対象とします");
        fprintf(stderr, " このwavファイルのbits/secは %d です. \n", waveFmtPcm->bitsPerSample);
        return -1;
    }
    return 0;
 }


 /*
  *ファイル内容読み出し
  */
 int wavHdrRead(char *wavefile, SpParam* sp)
 {
       SWaveFileHeader  waveFileHeader;
       tWaveFormatPcm   waveFmtPcm;
       tChank           chank;
       long cursor, len;
       FILE *fp;

       if((fp = fopen(wavefile, "rb")) == NULL)
       {
           fprintf(stderr, " %sをオープンできません.\n", wavefile);
           return -1;   //error
       }
       printf("\n%s :\n", wavefile);

       //ヘッダー情報
       if(fread(&waveFileHeader, sizeof waveFileHeader, 1, fp) != 1)
       {
            fprintf(stderr, "%ld で読み込み失敗.\n", ftell(fp));
            fclose(fp);
            return -1;      //error
       }

        //WAVEヘッダ情報
        if(memcmp(waveFileHeader.hdrWave, STR_WAVE, 4) != 0)
        {
            fprintf(stderr, "'WAVE'がない,\n");
            fclose(fp);
            return -1;      //error
        }
        
        if(memcmp( &waveFileHeader.hdrRiff, STR_RIFF, 4) != 0)
        {
            fprintf(stderr, "'RIFF'フォーマットでない.\n");
            fclose(fp);
            return -1;      //error
        }
        
        //4Byte これ以降のバイト数 = (ファイルサイズ - 8)(Byte)
        len = waveFileHeader.sizeOfFile;

        //チャンク情報
        while(fread(&chank,sizeof chank, 1, fp) == 1)
        { 
          if(memcmp(chank.hdrFmtData,STR_fmt, sizeof chank.hdrFmtData) == 0)
          { 
              len=chank.sizeOfFmtData;
              fprintf(stderr, "\"fmt \"の長さ: %ld [bytes]\n\n", len );
              cursor = ftell(fp);
              if(readfmtChunk(fp, &waveFmtPcm) != 0){  return -1; }

                sp->samplesPerSec = waveFmtPcm.samplesPerSec;
                sp->bitsPerSample = waveFmtPcm.bitsPerSample;
                sp->channels=waveFmtPcm.channels;
                sp->bytesPerSec=waveFmtPcm.bytesPerSec;
                fseek(fp,cursor + len, SEEK_SET);
          }
          else if(memcmp(chank.hdrFmtData,STR_data, 4) == 0)
          {
            sp->sizeOfData=chank.sizeOfFmtData;
            fprintf(stderr, "\n\"data\" の長さ: %ld [bytes]\n", sp->sizeOfData);
            sp->posOfData=ftell(fp);
            fseek(fp, sp->sizeOfData + sp->posOfData - 4,SEEK_SET);
          }
          else
          {
            len=chank.sizeOfFmtData;
            printf( "\"%c%c%c%c\"の長さ: %ld [bytes]\n\n",
                       chank.hdrFmtData[0],chank.hdrFmtData[1],
                          chank.hdrFmtData[2],chank.hdrFmtData[3], len);
            cursor = ftell(fp);
            fseek(fp, cursor + len, SEEK_SET);
          }
        }
        fclose(fp);

        return 0;
}
         