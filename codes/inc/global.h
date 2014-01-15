/*
Project: Real-time video coding and transimission with energy analysis
File: global.h
Author: Fei Cheng
Date: Oct. 2013
Platform: x86 PC and ARM Cortex A8 (pcDuino)
Function: Global parameters, such as platform and others
*/


/*To select the platform, if the platform is arm, energy information will
be obainted by UART port on pcDuino. And the libraries will be selected 
with platform*/
#ifndef _GLOBAL_H
#define _GLOBAL_H

#define PLATFORM_PC		//ARM or PC

#ifdef PLATFORM_PC
#define DISPLAY_YES		//Compile the SDL lib or not(DISPLAY_NO)
#define DISPLAYFORM "PC"
#endif

#ifdef PLATFORM_ARM
#define ENERGY_YES		//Compile the Andurino lib or not(ENERGY_NO)
#define DISPLAYFORM "ARM"
#endif

#include <string>

#define IFYESNO(CONDITION) CONDITION == 1?"YES":"NO"
#define IFINFO(CONDITION) CONDITION == 0?"NO":CONDITION == 1?"Simple":"Detail"
#define IFMODE(CONDITION) CONDITION == 0?"Real Time":"Test Mode"
#define IFENCMODE(CONDITION) CONDITION == 0?"Only I frames":CONDITION == 1?"IPPP mode":"Optimization Mode"

struct Global_Parameter
{
	/*Image and video parameters*/
	int video_width;
	int video_height;
	int video_fps_num;
	int video_fps_den;

	/*Encoder parameters*/
	char encoder_qp;
	int  encoder_slice_max_size;

	/*Input and output file for H264 stream, 
		YUV stream and information*/
	char file_YUV_output[50];
	char file_h264_output[50];
	char file_info_output[50];
	char file_YUV_input[50];

	/*States and modes*/
	char is_h264_out;		//0: do not output h264 file
							//1: output h264 file

	char is_YUV_out;		//0: do not output YUV file
							//1: output YUV file

	char is_display;		//0: do not display video locally
							//1: display   
							//   NOTE: For ARM, do not display

	char is_output_info;	//0: do not output information
							//1: simple output
							//2: detail output

	char is_energy_get;		//0: do not get energy information
							//1: get energy information

	char run_mode;			//0: Real time mode, by using camera
							//1: Test mode, by using input YUV

	char encode_mode;		//0: only I frame mode
							//1: IPPP mode
							//2: Optimization mode

	/*Communication parameters*/
	std::string ip_address_remote;
	unsigned int ip_port_remote;
	unsigned int ip_port_local;
};

struct Global_File
{
	FILE * output_YUV_file;
	FILE * output_264_file;
	FILE * output_info_file;
	FILE * input_YUV_file;
};

struct Global_Video
{
	uint8_t * Raw_YUV2;
	uint8_t * Raw_YUV420;
	uint8_t * Stream_H264;
};

void Global_Para_Init(int argc, char *argv[]);	//initialize parameters

void Global_Para_Default(); //Set all parameters to defaults

void Global_Para_Deal(int argc, char *argv[]);
							//Set parameters from command line
void Global_Para_Check();	//Check the parameters

void Global_Para_Print();	//Display all the parameters on terminal

void Globlal_Exit();

#endif