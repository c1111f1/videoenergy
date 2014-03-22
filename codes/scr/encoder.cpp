/*
Project: Real-time video coding and transimission with energy analysis
File: encoder.cpp
Author: Fei Cheng
Date: Nov. 2013
Platform: x86 PC and ARM Cortex A8 (pcDuino)
Function: The encoder of x264
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern "C"
{
	#include "x264.h"
}
#include "global.h"
#include "encoder.h"
#include "rtph.h"

Encoder g_X264Encoder;

uint g_ImgWidth;
uint g_ImgHeight;

uint8_t *g_H264_Buf;

extern struct Global_Parameter G_para;
extern float energy_stage_2[3];
float * Get_Energy();
extern float *energy;
extern int NAL_num;

void encode_init(Encoder *encoder, int img_width, int img_height)
{
//Set default x264 parameters
	encoder->param = (x264_param_t *) malloc(sizeof(x264_param_t));
	encoder->picture = (x264_picture_t *) malloc(sizeof(x264_picture_t));
	x264_param_default(encoder->param);

	encoder->param->i_width = img_width; //set frame width
	encoder->param->i_height = img_height; //set frame height
	encoder->param->rc.i_lookahead = 0; //I frame look ahead
	encoder->param->i_fps_num = 25; 	//fps
	encoder->param->i_fps_den = 1; 		//fps
	encoder->param->rc.i_lookahead = 0;
	encoder->param->i_sync_lookahead = 0;
	encoder->param->i_bframe = 0;
	encoder->param->b_sliced_threads = 1;
	encoder->param->b_vfr_input = 0;
	encoder->param->rc.b_mb_tree = 0;
	encoder->param->i_scenecut_threshold = G_para.encode_scenecout;
	//encoder->param->rc.i_qp_constant = 30;
	encoder->param->rc.i_qp_min = G_para.encoder_qp;
	encoder->param->rc.i_qp_max = G_para.encoder_qp;
	//encoder->param->analyse.i_me_method = X264_ME_DIA;
	encoder->param->analyse.i_me_range = G_para.encode_me_range;
	encoder->param->analyse.i_mv_range = G_para.encode_mv_range;

	encoder->param->i_slice_max_size = G_para.encoder_slice_max_size;

	x264_param_apply_profile(encoder->param, x264_profile_names[0]);

	encoder->handle = x264_encoder_open(encoder->param);

	if (encoder->handle == 0) 
	{
		return;
	}

	x264_picture_alloc(encoder->picture, X264_CSP_I420, 
		encoder->param->i_width,encoder->param->i_height);

	encoder->picture->img.i_csp = X264_CSP_I420;
	encoder->picture->img.i_plane = 3;

	g_H264_Buf = (uint8_t *) malloc(
	sizeof(uint8_t) * g_ImgWidth * g_ImgHeight * 3);

}

int encode_one_frame(int type, uint8_t *frame, uint8_t *h264stream)
{
	return encode_frame(&g_X264Encoder,type, frame, h264stream);
}

int encode_frame(Encoder *encoder, int type, uint8_t *frame, uint8_t *h264stream)
{
	x264_picture_t pic_out;
	int num_Nal = -1;
	int result = 0;
	
	uint8_t *p_out = h264stream;
	uint i = 0;

	uint8_t *Y = encoder->picture->img.plane[0];
	uint8_t *U = encoder->picture->img.plane[1];
	uint8_t *V = encoder->picture->img.plane[2];


	int YUV420_length = encoder->param->i_width * encoder->param->i_height * 1.5;
	memcpy(Y, frame, YUV420_length);

	switch (type) 
	{
		case 0:
		encoder->picture->i_type = X264_TYPE_P;
		break;
		case 1:
		encoder->picture->i_type = X264_TYPE_IDR;
		break;
		case 2:
		encoder->picture->i_type = X264_TYPE_I;
		break;
		default:
		encoder->picture->i_type = X264_TYPE_AUTO;
		break;
	}

	if (x264_encoder_encode(encoder->handle, &(encoder->nal), &num_Nal, 
		encoder->picture, &pic_out) < 0) 
	{
		return -1;
	}

	energy = Get_Energy();
	energy_stage_2[0] = *energy;energy_stage_2[1] = *(energy + 1);energy_stage_2[2] = *(energy + 2);
	NAL_num = num_Nal;
	for (i = 0; i < num_Nal; i++) 
	{
		memcpy(p_out, encoder->nal[i].p_payload, encoder->nal[i].i_payload);
		RTP_send((char *)p_out, encoder->nal[i].i_payload); //Senddata by RTP
		p_out += encoder->nal[i].i_payload;
		result += encoder->nal[i].i_payload;
	}
	if (result > 0)
	{
		//fwrite(h264stream,result,1,H264_FP);
		//RTP_send((char *)h264stream,result); //Senddata by RTP
	}
	return result;
}

void encoder_end(Encoder *encoder) 
{
	if (encoder->picture) {
		x264_picture_clean(encoder->picture);
		free(encoder->picture);
		encoder->picture = 0;
	}
	if (encoder->param) {
		free(encoder->param);
		encoder->param = 0;
	}
	if (encoder->handle) {
		x264_encoder_close(encoder->handle);
	}
	free(g_H264_Buf);
}


void X264_init()
{
	g_ImgWidth = G_para.video_width;
	g_ImgHeight = G_para.video_height;
	encode_init(&g_X264Encoder, g_ImgWidth, g_ImgHeight);
}

void X264_end()
{
	encoder_end(&g_X264Encoder);
}
