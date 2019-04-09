//#include "jpeg.h"
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <string.h>

//#include <jpeg.h>

static FILE *file;
int row_stride;
JSAMPARRAY buffer;
struct jpeg_decompress_struct cinfo;
struct jpeg_error_mgr jerr;

unsigned char *data;

int init_jpeg(char *path,int *w,int *h)
{
    int i = 0;


    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    if ((file = fopen(path, "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", path);
        return -1;
    }

    jpeg_stdio_src(&cinfo, file);
    // 用jpeg_read_header获得jpg信息
    jpeg_read_header(&cinfo, TRUE);
    
    printf("image_width = %d\n", cinfo.image_width); 
    printf("image_height = %d\n", cinfo.image_height); 
    printf("num_components = %d\n", cinfo.num_components);

    // 设置解压参数,比如放大、缩小
    cinfo.scale_num = 1;
    cinfo.scale_denom = 2;

    // 启动解压：jpeg_start_decompress   
    jpeg_start_decompress(&cinfo);

    printf("output_width = %d\n", cinfo.output_width); 
    printf("output_height = %d\n", cinfo.output_height); 
    printf("output_components = %d\n", cinfo.output_components);//解压的是rgb，故为3元素
  
    row_stride = cinfo.output_width * cinfo.output_components;
    
    data = malloc(row_stride*cinfo.output_height);
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    while (cinfo.output_scanline < cinfo.output_height) { 
        (void) jpeg_read_scanlines(&cinfo, buffer, 1); 
        //printf("cinfo.output_scanline=%d\r\n",cinfo.output_scanline);
        #if 0
        for(i = 0 ; i < cinfo.output_width ; i ++){
            data[(cinfo.output_scanline-1)*row_stride+3*i+0] = buffer[0][3*i+0];
            data[(cinfo.output_scanline-1)*row_stride+3*i+1] = buffer[0][3*i+1];
            data[(cinfo.output_scanline-1)*row_stride+3*i+2] = buffer[0][3*i+2];
        }
        #else
        memcpy(&data[(cinfo.output_scanline-1)*row_stride],buffer[0],row_stride);
        #endif
         
    }


    jpeg_finish_decompress(&cinfo);

    *w = cinfo.output_width;
    *h = cinfo.output_height;

    printf("sdl w:%d\r\n",*w);
    printf("sdl h:%d\r\n",*h);
    return 0;
}

unsigned char  *jpeg_get_pixel()
{
    return data;
}

void free_jpeg_resource()
{
    free(buffer);
    
    jpeg_destroy_decompress(&cinfo);
}




