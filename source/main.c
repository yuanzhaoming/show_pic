#include <stdio.h>
#include "bmp.h"
#include "sdl_show.h"

unsigned char *pic;

int main(int argc,char *argv[])
{
    int ret = 0;
    char *file_path;
    int h,w;

    if(argc<2){
        printf("args not acceptale...\r\n");
        return -1;
    }

    file_path = argv[1];
    printf("file path:%s\r\n",file_path);
    pic=open_bitmap(file_path,&w,&h);

    ret =  sdl_init(w,h);
    if(ret <0 ){
        printf("sdl init error...\r\n");
        return -1;
    }

    put_pixels(pic);

    //draw_rectangle(200,200,400,400);
    sdl_refresh();


    return 0;
}














