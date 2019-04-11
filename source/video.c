#include "video.h"
#include <linux/videodev2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>           
#include <unistd.h>
#include <sys/mman.h>

typedef struct buffer{
    void *start; 
    unsigned int length;
}BUFFER;

BUFFER *buffers;

static int fd = -1;
FILE *file_fd;


struct v4l2_capability cap;
struct v4l2_format fmt;
unsigned int i;
enum v4l2_buf_type type;
struct v4l2_queryctrl qctrl;
struct v4l2_fmtdesc fmt1;
struct v4l2_requestbuffers req;

int init_video_device(int *w,int *h)
{
    int ret;
    int file_length;
    int n_buffers;

    fd = open ("/dev/video0", O_RDWR /* required */ |O_NONBLOCK);//打开设备
    if(fd<0)
    { 
        perror("open faiure!");
        exit(1);
    }
    int ff=ioctl (fd, VIDIOC_QUERYCAP, &cap);              //获取摄像头参数
    if(ff<0)
        printf("failture VIDIOC_QUERYCAP\n");

    if ((cap.capabilities &  V4L2_CAP_VIDEO_CAPTURE) == 0) 
    {
        printf("video device donot support capture\n");
        return -1;
    }
    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        printf("%s does not support streaming i/o\n", "/dev/vide0");
    }
    if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
        printf("%s does not support read i/o\n", "/dev/vide0");
    }
    printf("driver name	:%s\n",cap.driver);
		printf("card name	:%s\n",cap.card);
		printf("bus info	:%s\n",cap.bus_info);
		printf("driver version	:%u.%u.%u\n",(cap.version>>16)&0xff,(cap.version>>8)&0xff,(cap.version)&0xff);
		printf("video capability:%u.%u.%u\n",(cap.capabilities>>16)&0xff,(cap.capabilities>>8)&0xff,(cap.capabilities)&0xff);

  	memset(&fmt1, 0, sizeof(fmt1));  
	fmt1.index = 0;
	fmt1.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while ((ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmt1)) == 0) //查看摄像头所支持的格式
    {
        fmt1.index++;
        printf("{ pixelformat = '%c%c%c%c', description = '%s' }\n",
                fmt1.pixelformat&0xFF,
                (fmt1.pixelformat>>8)&0xFF,
                (fmt1.pixelformat >>16)&0xFF,
                (fmt1.pixelformat>>24)&0xFF,
                fmt1.description);
    }

    /*clear fmt*/
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = 320;
	fmt.fmt.pix.height = 240;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24; 
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    //设置图像格式
	if(ioctl (fd,VIDIOC_S_FMT,&fmt)<0)
	{
		fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24; 
        printf("not support rgb24bit...\r\n");
		if(ioctl (fd,VIDIOC_S_FMT,&fmt)<0)
			printf("failture VIDIOC_S_FMT to YUYV\n");
 	}

    while ((ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmt1)) == 0) //查看摄像头所支持的格式
    {
        fmt1.index++;
        printf("{ pixelformat = '%c%c%c%c', description = '%s' }\n",
                fmt1.pixelformat&0xFF,
                (fmt1.pixelformat>>8)&0xFF,
                (fmt1.pixelformat >>16)&0xFF,
                (fmt1.pixelformat>>24)&0xFF,
                fmt1.description);
    }

    file_length = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height; //计算图片大小
    printf("width:%d\t\theight:%d\n",fmt.fmt.pix.width,fmt.fmt.pix.height);
 
	req.count = 1;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

    //申请缓冲，count是申请的数量
	if(ioctl(fd, VIDIOC_REQBUFS, &req)<0)
		printf("failture VIDIOC_REQBUFS\n");
	if (req.count < 1)
	    printf("Insufficient buffer memory\n");
    buffers = calloc (req.count, sizeof (*buffers));//内存中建立对应空间
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
	{
		struct v4l2_buffer buf; 
		//CLEAR (buf);
		buf.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory	= V4L2_MEMORY_MMAP;
		buf.index	= n_buffers;
		if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf)) //映射用户空间
			printf ("VIDIOC_QUERYBUF error\n");
		buffers[n_buffers].length = buf.length;
        printf("buf.length:%d\r\n",buf.length);
		buffers[n_buffers].start=mmap(NULL,buf.length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,buf.m.offset); //通过mmap建立映射关系
		if (MAP_FAILED == buffers[n_buffers].start){
            printf ("mmap failed\n");
            exit(-1);
        }	
	}
    //get camera data
    unsigned int i;
    enum v4l2_buf_type type;
    for(i=0;i<req.count;i++)
    {
        struct v4l2_buffer buf;
        memset(&buf,0,sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        ioctl(fd,VIDIOC_QBUF,&buf);
    }
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fd,VIDIOC_STREAMON,&type);

    printf("req.count:%d\r\n",req.count);

    *w = fmt.fmt.pix.width;
    *h = fmt.fmt.pix.height;

    return 0;
}


void video_get_data(unsigned char *p)
{
    int i = 0;

     
    for(i =0;i<req.count;i++)
    {
        struct v4l2_buffer buf;
        memset(&buf,0,sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        ioctl(fd,VIDIOC_DQBUF,&buf);

        //deal with data
        process_image(&buffers[i],fmt.fmt.pix.width,fmt.fmt.pix.height,p);

        ioctl(fd,VIDIOC_QBUF,&buf);
    }
}


int process_image(BUFFER *buf,int width,int height,unsigned char *p)
{
    memcpy(p, buf->start, width * height * 2/*buf->length*/);
#if 0
    if((p[0]==0xff) && (p[1] == 0xd8))
    {
        int fd1= open ("ttt.jpg", O_RDWR | O_CREAT);// 
        if(fd1<0)
        { 
            perror("open faiure!");
            exit(1);
        }

        write(fd1,p,320*240*12);
        while(1){
            sleep(1);
        }


    }
#endif
    return 0;
}
