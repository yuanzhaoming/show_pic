#include "avi_audio_video.h"
#include "avilib.h"
#include "SDL2/SDL.h"
#include "myjpeg.h"

extern AVI_DATA_T AVI_DATA_t;
extern avi_t *avi_file;


#define PCM_BUF_SIZE 4096
SDL_AudioSpec audio_spec_t;

static current_len = 0;

char *audio_buf;
char *video_buf;

static int quit = 0;
static SDL_Event sdlevent;

static FILE *file;
char *pcm_chunk;

static int done = 0;
static int pos = 0;

static int current_audio_frames = 0;

static void fill_audio_buffer (void *userdata, Uint8 * stream,
                                            int len)
{
    SDL_memset(stream,0,len);
    if(current_len==0){
        return ;
    }

   // printf("len:%d\r\n",len);

    pcm_chunk = &audio_buf[384*((pos+1)%4)];
    SDL_MixAudio(stream,pcm_chunk,len,SDL_MIX_MAXVOLUME);

    pos ++;
    if(pos == 4){
        pos = 0;
    }
    done = 1;
}

 int init_avi_audio( )
{
    printf("play audio...\r\n");

    audio_spec_t.callback = fill_audio_buffer;
    audio_spec_t.channels = 2;
    audio_spec_t.format = AUDIO_S16MSB;
    audio_spec_t.freq = 48000;
    audio_spec_t.samples = 96;
    audio_spec_t.silence = 0;

    if(SDL_OpenAudio(&audio_spec_t,NULL)<0){
        printf("cant't open audio device...\r\n");
        return -1;
    }

    audio_buf = (char *)malloc(PCM_BUF_SIZE*4);
    video_buf = (char *)malloc(PCM_BUF_SIZE*1024);
    memset(audio_buf,0,PCM_BUF_SIZE*4);

    SDL_PauseAudio(0);

    if (AVI_seek_start( avi_file )) //寻找文件开始 
    {
            printf("bad seek start/n");             
            return -2; 
    }

    while(quit == 0){
        while (SDL_PollEvent(&sdlevent))
        {
            if (sdlevent.type == SDL_QUIT)
            {
                quit =1;
                break;
            }
        }         


        if(current_audio_frames < AVI_DATA_t._AVI_VIDEO_t.frames){
            int key_frames = 0;
            AVI_set_video_position(avi_file, current_audio_frames);//将avi文件移动到第fream帧
            AVI_set_audio_position(avi_file,current_audio_frames);
            long video_bytes = AVI_frame_size(avi_file,current_audio_frames);//获取视频大小
            long audio_bytes = AVI_audio_size(avi_file,current_audio_frames);//获取音频大小
            long actual_video_size = AVI_read_frame(avi_file,(char *)video_buf,&key_frames);//读取视频帧数据

            int ret = AVI_read_audio(avi_file, &audio_buf[384*pos],audio_bytes); //读取音频帧数据

            if(ret < 0 ){
                printf("read audio failed...\r\n");
            }

            //printf("audio:%d,%d,%d,%d\r\n",audio_buf[384*pos+0],audio_buf[384*pos+1],audio_buf[384*pos+2],audio_buf[384*pos+3]);




            init_jpeg_from_buffer(video_buf);

            current_len = audio_bytes;

            if(audio_bytes!=384){
                printf("not equal 384!!!\r\n");
                return -1;
            }

           // printf("actual_video_size:%d,video_buf[0]:%0x,video_buf[1]:%0x,%0x,%0x\r\n",
            //    actual_video_size,video_buf[0],video_buf[1],audio_buf[0],audio_buf[1]);

            current_audio_frames++;
        }
        //current_len = fread(&audio_buf[PCM_BUF_SIZE*pos],1,PCM_BUF_SIZE,file);

        //printf("current_len:%d\r\n",current_len);
        while(done == 0)
        {   
            if(current_len == 0)
            {
                printf("audio finished...\r\n");
            }
             
        }
        done = 0;
    }
}







