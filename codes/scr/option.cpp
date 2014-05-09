/*
Project: Real-time video coding and transimission with energy analysis
File: option.cpp
Author: Fei Cheng
Date: Oct. 2013
Platform: x86 PC and ARM Cortex A8 (pcDuino)
Function: The whole processing of this project
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "global.h"

struct Global_Parameter G_para; //Define Global parameter
struct Global_File G_file; //Define Global parameter
struct Global_Video G_video;

//initialize parameters
int Global_Para_Init(int argc, char *argv[])
{
	Global_Para_Default();
	if (Global_Para_Deal(argc, argv) == 0)
		return 0;
	Global_Para_Check();

	//Creat Data space for video
	G_video.Raw_YUV2   = (uint8_t *) malloc(sizeof(uint8_t) * G_para.video_width * G_para.video_height * 2);
	G_video.Raw_YUV420 = (uint8_t *) malloc(sizeof(uint8_t) * G_para.video_width * G_para.video_height * 1.5);
	G_video.Stream_H264= (uint8_t *) malloc(sizeof(uint8_t) * G_para.video_width * G_para.video_height * 1.5);
	return 1;
}

//Set all parameters to defaults
void Global_Para_Default()
{
	/*Image and video parameters*/
#ifdef PLATFORM_ARM
	G_para.video_width = 176;
	G_para.video_height = 144;
#endif

#ifdef PLATFORM_PC
	G_para.video_width = 640;
	G_para.video_height = 480;
#endif

	G_para.video_fps_num = 25;
	G_para.video_fps_den = 1;

	/*Encoder parameters*/
	G_para.encoder_qp = 30;
	G_para.encoder_slice_max_size = 1000;

	/*Input and output file for H264 stream, 
		YUV stream and information*/
	strcpy(G_para.file_YUV_output, "data/yuv_output.yuv");
	strcpy(G_para.file_h264_output, "data/264_output.264");
	strcpy(G_para.file_info_output, "data/info.txt");
	strcpy(G_para.file_YUV_input, "/");

	/*States and modes*/
	G_para.is_h264_out = 0;	//0: do not output h264 file
							//1: output h264 file

	G_para.is_YUV_out = 0;	//0: do not output YUV file
							//1: output YUV file

	G_para.is_display = 0;	//0: do not display video locally
							//1: display   
							//   NOTE: For ARM, do not display

	G_para.is_output_info = 0;	//0: do not output information
								//1: simple output
								//2: detail output

	G_para.is_energy_get = 0;	//0: do not get energy information
								//1: get energy information

	G_para.run_mode = 0;		//0: Real time mode, by using camera
								//1: Test mode, by using input YUV

	G_para.encode_mode = 1;	//0: only I frame mode
							//1: IPPP mode
							//2: Optimization mode

	G_para.encode_scenecout = 40;

	G_para.encode_mv_range = -1;

	G_para.encode_me_range = 16;

	G_para.idle_energy = 0;

	/*Communication parameters*/
	G_para.ip_address_remote = "192.168.1.100";
	G_para.ip_port_remote = 4444;
	G_para.ip_port_local = 4004;
}

//Set parameters from command line
int Global_Para_Deal(int argc, char *argv[])
{
	//-s Size heightxwidth, eg 352x288
	//-q QP value
	//-p Package size
	//-i Input YUV
	//-m Run mode 0 real time 1 input file
	//-c Encode mode 0 I only 1 IPPP, 2 Optimization
	//-e Energy obtain 
	//-a IP address ,default:192.168.1.100
	//-r Remote port ,default:4444
	//-l Local port ,default:4444
	//-g ME search range v mv range
	//-v MV range
	//-o Output data,
	int opt;
	while ((opt = getopt(argc, argv, "q:p:i:m:c:ea:r:l:s:g:o:dhy:")) != -1) 
	{
		switch(opt) 
		{
			case 's':
			{	
				sscanf(optarg, "%dx%d", &G_para.video_width, &G_para.video_height);
				break;
			}
			case 'q':
			{	
				sscanf(optarg, "%d", &G_para.encoder_qp);
				break;
			}
			case 'p':
			{	
				sscanf(optarg, "%d", &G_para.encoder_slice_max_size);
				break;
			}
			case 'i':
			{	
				memset(G_para.file_YUV_input,0,100);
				memcpy(G_para.file_YUV_input, optarg,strlen(optarg));
				G_para.run_mode = 1;
				break;
			}
			case 'm':
			{	
				sscanf(optarg, " %d", &G_para.run_mode);
				break;
			}
			case 'c':
			{	
				sscanf(optarg, "%d", &G_para.encode_mode);
				break;
			}
			case 'e':
			{
				G_para.is_energy_get = 1;
				break;
			}
			case 'a':
			{
				G_para.ip_address_remote = optarg;
				break;
			}
			case 'r':
			{
				sscanf(optarg, "%d", &G_para.ip_port_remote);
				break;
			}
			case 'l':
			{
				sscanf(optarg, "%d", &G_para.ip_port_local);
				break;
			}

			case 'g':
			{
				sscanf(optarg, "%d", &G_para.encode_me_range);
				break;
			}
			case 'v':
			{
				sscanf(optarg, "%d", &G_para.encode_mv_range);
				break;
			}
			case 'o':
			{
				memset(G_para.file_info_output,0,100);
				memcpy(G_para.file_info_output, optarg,strlen(optarg));
				G_para.is_output_info = 1;
				break;
			}
			case 'd':
			{
				G_para.idle_energy = 1;
				break;
			}
			case 'h':
			{
				printf("\t-s Size heightxwidth, eg. 352x288\n");
				printf("\t-q QP value\n");
				printf("\t-p Package size\n");
				printf("\t-i Input YUV\n");
				printf("\t-m Run mode 0 real time 1 input file\n");
				printf("\t-c Encode mode 0 I only 1 IPPP, 2 Optimization\n");
				printf("\t-e Energy obtain \n");
				printf("\t-a IP address ,default:192.168.1.100\n");
				printf("\t-r Remote port ,default:4444\n");
				printf("\t-l Local port ,default:4444\n");
				printf("\t-g ME search range v mv range\n");
				printf("\t-v MV range\n");
				printf("\t-o Output data\n");
				return 0;
				break;
			}
			case 'y':
			{
				G_para.is_YUV_out = 1;
				memset(G_para.file_YUV_output,0,100);
				memcpy(G_para.file_YUV_output, optarg,strlen(optarg));
				break;
			}
			
		}
	}
	return 1;
}
							
