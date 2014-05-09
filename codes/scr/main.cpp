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
#include <math.h>

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
int Get_Energy();
float angle_x, angle_y, angle_z, g_x, g_y, g_z;
float *energy;

int NAL_num;

extern struct Global_Parameter G_para;
extern struct Global_Video G_video;
extern struct Global_File G_file;
unsigned long interval = 0, time_begin = 0, interval1 = 0; //Time control
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
	unsigned char sensor_state = 0; //0 Not stable  1 OK
	float th = 100;
	int h264size = 0;
	float tangle_x, tangle_y, tangle_z, tg_x, tg_y, tg_z;
	int stable_num = 0;
	//Initialization
	if (Global_Para_Init(argc, argv) == 0)
		return 0;
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

	Get_Energy();

	time_begin = (unsigned long)getCurrentTime();
/*
	if (G_para.idle_energy)
	{
		int test_num = 10;
		energy = Get_Energy();
		while(test_num--)
		{
			usleep(40000);
			energy = Get_Energy();
			printf("%4.2f\t%4.2f\t%4.2f\n", *energy, *(energy + 1), *(energy + 2));
		}
		return 0;
	}*/

	while(capture_state != -1)
	{
		//Get a frame
		
		if (G_para.run_mode == 0)
		{	
			if (G_para.is_YUV_out)
			{
				if (Get_Energy() == 1)
				{
					if (sensor_state == 0)
					{
						printf("%4.2f\t%4.2f\t%4.2f\t \n", angle_x, angle_y, angle_z);
						if (fabs(tangle_x - angle_x) < 0.02 && fabs(tangle_y - angle_y) < 0.02 && fabs(tangle_z - angle_z) < 0.02)
						{
							stable_num ++;
						}
						else
						{
							stable_num = 0;
						}
						tangle_x = angle_x;
						tangle_y = angle_y;
						tangle_z = angle_z;
						usleep(500000);
						if (stable_num >= 10)
						{
							sensor_state = 1;

							printf("\nSensor OK. Press ENTER to sample.\n");
							getchar();
							
						}
					}
				}
			}
			else
			{
				sensor_state = 1;
			}

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

		//energy = Get_Energy();
		//energy_stage_1[0] = *energy;energy_stage_1[1] = *(energy + 1);energy_stage_1[2] = *(energy + 2);

		if (G_para.is_YUV_out)
		{
			if (sensor_state)
			{
				fwrite(G_video.Raw_YUV420, 1, G_para.video_width * G_para.video_height * 1.5,G_file.output_YUV_file);
			}
			
		}
		else 
		{
			h264size = encode_one_frame(frame_type, G_video.Raw_YUV420, G_video.Stream_H264);

		}
		
		//energy = Get_Energy();
		//energy_stage_3[0] = *energy;energy_stage_3[1] = *(energy + 1);energy_stage_3[2] = *(energy + 2);

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
	      //usleep(40000 - interval * 1000);
	    }
	    if (G_para.is_YUV_out == 0 || (G_para.is_YUV_out == 1 && sensor_state == 1))
	    {

	    	frame_number ++;
			printf("%lu %lu %d %d ",frame_number, interval1, h264size, NAL_num);
			printf("%4.2f %4.2f %4.2f %4.2f %4.2f %4.2f\n", 
				angle_x,angle_y,angle_z,
				g_x, g_y, g_z);	

			if (G_para.is_output_info == 1)
			{
				fprintf(G_file.output_info_file, "%lu %lu %d %d ",frame_number, interval1, h264size, NAL_num);
				fprintf(G_file.output_info_file, "%4.2f %4.2f %4.2f %4.2f %4.2f %4.2f\n", 
					angle_x,angle_y,angle_z,
					g_x, g_y, g_z);	
			}
	
	    }

	    time_begin = (unsigned long)getCurrentTime();
	    if (frame_number > 300)
	    {
	    	capture_state = -1;
	    	break;
	    }
	    
	}
	//Exit
	Globlal_Exit();
	RTP_end();
	X264_end();
	Video_Exit();
	
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

int Get_Energy()
{

#ifdef PLATFORM_ARM
	int thisByte = 33; 
    char dat[200];
    int dat_i = 0;
    if (G_para.is_energy_get == 0)
    {
		return 0;
    } 
READ_SENSOR:
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

    while(Serial.available() > 0)
    {
    	thisByte = Serial.read();
    }
    if (6 != sscanf(dat, " %f\t%f\t%f\t%f\t%f\t%f\0", &angle_x, &angle_y, &angle_z, &g_x, &g_y, &g_z))
    {
    	goto READ_SENSOR;
    }
    else
    {
    	
    }
	return 1;
#else

	return 0;
#endif
}