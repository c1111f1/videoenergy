/*
Project: Real-time video coding and transimission with energy analysis
File: trans.cpp
Author: Fei Cheng
Date: Oct. 2013
Platform: x86 PC and ARM Cortex A8 (pcDuino)
Function: Video format transformation
*/

#include <stdint.h>
#include <stdio.h>
#include "global.h"
#include "trans.h"

extern struct Global_Parameter G_para;

void YUV2_to_YUV420(uint8_t *yuv2, uint8_t *yuv420)
{
	int yuv2_length = G_para.video_width * G_para.video_height * 2;
	int Y_index = 0, U_index = 0, V_index = 0, H_index = 0;
	int i;

	uint8_t *Y = yuv420;
	uint8_t *U = Y + G_para.video_width * G_para.video_height;
	uint8_t *V = U + G_para.video_width * G_para.video_height / 4;

	
	for (i =0; i < yuv2_length; i = i + 4)
	{
		H_index = i / (G_para.video_width * 2);

		*(Y + Y_index++) = *(yuv2 + i + 0);
		*(Y + Y_index++) = *(yuv2 + i + 2);
		if(H_index % 2)
		{
			*(U + U_index++) = *(yuv2 + i +1);
		}
		else
		{
			*(V + V_index++) = *(yuv2 + i +3);
		}
	}

}
