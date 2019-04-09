#ifndef _sdl_show_h
#define _sdl_show_h


extern void put_bitmap_pixels(unsigned char *pixels);
extern void sdl_refresh();
extern int sdl_init(int w,int h);
extern void put_jpeg_pixels(unsigned char *pixels);

#endif