//Check the parameters							
void Global_Para_Check()
{
#ifdef PLATFORM_ARM
/*For ARM platform, display is disable*/
	G_para.is_display = 0;
#endif

#ifdef PLATFORM_PC
/*For PC platform, energy get is disable*/
	G_para.is_energy_get = 0;
#endif

	if (G_para.is_h264_out)
	{
		G_file.output_264_file = fopen(G_para.file_h264_output, "wb");
	}

	if (G_para.is_YUV_out)
	{
		G_file.output_YUV_file = fopen(G_para.file_YUV_output,"wb");
	}

	if (G_para.is_output_info)
	{
		G_file.output_info_file = fopen(G_para.file_info_output,"w");
	}

	if (G_para.run_mode == 1)
	{
		G_file.input_YUV_file = fopen(G_para.file_YUV_input,"rb");
	}

}

//Display all the parameters on terminal
void Global_Para_Print()
{
	//int feedback = system("clear");
	//printf("Real Time Video Coding and transimission Test Software with Energy\n");
	//printf("MSc Project of EEE Department in XJTLU - Candidate: Fei Cheng\n");
	printf("------------------------------------------------------------------\n");
	printf("Platform          :  %s\n", DISPLAYFORM);
	printf("Video Size        :  %d x %d\n", G_para.video_width, G_para.video_height);
	printf("Frames per Second :  %d / %d\n", G_para.video_fps_num, G_para.video_fps_den);
	printf("Output H.264 file :  %s\n", IFYESNO(G_para.is_h264_out));
	printf("Output H.264 path :  %s\n", G_para.file_h264_output);
	printf("Output YUV file   :  %s\n", IFYESNO(G_para.is_YUV_out));
	printf("Output YUV path   :  %s\n", G_para.file_YUV_output);
	printf("Output info file  :  %s\n", IFINFO(G_para.is_output_info));
	printf("Info file path    :  %s\n", G_para.file_info_output);
	printf("Display video     :  %s\n", IFYESNO(G_para.is_display));
	printf("Energy info get   :  %s\n", IFYESNO(G_para.is_energy_get));
	printf("Run mode          :  %s\n", IFMODE(G_para.run_mode));
	printf("Input YUV file    :  %s\n", G_para.file_YUV_input);
	printf("Encode mode       :  %s\n", IFENCMODE(G_para.encode_mode));
	printf("Encode QP         :  %d\n", G_para.encoder_qp);
	printf("Encode slice size :  %d\n", G_para.encoder_slice_max_size);
    printf("Encode scenecout  :  %d\n", G_para.encode_scenecout);
	printf("Remote IP         :  %s\n", G_para.ip_address_remote.c_str()); //Print string in C style
	printf("Remote port       :  %d\n", G_para.ip_port_remote);
	printf("Local  port       :  %d\n", G_para.ip_port_local);
	printf("------------------------------------------------------------------\n");
}

void Globlal_Exit()
{
	if (G_para.is_h264_out)
	{
		fclose(G_file.output_264_file);
	}

	if (G_para.is_YUV_out)
	{
		fclose(G_file.output_YUV_file);
	}

	if (G_para.is_output_info)
	{
		fclose(G_file.output_info_file);
	}

	if (G_para.run_mode == 1 && G_file.input_YUV_file)
	{
		fclose(G_file.input_YUV_file);
	}

}