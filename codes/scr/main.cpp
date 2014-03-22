/*
Project: Real-time video coding and transimission with energy analysis
File: main.cpp
Author: Fei Cheng
Date: Oct. 2013
Platform: x86 PC and ARM Cortex A8 (pcDuino)
Function: The whole processing of this project
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "global.h"
#include "capture.h"
#include "trans.h"
#include "rtph.h"

#include <sys/time.h>    
#include <unistd.h>

#ifdef PLATFORM_ARM
	#include "core.h"
	#include "Serial.h"
#endif

extern "C"
{
#include "x264.h"
}

#include "encoder.h"	

void uart_init();
float * Get_Energy();
float energy_stage_1[3],energy_stage_2[3],energy_stage_3[3],energy_stage_4[3];
float *energy;
int NAL_num;
extern struct Global_Parameter G_para;
extern struct Global_Video G_video;
extern struct Global_File G_file;

long getCurrentTime();

long getCurrentTime()  
{  
   struct timeval tv;  
   gettimeofday(&tv,NULL);  
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;  
}  

int main(int argc, char *argv[])
{
	char capture_state = 0;
	int frame_type = -1;
	unsigned long frame_number = 0;
	float total_energy;
	
	float th = 100;
	int h264size = 0;
	unsigned long interval = 0, time_begin = 0; //Time control
	//Initialization
	Global_Para_Init(argc, argv);
	if (G_para.run_mode == 0)
	{
		//Real time mode
		Video_Init();
	}

	X264_init();
	RTP_init();
	uart_init();
	
	//After all the initialization, print all the parameters
	Global_Para_Print();

	energy = Get_Energy();

	time_begin = (unsigned long)getCurrentTime();

	while(capture_state != -1)
	{
		//Get a frame
		frame_number ++;
		if (G_para.run_mode == 0)
		{
			capture_state = Video_Capture(G_video.Raw_YUV2);
			YUV2_to_YUV420(G_video.Raw_YUV2, G_video.Raw_YUV420);
		}
		else if (G_para.run_mode == 1)
		{
			if (fread(G_video.Raw_YUV420, 1, G_para.video_width * G_para.video_height * 1.5, G_file.input_YUV_file) 
				!= G_para.video_width * G_para.video_height* 1.5)
			{
				capture_state = -1;
				break;
			}
		}

		energy = Get_Energy();
		energy_stage_1[0] = *energy;energy_stage_1[1] = *(energy + 1);energy_stage_1[2] = *(energy + 2);

		if (G_para.is_YUV_out)
		{
			fwrite(G_video.Raw_YUV420, 1, G_para.video_width * G_para.video_height * 1.5,G_file.output_YUV_file);
		}

		h264size = encode_one_frame(frame_type, G_video.Raw_YUV420, G_video.Stream_H264);


		energy = Get_Energy();
		energy_stage_3[0] = *energy;energy_stage_3[1] = *(energy + 1);energy_stage_3[2] = *(energy + 2);



		//energy = Get_Energy();

		if (G_para.encode_mode == 2)
		{
			if ( *energy - total_energy > th)
				frame_type = 2;
			else
				frame_type = -1;
		}

		if (G_para.encode_mode == 1)
		{
			frame_type = -1;
		}

		if (G_para.encode_mode == 0)
		{
			frame_type = 2;
		}

		if (G_para.is_h264_out)
		{
			fwrite(G_video.Stream_H264, 1, h264size ,G_file.output_264_file);
		}
		
		interval =  (unsigned long)getCurrentTime() - time_begin;
		
   		//printf("%lu\n",interval);
	    if (interval <= 40)
	    {
	      usleep(40000 - interval * 1000);
	    }

	    energy = Get_Energy();
		energy_stage_4[0] = *energy;energy_stage_4[1] = *(energy + 1);energy_stage_4[2] = *(energy + 2);

		printf("%lu %lu %d %d ",frame_number, interval, h264size, NAL_num);
		printf("%4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f\n", 
			energy_stage_1[0],energy_stage_1[1],energy_stage_1[2],
			energy_stage_2[0],energy_stage_2[1],energy_stage_2[2],
			energy_stage_3[0],energy_stage_3[1],energy_stage_3[2],
			energy_stage_4[0],energy_stage_4[1],energy_stage_4[2]);
		fprintf(G_file.output_info_file, "%lu %lu %d %d ",frame_number, interval, h264size, NAL_num);
		fprintf(G_file.output_info_file, "%4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f\n", 
			energy_stage_1[0],energy_stage_1[1],energy_stage_1[2],
			energy_stage_2[0],energy_stage_2[1],energy_stage_2[2],
			energy_stage_3[0],energy_stage_3[1],energy_stage_3[2],
			energy_stage_4[0],energy_stage_4[1],energy_stage_4[2]);		
	    time_begin = (unsigned long)getCurrentTime();

	}

	//Exit
	RTP_end();
	X264_end();
	Video_Exit();
	Globlal_Exit();
}

void uart_init() 
{ 
#ifdef PLATFORM_ARM
	init(); 
	int rate = 115200;
	Serial.begin(rate); 
	while (!Serial);
#endif
} 

float x[3];

float * Get_Energy()
{
float *p = x;
#ifdef PLATFORM_ARM
	int thisByte = 33; 
    char dat[100];
    int dat_i = 0;
    Serial.print('1');
    while (Serial.available() == 0)
    {
      if (serialEventRun) serialEventRun(); //Important
    };
    do
    {
      if (Serial.available() > 0)
      {
        thisByte = Serial.read();
        dat[dat_i++] = thisByte;
        //printf("%c", thisByte);
      }
      if (serialEventRun) serialEventRun();
    } while (thisByte != 10);
    sscanf(dat, "%f\t%f\t%f\0", p, p + 1, p + 2);
	return p;
#else
	*p = -1;
	*(p + 1) = -1;
	*(p + 2) = -1;
	return p;
#endif
}