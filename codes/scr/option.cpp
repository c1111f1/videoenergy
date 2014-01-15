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
void Global_Para_Init(int argc, char *argv[])
{
	Global_Para_Default();
	Global_Para_Deal(argc, argv);
	Global_Para_Check();

	//Creat Data space for video
	G_video.Raw_YUV2   = (uint8_t *) malloc(sizeof(uint8_t) * G_para.video_width * G_para.video_height * 2);
	G_video.Raw_YUV420 = (uint8_t *) malloc(sizeof(uint8_t) * G_para.video_width * G_para.video_height * 1.5);
	G_video.Stream_H264= (uint8_t *) malloc(sizeof(uint8_t) * G_para.video_width * G_para.video_height * 1.5);
	
}

//Set all parameters to defaults
void Global_Para_Default()
{
	/*Image and video parameters*/
	G_para.video_width = 640;
	G_para.video_height = 480;
	G_para.video_fps_num = 25;
	G_para.video_fps_den = 1;

	/*Encoder parameters*/
	G_para.encoder_qp = 20;
	G_para.encoder_slice_max_size = 1000;

	/*Input and output file for H264 stream, 
		YUV stream and information*/
	strcpy(G_para.file_YUV_output, "data/yuv_output.yuv");
	strcpy(G_para.file_h264_output, "data/264_output.264");
	strcpy(G_para.file_info_output, "data/info.txt");
	strcpy(G_para.file_YUV_input, "data/input.yuv");

	/*States and modes*/
	G_para.is_h264_out = 1;	//0: do not output h264 file
							//1: output h264 file

	G_para.is_YUV_out = 0;	//0: do not output YUV file
							//1: output YUV file

	G_para.is_display = 1;	//0: do not display video locally
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

	/*Communication parameters*/
	G_para.ip_address_remote = "192.168.1.100";
	G_para.ip_port_remote = 4444;
	G_para.ip_port_local = 4004;
}

//Set parameters from command line
void Global_Para_Deal(int argc, char *argv[])
{

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
	int feedback = system("clear");
	printf("Real Time Video Coding and transimission Test Software with Energy\n");
	printf("MSc Project of EEE Department in XJTLU - Candidate: Fei Cheng\n");
	printf("------------------------------------------------------------------\n");
	printf("Platform          :  %s\n", DISPLAYFORM);
	printf("Video Size        :  %d x %d\n", G_para.video_width, G_para.video_height);
	printf("Frames per Second :  %d / %d\n", G_para.video_fps_num, G_para.video_fps_den);
	printf("Output H.264 file :  %s\n", IFYESNO(G_para.is_h264_out));
	printf("Output H.264 path :  %s\n", G_para.file_h264_output);
	printf("Output YUV   file :  %s\n", IFYESNO(G_para.is_YUV_out));
	printf("Output YUV   path :  %s\n", G_para.file_YUV_input);
	printf("Output info file  :  %s\n", IFINFO(G_para.is_output_info));
	printf("Info file   path  :  %s\n", G_para.file_info_output);
	printf("Display   video   :  %s\n", IFYESNO(G_para.is_display));
	printf("Energy  info  get :  %s\n", IFYESNO(G_para.is_energy_get));
	printf("Run    mode       :  %s\n", IFMODE(G_para.run_mode));
	printf("Input  YUV  file  :  %s\n", G_para.file_YUV_input);
	printf("Encode    mode    :  %s\n", IFENCMODE(G_para.encode_mode));
	printf("Remote   IP       :  %s\n", G_para.ip_address_remote.c_str()); //Print string in C style
	printf("Remote   port     :  %d\n", G_para.ip_port_remote);
	printf("Local    port     :  %d\n", G_para.ip_port_local);
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