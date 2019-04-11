#include "audio.h"
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


#define PCM_BUF_SIZE 4096
SDL_AudioSpec audio_spec_t;

static current_len = 0;


static int quit = 0;
static SDL_Event sdlevent;

static FILE *file;
char *g_buff;
char *pcm_chunk;

static int done = 0;
static int pos = 0;

void fill_audio_buffer (void *userdata, Uint8 * stream,
                                            int len)
{
    SDL_memset(stream,0,len);
    if(current_len==0){
        return ;
    }


    if(len!=PCM_BUF_SIZE){
        printf("len not equal PCM_BUF_SIZE\r\n");
    }


    pcm_chunk = &g_buff[PCM_BUF_SIZE*((pos+1)%4)];
    SDL_MixAudio(stream,pcm_chunk,len,SDL_MIX_MAXVOLUME);

    pos ++;
    if(pos == 4){
        pos = 0;
    }
    done = 1;
}

int init_audio(void)
{
    printf("play audio...\r\n");

    audio_spec_t.callback = fill_audio_buffer;
    audio_spec_t.channels = 2;
    audio_spec_t.format = AUDIO_S16LSB;
    audio_spec_t.freq = 44100;
    audio_spec_t.samples = 1024;
    audio_spec_t.silence = 0;
    if(SDL_OpenAudio(&audio_spec_t,NULL)<0){
        printf("cant't open audio device...\r\n");
        return -1;
    }

    file = fopen("me.wav","rb+");
    if(file == NULL){
        printf("open audio file error...\r\n");
        return -1;
    }
    g_buff = (char *)malloc(PCM_BUF_SIZE*4);
    memset(g_buff,0,PCM_BUF_SIZE*4);

    SDL_PauseAudio(0);   

    while(quit == 0){
        while (SDL_PollEvent(&sdlevent))
        {
            if (sdlevent.type == SDL_QUIT)
            {
                quit =1;
                break;
            }
        }         



        current_len = fread(&g_buff[PCM_BUF_SIZE*pos],1,PCM_BUF_SIZE,file);

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
















