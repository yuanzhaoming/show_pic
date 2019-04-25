#ifndef _wave_h_
#define _wave_h_


typedef struct _WAVE_T
{
    unsigned short Audio_Format;/*1.pcm  2.microsoft adpcm 3.ieee float 4.a-law etc*/
    unsigned short Num_Channels;
    unsigned int Sample_Rate;
    unsigned int Byte_Rate;
    unsigned short BLock_Align;    
    unsigned short BitsPerSample;  
}WAVE_T;


#endif

