#include "sdl_show.h"
#include <stdio.h>
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;

static int sdl_width = 0;
static int sdl_height = 0;

int sdl_init(int w,int h)
{
    int i = 0;

    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return -1;
    }     

    sdl_width = w;
    sdl_height = h;
    window = SDL_CreateWindow( "窗口名称", 100, 100, sdl_width, sdl_height, SDL_WINDOW_SHOWN );
 
    screenSurface = SDL_GetWindowSurface( window );



#if 0
    for(i = 0 ; i <1280 *240*1; i ++ )
        ((int *)screenSurface->pixels)[i] = 0xffff0000;
    for(i = 1280 *240*1 ; i <1280 *480*1; i ++ )
        ((int *)screenSurface->pixels)[i] = 0xff00ff00;
    for(i = 1280 *480*1 ; i <1280 *720*1; i ++ )
        ((int *)screenSurface->pixels)[i] = 0x0000ff;
#endif
    //SDL_UpdateWindowSurface( window );
    //SDL_Delay(4000);
    //SDL_DestroyWindow( window );
    //SDL_Quit();
    return 0;
}


void put_bitmap_pixels(unsigned char *pixels)
{
    int i = 0;
    for(i = 0 ; i < sdl_width * sdl_height ; i ++){
        ((int *)screenSurface->pixels)[i] = (pixels[3*i]<<16) | (pixels[3*i+1]<<8) | (pixels[3*i+2]<<0);
    }
}

void put_jpeg_pixels(unsigned char *pixels)
{
    int i = 0;
    for(i = 0 ; i < sdl_width * sdl_height ; i ++){
        ((int *)screenSurface->pixels)[i] = (pixels[3*i]<<16) | (pixels[3*i+1]<<8) | (pixels[3*i+2]<<0);
    }
}



void draw_rectangle(int x1,int y1,int x2,int y2)
{
    int x,y;
    for(x = x1; x<x2;x++){
        ((int *)screenSurface->pixels)[sdl_width*y1 + x] = 0x00ff0000;
    }
}




void sdl_refresh()
{
    SDL_UpdateWindowSurface( window );
    SDL_Delay(50);

}

void sdl_exit()
{
    SDL_DestroyWindow( window );
    SDL_Quit();
}


