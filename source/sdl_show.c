#include "sdl_show.h"
#include <stdio.h>
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;

int sdl_init()
{
    int i = 0;

    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return -1;
    }     

    window = SDL_CreateWindow( "窗口名称", 100, 100, 1280, 720, SDL_WINDOW_SHOWN );
 
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


void put_pixels(int *pixels,int w,int h)
{
    int i = 0;


}


void sdl_refresh()
{
    SDL_UpdateWindowSurface( window );
}


