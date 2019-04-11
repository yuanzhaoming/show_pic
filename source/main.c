#include <stdio.h>
#include "bmp.h"
#include "sdl_show.h"
#include "myjpeg.h"

#include "SDL2/SDL.h"

SDL_Event sdlevent;

unsigned char *pic;
unsigned char *data;

int main(int argc,char *argv[])
{
    int ret = 0;
    char *file_path;
    int h,w;
    int quit = 0;
#if 1
    init_video_device(&w,&h);

    ret =  sdl_init(w,h);
    if(ret <0 ){
        printf("sdl init error...\r\n");
        return -1;
    }

    init_audio();

#if 0
    pic = malloc(w*h*12);
    data = malloc(w*h*12);

    while(quit == 0){

        while (SDL_PollEvent(&sdlevent))
        {
            if (sdlevent.type == SDL_QUIT)
            {
                quit =1;
                break;
            }
        }
        video_get_data(pic);
        init_jpeg_from_buffer(pic);

        data = jpeg_get_pixel();

        put_jpeg_pixels(data);
        sdl_refresh();  
    }
 #endif

#else
    file_path = argv[1];
    printf("file path:%s\r\n",file_path);

    init_jpeg(file_path,&w,&h);

    ret =  sdl_init(w,h);
    if(ret <0 ){
        printf("sdl init error...\r\n");
        return -1;
    }


    pic=jpeg_get_pixel();
    put_jpeg_pixels(pic);
    //put_bitmap_pixels(pic);
    sdl_refresh();
 #endif

    return 0;
}














