#ifndef _avi_audio_video_h_
#define _avi_audio_video_h_


typedef struct _AVI_audio_T{
    int audio_chans;
    int mp3_rate;
    int audio_bits;
    int audio_format;
    int audio_rate;
    int audio_bytes;
    int audio_chunks;
}AVI_AUDIO_T;


typedef struct _AVI_video_T{
    int frame;
    int frames;
    int framew ;
    int frameh;
    int framerate;
}AVI_VIDEO_T;


typedef struct  _AVI_DATA_T
{
    AVI_AUDIO_T _AVI_AUDIO_t;
    AVI_VIDEO_T _AVI_VIDEO_t;
}AVI_DATA_T;



#endif


