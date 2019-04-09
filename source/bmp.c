#include "bmp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define  BMP_FILEHEADER_SIZE    14 
#define  BMP_INFO_SIZE          40

FILE *fp;
Bmp_FileHeader bf;
Bmp_Info bi;
int perW, perWCount;
unsigned char *data,*pic;
int overLineBytesNum;
unsigned int overLineBytesSum;
int totalSize;
int picCount;

unsigned char *open_bitmap(char *path,int *w,int *h)
{
    int ret = 0;
    int i,j,k;
    unsigned char buffHeader[512];
    if((fp = fopen(path, "rb")) < 0)
    {   
        printf("bmp_get : open file %s failed\r\n", path);
        return NULL;
    }        
    if(fread(buffHeader, 1, BMP_FILEHEADER_SIZE, fp) <= 0){
        printf("bmp_get : read Bmp_FileHeader failed\r\n");
        fclose(fp);
        return NULL;       
    }
    bf.bfType[0] = buffHeader[0]; 
    bf.bfType[1] = buffHeader[1];

    bf.bfSize = buffHeader[2] + ((buffHeader[3]&0xFF)<<8) + ((buffHeader[4]&0xFF)<<16) + ((buffHeader[5]&0xFF)<<24);
    bf.bfOffbits = buffHeader[10] + ((buffHeader[11]&0xFF)<<8) + ((buffHeader[12]&0xFF)<<16) + ((buffHeader[13]&0xFF)<<24);

    if(bf.bfType[0] != 'B' || bf.bfType[1] != 'M') { 
        printf("bmp_get : bmp type err, bfType must be \"BM\"\r\n"); 
        fclose(fp); 
        return NULL; 
    }
    if(bf.bfOffbits - BMP_FILEHEADER_SIZE < BMP_INFO_SIZE || fread(buffHeader, 1, BMP_INFO_SIZE, fp) <= 0){
        printf("bmp_get : read Bmp_Info failed\r\n");
        fclose(fp);
        return NULL;
    }

    bi.biSize = buffHeader[0] + ((buffHeader[1]&0xFF)<<8) + ((buffHeader[2]&0xFF)<<16) + ((buffHeader[3]&0xFF)<<24); 
    bi.biWidth = buffHeader[4] + ((buffHeader[5]&0xFF)<<8) + ((buffHeader[6]&0xFF)<<16) + ((buffHeader[7]&0xFF)<<24);
    bi.biHeight = buffHeader[8] | ((buffHeader[9]&0xFF)<<8) | ((buffHeader[10]&0xFF)<<16) | ((buffHeader[11]&0xFF)<<24); 
    bi.biPlanes = buffHeader[12] + ((buffHeader[13]&0xFF)<<8); 
    bi.biBitCount = buffHeader[14] + ((buffHeader[15]&0xFF)<<8); 
    bi.biCompression = buffHeader[16] + ((buffHeader[17]&0xFF)<<8) + ((buffHeader[18]&0xFF)<<16) + ((buffHeader[19]&0xFF)<<24); 
    bi.biSizeImage = buffHeader[20] + ((buffHeader[21]&0xFF)<<8) + ((buffHeader[22]&0xFF)<<16) + ((buffHeader[23]&0xFF)<<24); 
    bi.biXPelsPerMeter = buffHeader[24] | ((buffHeader[25]&0xFF)<<8) | ((buffHeader[26]&0xFF)<<16) | ((buffHeader[27]&0xFF)<<24); 
    bi.biYPelsPerMeter = buffHeader[28] | ((buffHeader[29]&0xFF)<<8) | ((buffHeader[30]&0xFF)<<16) | ((buffHeader[31]&0xFF)<<24); 
    bi.biClrUsed = buffHeader[32] + ((buffHeader[33]&0xFF)<<8) + ((buffHeader[34]&0xFF)<<16) + ((buffHeader[35]&0xFF)<<24); 
    bi.biClrImportant = buffHeader[36] + ((buffHeader[37]&0xFF)<<8) + ((buffHeader[38]&0xFF)<<16) + ((buffHeader[39]&0xFF)<<24);

    printf("bi.biSize:%d\r\n",bi.biSize);
    printf("bi.biWidth:%d\r\n",bi.biWidth);
    printf("bi.biHeight:%d\r\n",bi.biHeight);
    printf("bi.biPlanes:%d\r\n",bi.biPlanes);
    printf("bi.biBitCount:%d\r\n",bi.biBitCount);
    printf("bi.biCompression:%d\r\n",bi.biCompression);
    printf("bi.biSizeImage:%d\r\n",bi.biSizeImage);
    printf("bi.biXPelsPerMeter:%d\r\n",bi.biXPelsPerMeter);
    printf("bi.biYPelsPerMeter:%d\r\n",bi.biYPelsPerMeter);
    printf("bi.biClrUsed:%d\r\n",bi.biClrUsed);
    printf("bi.biClrImportant:%d\r\n",bi.biClrImportant);

   if(bi.biBitCount >= 8)
        perW = bi.biBitCount/8;
    else
        perW = 1;
    
    /*to bmp data position*/
    if(fseek(fp, bf.bfOffbits, 0) < 0)
    {
        printf("bmp_get : lseek failed\r\n");
        fclose(fp);
        return NULL;
    }
   
    if(bi.biHeight < 0) { 
        totalSize = bi.biWidth*(-bi.biHeight)*(bi.biBitCount/8); 
    }else { 
        totalSize = bi.biWidth*bi.biHeight*(bi.biBitCount/8); 
    }

    printf("total size:%d\r\n",totalSize);
    
    data = (unsigned char *)malloc(totalSize);
    if((ret = fread(data, 1, totalSize, fp)) != (totalSize)){
        printf("bmp_get : read data failed\r\n");
        free(data);
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    
    pic = (unsigned char *)malloc(totalSize);
    memset(pic, 0, totalSize);
    
    if(bi.biHeight > 0){
        for(i = 0, picCount = totalSize; i < totalSize && picCount >= 0; ){
            picCount -= bi.biWidth*perW;
            for(j = 0; j < bi.biWidth*perW; j++){
                pic[picCount + j ] = data[i++];
            }
        }
    }
    free(data);

    *w = bi.biWidth;
    *h = bi.biHeight;
    return pic;
}






