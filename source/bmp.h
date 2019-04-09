#ifndef _bmp_h
#define _bmp_h


typedef unsigned char   uint8_t;     
typedef unsigned short  uint16_t;   
typedef unsigned int    uint32_t; 
typedef int             int32_t;  

typedef struct{ 
    uint8_t bfType[2];           //文件类型: "BM"/bmp, "BA"/.. , ... 
    uint32_t bfSize;             //整个文件的大小 
    uint16_t bfReserved1;        //保留: 0 
    uint16_t bfReserved2;        //保留: 0 
    uint32_t bfOffbits;          //文件数据从第几个字节开始 
}Bmp_FileHeader;


typedef struct{ 
    uint32_t biSize;             //该段占用字节数 
    uint32_t biWidth;            //图像宽度, 单位像素 
    int32_t biHeight;            //图像高度, 单位像素(数据为正时为倒向) 
    uint16_t biPlanes;           //平面数, 总是为1 
    uint16_t biBitCount;         //单位像素占用比特数: 1, 4, 8, 16, 24, 42 
    uint32_t biCompression;      //图像压缩方式: 0/BI_BGB 不压缩, //  1/BI_RLE8 8比特游程压缩, 只用于8位位图 //  2/BI_RLE4 4比特游程压缩, 只用于4位位图 //  3/BI_BITFIELDS 比特域, 用于16/32位位图 //  4/BI_JPEG 位图含有jpeg图像, 用于打印机 //  5/BI_PWG 位图含有pwg图像, 用于打印机 
    uint32_t biSizeImage;        //说明图像大小, 为BI_BGB时可能为0 
    int32_t biXPelsPerMeter;     //水平分辨率, 像素/米, 有符号整数 
    int32_t biYPelsPerMeter;     //垂直分辨率, 像素/米, 有符号整数 
    uint32_t biClrUsed;          //位图实际使用彩色表中的颜色索引数(为0表示使用所有) 
    uint32_t biClrImportant;     //图像显示有重要影响的颜色索引数 
}Bmp_Info;

extern unsigned char *open_bitmap(char *path,int *w,int *h);
#endif

