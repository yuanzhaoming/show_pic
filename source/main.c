#include <stdio.h>


int main(int argc,char *argv[])
{
    int ret = 0;
    ret = sdl_init();
    if(ret <0 ){
        printf("sdl init error...\r\n");
        return -1;
    }

    


    return 0;
}














