
#ifndef _SPWAV_HEADER_

#define _SPWAV_HEADER_

#include <stdio.h>

#ifndef _MAX_PATH
#define _MAX_PATH (255)
#endif

#pragma pack(push,1)

typedef struct tagSWaveFileHeader
{
    char            hdrRiff[4]; //'RIFF'
    unsigned int    sizeOfFile; //ファイルサイズ-8 
    char            hdrWave[4]; //'WAVE'
} SWaveFileHeader;

typedef struct tagChank
{
    char   hdrFmtData[4];  //'fmt' or 'data'
    unsigned int    sizeOfFmtData;  //sizeof{PCMWAVEFORMAT} or Waveデータサイズ
} tChank;

typedef struct tagWaveFormatPcm
{
    unsigned short  formatTag;      //WAVE_FORMAT_PCM
    unsigned short  channels;       //number of channels
    unsigned int    samplesPerSec;  //sampling rate
    unsigned int    bytesPerSec;    //samplesPerSec* channels * [bitsPerSample/8]
    unsigned short  blockAlign;     //block align
    unsigned short  bitsPerSample;  //bit per sampling
} tWaveFormatPcm;

typedef struct tagWrSWaveFileHeader
{
    char   hdrRiff[4];     // 'RIFF'
    unsigned int    sizeOfFile;     // ファイルサイズ
    char   hdrWave[4];     // 'WAVE'
    char   hdrFmt[4];      // 'fmt'
    unsigned int    sizeOfFmt;      // sizeof(PCMWAVEORMAT)
    struct {
        unsigned short  formatTag;      // WAVE_FORMAT_PCM
        unsigned short  channels;       // number of channnel
        unsigned int    samplesPerSec;  // sampling rate
        unsigned int    bytesPerSec;    // samplesPerSec * channels * (bitsPerSample/8)
        unsigned short  blockAlign;     // block align
        unsigned short  bitsPerSample;  // bits per sampling
    } stWaveFormat;                     // PCMWAVEFORMAT
    char    hdrData[4];     // 'data'
    unsigned    int     sizeOfData;     // Waveデータサイズ
} WrSWaveFileHeader;

#pragma pack(pop)


//defines
#define STR_RIFF    "RIFF"
#define STR_WAVE    "WAVE"
#define STR_fmt     "fmt "
#define STR_data    "data"



#define WAV_MONAURAL    1
#define WAV_STEREO      2



//関数間の引数
typedef struct tagPram
{
    unsigned int    samplesPerSec;
    unsigned short  bitsPerSample;
    unsigned int    sizeOfData;
    unsigned short  channels;
    unsigned int    bytesPerSec;
    long            posOfData;
    int             start;
    int             end;
} SpParam;


//prototype
int     wavHdrRead(char *wavefile, SpParam* sp);
long    wavHeaderWrite(FILE *fp, SpParam* sp);
 void	timeStretch_Run(char *input, char *output,float speed);


#endif  /*_SPWAV_HEADER_*/


