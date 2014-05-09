/*
Project: Real-time video coding and transimission with energy analysis
File: capture.cpp
Author: Fei Cheng
Date: Oct. 2013
Platform: x86 PC and ARM Cortex A8 (pcDuino)
Function: Video capturing
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include "capture.h"
#include "global.h"

#define BUFFER_FRAME_NUM 5

static int stream_flag = V4L2_BUF_TYPE_VIDEO_CAPTURE;

extern struct Global_Parameter G_para;

struct Video video;

static void Video_Open();
static void Video_Close();
static void Video_Set_Para();
static void Video_Streamon();
static void Video_Streamoff();
static void Buffer_Init();
static void Buffer_Free();
static void Buffer_Request();
static void Buffer_Map(int index);

static void Video_Open()
{
	int i, fd;
	char device[13];

	for (i = 0; i < 99; i++) 
	{
		sprintf(device, "%s%d", "/dev/video", i);
		fd = open(device, O_RDWR);
		if (fd != -1) 
		{
			break;
		}
	}
	if (i == 99) 
	{
		perror("No WebCam Device");
		exit(EXIT_FAILURE);
	}
	video.fd = fd;
}

static void Video_Close()
{
	close(video.fd);
}

static void Video_Set_Para()
{
	memset(&video.stmpara, 0, sizeof(struct v4l2_streamparm));
	memset(&video.format, 0, sizeof(video.format));

	video.format.type = (v4l2_buf_type)stream_flag;
	video.format.fmt.pix.width = G_para.video_width;
	video.format.fmt.pix.height = G_para.video_height;
	video.format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

	video.stmpara.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	video.stmpara.parm.capture.capturemode = V4L2_MODE_HIGHQUALITY;     
	video.stmpara.parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
	video.stmpara.parm.output.outputmode = V4L2_MODE_HIGHQUALITY;     
	video.stmpara.parm.output.capability = V4L2_CAP_TIMEPERFRAME;
	video.stmpara.parm.capture.timeperframe.denominator = G_para.video_fps_num;
	video.stmpara.parm.capture.timeperframe.numerator = G_para.video_fps_den;

	if (-1 == ioctl(video.fd,VIDIOC_S_PARM, &video.stmpara))
	{
		perror("Video capture parameter error");
		exit(EXIT_FAILURE);
	}
	if (-1 == ioctl(video.fd, VIDIOC_S_FMT, &video.format)) 
	{
		perror("Video capture format error");
		exit(EXIT_FAILURE);
	}

	ioctl(video.fd,VIDIOC_S_PARM, &video.stmpara);
	
	G_para.video_fps_num = video.stmpara.parm.capture.timeperframe.denominator;
	G_para.video_fps_den = video.stmpara.parm.capture.timeperframe.numerator;

}

static void Video_Streamon()
{
	if (ioctl(video.fd, VIDIOC_STREAMON, &stream_flag) == -1) {
		if (errno == EINVAL) 
		{
			perror("Straming i/o is not support");
		}
		else 
		{
			perror("VIDIOC_STREAMON");
		}
		exit(EXIT_FAILURE);
	}
}

static void Video_Streamoff()
{
	if (ioctl(video.fd, VIDIOC_STREAMOFF, &stream_flag) == -1) 
	{
		if (errno == EINVAL) 
		{
			perror("streaming i/o is not support");
		}
		else 
		{
			perror("VIDIOC_STREAMOFF");
		}
		exit(EXIT_FAILURE);
	}
}

static void Buffer_Init()
{
	int i;

	Buffer_Request();
	for (i = 0; i < video.buffer.req.count; i++) 
	{
		Buffer_Map(i);
		Buffer_Enqueue(i);
	}
}

static void Buffer_Free()
{
	int i;

	for (i = 0; i < video.buffer.req.count; i++) 
	{
		munmap(video.buffer.buf[i].start, video.buffer.buf[i].length);
	}
	free(video.buffer.buf);
}

static void Buffer_Request()
{
	int sizenum = 0;

	memset(&video.buffer.req, 0, sizeof(video.buffer.req));
	video.buffer.req.type = (v4l2_buf_type)stream_flag;
	video.buffer.req.memory = V4L2_MEMORY_MMAP;
	video.buffer.req.count = BUFFER_FRAME_NUM;

	if (ioctl(video.fd, VIDIOC_REQBUFS, &video.buffer.req) == -1) 
	{
		if (errno == EINVAL) {
		perror("video capturing or mmap-streaming is not support");
	}
	else 
	{
		perror("VIDIOC_REQBUFS");
	}
		exit(EXIT_FAILURE);
	}
	if (video.buffer.req.count < BUFFER_FRAME_NUM) 
	{
		perror("No enough buffer");
		exit(EXIT_FAILURE);
	}
	sizenum = sizeof(*video.buffer.buf);
	video.buffer.buf = (Buffer::buftype*)calloc(video.buffer.req.count, sizenum);
	assert(video.buffer.buf != NULL);
}

static void Buffer_Map(int index)
{
	memset(&video.buffer.query, 0, sizeof(video.buffer.query));
	video.buffer.query.type = video.buffer.req.type;
	video.buffer.query.memory = V4L2_MEMORY_MMAP;
	video.buffer.query.index = index;

	if (ioctl(video.fd, VIDIOC_QUERYBUF, &video.buffer.query) == -1) 
	{
		perror("Video querybuf error");
		exit(EXIT_FAILURE);
	}
	video.buffer.buf[index].length = video.buffer.query.length;
	video.buffer.buf[index].start = mmap(NULL,
	       video.buffer.query.length, PROT_READ | PROT_WRITE, MAP_SHARED,
	       video.fd,video.buffer.query.m.offset);
	if (video.buffer.buf[index].start == MAP_FAILED) 
	{
		perror("Video mmap error");
		exit(EXIT_FAILURE);
	}
}

void Video_Init()
{
	Video_Open();
	Video_Set_Para();
	Buffer_Init();
	Video_Streamon();
}

void Video_Exit()
{
	//Video_Streamon();
	Video_Streamoff();
	Video_Close();
	Buffer_Free();
}

void Buffer_Enqueue(int index)
{
	memset(&video.buffer.query, 0, sizeof(video.buffer.query));
	video.buffer.query.type = video.buffer.req.type;
	video.buffer.query.memory = V4L2_MEMORY_MMAP;
	video.buffer.query.index = index;

	if (ioctl(video.fd, VIDIOC_QBUF, &video.buffer.query) == -1) 
	{
		perror("VIDIOC_QBUF");
		exit(EXIT_FAILURE);
	}
}

void Buffer_Dequeue(int index)
{
	memset(&video.buffer.query, 0, sizeof(video.buffer.query));
	video.buffer.query.type = video.buffer.req.type;
	video.buffer.query.memory = V4L2_MEMORY_MMAP;
	video.buffer.query.index = index;
	
	if (ioctl(video.fd, VIDIOC_DQBUF, &video.buffer.query) == -1) 
	{
		perror("VIDIOC_DQBUF");
		exit(EXIT_FAILURE);
	}
}

int Video_Capture(uint8_t* YUV2)
{
	static int i = 0;
	i %= video.buffer.req.count;
	Buffer_Dequeue(i);
	memcpy(YUV2, (uint8_t*)video.buffer.buf[i].start, 
				G_para.video_width * G_para.video_height * 2);
	Buffer_Enqueue(i);
	i++;
	return i;
}