#include "wav.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>

#include "SDL2/SDL.h"
/*
*   chunk:
*           id
*           size
*           data
*/

/**
 *   reference: the canonical wave format 
 *   
 * 
 * 
 * 
 * 
 */
WAVE_T WAVE_t;

static SDL_Event wave_sdlevent;
#define WAVE_PCM_BUF_SIZE 4096
SDL_AudioSpec wav_audio_spec_t;
unsigned char *wave_audio_buf;

static int audio_bytes;

static int wave_quit;
static int current_audio_frames;
static int done = 0;

static int wave_fd;

static int wave_pos = 3;

static unsigned int wave_audio_size = 0;

int wave_get_id(int fd,char *id)
{
    /*id len=4 fixed*/
    int len = read(fd,id,4);
    if(len < 4){
        printf("len error...\r\n");
        return -1;
    }
    return len;
}

unsigned int wave_get_size(int fd)
{
    unsigned int file_len;
    int len = 0;
    len = read(fd,&file_len,4);
    if(len < 4){
        printf("len error...\r\n");
    }
    return file_len;
}



unsigned int wave_get_short(int fd)
{
    unsigned int file_len;
    int len = 0;
    len = read(fd,&file_len,2);
    if(len < 2){
        printf("len error...\r\n");
    }
    return file_len;
}


unsigned int wave_get_char(int fd)
{
    unsigned int file_len;
    int len = 0;
    len = read(fd,&file_len,1);
    if(len < 1){
        printf("len error...\r\n");
    }
    return file_len;
}

unsigned int wave_get_int(int fd)
{
    unsigned int file_len;
    int len = 0;
    len = read(fd,&file_len,4);
    if(len < 4){
        printf("len error...\r\n");
    }
    return file_len;
}

void wave_get_audio_data(int fd,unsigned char *buf,int datalen)
{
    int len = 0;
    len = read(fd,buf,datalen);
}

unsigned char  *pcm_chunk;


static void fill_wave_audio_buffer (void *userdata, Uint8 * stream,
                                            int len)
{
    
    SDL_memset(stream,0,len);
    
    if(len != wav_audio_spec_t.samples*4){
        printf("len error:%d\r\n",len);
    }
   

    pcm_chunk = &wave_audio_buf[ wav_audio_spec_t.samples*4*(wave_pos%4)];   
    SDL_MixAudio(stream,pcm_chunk,len,SDL_MIX_MAXVOLUME);

    done = 1;
}

 
int init_wav_audio( )
{
    printf("play wav audio...\r\n");

    static unsigned int loop_count = 0;

    wav_audio_spec_t.callback = fill_wave_audio_buffer;
    wav_audio_spec_t.channels = 2;
    wav_audio_spec_t.format = AUDIO_S16SYS;
    wav_audio_spec_t.freq = 48000;
    wav_audio_spec_t.samples = 1024;
    wav_audio_spec_t.silence = 0;

    if(SDL_OpenAudio(&wav_audio_spec_t,NULL)<0){
        printf("cant't open audio device...\r\n");
        return -1;
    }

    wave_audio_buf = (unsigned char *)malloc(WAVE_PCM_BUF_SIZE*4);
    memset(wave_audio_buf,0,WAVE_PCM_BUF_SIZE*4);

    SDL_PauseAudio(0);

    printf("wave_audio_size:0x%0x\r\n",wave_audio_size);
    

    while(wave_quit == 0){
        while (SDL_PollEvent(&wave_sdlevent))
        {
            if (wave_sdlevent.type == SDL_QUIT)
            {
                wave_quit =1;
                break;
            }
        }         

        if( loop_count * wav_audio_spec_t.samples * 4 < wave_audio_size){
            if(done == 1){
                done = 0;

                wave_get_audio_data(wave_fd,&wave_audio_buf[wave_pos*wav_audio_spec_t.samples*4],wav_audio_spec_t.samples*4);
                wave_pos ++;
                wave_pos = wave_pos % 4;

                loop_count++;
            }
            else{
                 
            }
        }
        else{
            wave_quit = 1;
            break;
        }
       // SDL_Delay(1);
    }
    printf("wave finished quit...\r\n");
    SDL_PauseAudio(1);
    free(wave_audio_buf);

    SDL_CloseAudio();

     
    return 0;
}


int open_wav(char *path)
{   
    char id[5];
    int i;
    unsigned int chunksize;
    unsigned int riffsize;
    memset(id,0,sizeof(id));
    wave_fd = open(path,O_RDONLY);
    if(wave_fd<0){
        printf("open wave file error...\r\n");
        return -1;
    }

    wave_get_id(wave_fd,id);
    printf("id=%s\r\n",id);
    riffsize = wave_get_size(wave_fd);
    printf("riffsize=0x%0x\r\n",riffsize);

    wave_get_id(wave_fd,id);
    printf("id=%s\r\n",id);
    wave_get_id(wave_fd,id);
    printf("id=%s\r\n",id);

    chunksize = wave_get_size(wave_fd);
    printf("chunksize=0x%0x\r\n",chunksize);

    WAVE_t.Audio_Format=wave_get_short(wave_fd);
    WAVE_t.Num_Channels=wave_get_short(wave_fd);
    WAVE_t.Sample_Rate=wave_get_int(wave_fd);
    WAVE_t.Byte_Rate=wave_get_int(wave_fd);
    WAVE_t.BLock_Align=wave_get_short(wave_fd);
    WAVE_t.BitsPerSample=wave_get_short(wave_fd);

    printf("WAVE_t.Audio_Format=0x%0x\r\n",WAVE_t.Audio_Format);
    printf("WAVE_t.Num_Channels=0x%0x\r\n",WAVE_t.Num_Channels);
    printf("WAVE_t.Sample_Rate=%d\r\n",WAVE_t.Sample_Rate);
    printf("WAVE_t.Byte_Rate=%d\r\n",WAVE_t.Byte_Rate);
    printf("WAVE_t.BLock_Align=0x%0x\r\n",WAVE_t.BLock_Align);
    printf("WAVE_t.BitsPerSample=%d\r\n",WAVE_t.BitsPerSample);

    if(chunksize>16){
        printf("has reserved val\r\n");
        for(i = 0 ; i <  chunksize-16; i ++){
            wave_get_char(wave_fd);
        }
    }

    while(1){
        wave_get_id(wave_fd,id);
        printf("id=%s\r\n",id);   
        chunksize = wave_get_size(wave_fd);
        printf("chunksize=0x%0x\r\n",chunksize);

        if(strncmp(id,"data",4)==0){
            printf("get data chunk...\r\n");
            wave_audio_size = chunksize;
            break;
        }
        for(i = 0 ; i <  chunksize; i ++){
            wave_get_char(wave_fd);
        }
    }

    
    init_wav_audio();
}











