/*
 *
 * 
 *
 *
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

 #include "spWav.h"


/*
 *wavヘッダ書き込み
 */
 long wavHeaderWrite(FILE *fp,SpParam* sp)
 {
 	unsigned	short	bytes;
 	WrSWaveFileHeader wrWavHdr;
 	
 	strncpy(wrWavHdr.hdrRiff,STR_RIFF,sizeof wrWavHdr.hdrRiff);

 	wrWavHdr.sizeOfFile=sp->sizeOfData + sizeof(wrWavHdr) - 8;	

 	strncpy(wrWavHdr.hdrWave,STR_WAVE,sizeof wrWavHdr.hdrWave);

 	strncpy(wrWavHdr.hdrFmt,STR_fmt,sizeof wrWavHdr.hdrFmt);

 	wrWavHdr.sizeOfFmt= sizeof wrWavHdr.stWaveFormat;

 	wrWavHdr.stWaveFormat.formatTag=1;

 	wrWavHdr.stWaveFormat.channels=sp->channels;

 	wrWavHdr.stWaveFormat.samplesPerSec=sp->samplesPerSec;

 	bytes = sp->bitsPerSample / 8;
 	wrWavHdr.stWaveFormat.bytesPerSec = bytes * sp->channels * sp->samplesPerSec;



 	wrWavHdr.stWaveFormat.blockAlign=bytes * sp->channels;

 	wrWavHdr.stWaveFormat.bitsPerSample=sp->bitsPerSample;

 	strncpy(wrWavHdr.hdrData,STR_data,sizeof wrWavHdr.hdrData);

 	wrWavHdr.sizeOfData=sp->sizeOfData;

	fwrite(&wrWavHdr, sizeof wrWavHdr, 1, fp);

	return ftell(fp); 	
 }

