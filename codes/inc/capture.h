#ifndef _CAPTURE_H
#define _CAPTURE_H

#include <linux/videodev2.h>
 
struct Buffer
{
	struct v4l2_requestbuffers req;
	struct v4l2_buffer query;
	struct buftype
	{
		void   *start;
		size_t length;
  	}*buf;
};

struct Video 
{
	int fd;
	struct v4l2_format format;
	struct v4l2_streamparm stmpara;
	struct Buffer buffer;
};

void Video_Init();
void Video_Exit();
void Buffer_Enqueue(int index);
void Buffer_Dequeue(int index);
int Video_Capture(uint8_t* YUV2);

#endif
