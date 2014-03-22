/*
Project: Real-time video coding and transimission with energy analysis
File: rtp.cpp
Author: Fei Cheng
Date: Nov. 2013
Platform: x86 PC and ARM Cortex A8 (pcDuino)
Function: real time communication
*/
#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#ifndef WIN32
	#include <netinet/in.h>
	#include <arpa/inet.h>
#else
	#include <winsock2.h>
#endif // WIN32
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "global.h"
using namespace jrtplib;

RTPSession sess;

extern struct Global_Parameter G_para;

void checkerror(int rtperr)
{
	if (rtperr < 0)
	{
		std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
		exit(-1);
	}
}


int RTP_init()
{
	char if_default;
#ifdef WIN32
	WSADATA dat;
	WSAStartup(MAKEWORD(2,2),&dat);
#endif // WIN32
	uint16_t portbase,destport;
	uint32_t destip;
	std::string ipstr;
	int status,i,num;
	//printf("Use default IP and port? (%s:%d)\n",G_para.ip_address_remote.c_str(),G_para.ip_port_remote);
	if_default = 'y';

	if (if_default == 'y')
	{
		portbase=4444;
		ipstr = G_para.ip_address_remote;
		destport = G_para.ip_port_remote;
	}
	else
	{
		std::cout << "Enter local portbase:" << std::endl;
		std::cin >> portbase;
		std::cout << std::endl;
		std::cout << "Enter the destination IP address" << std::endl;
		std::cin >> ipstr;
		std::cout << std::endl;
		std::cout << "Enter the destination port" << std::endl;
		std::cin >> destport;
		std::cout << std::endl;
	}
	
	destip = inet_addr(ipstr.c_str());
	if (destip == INADDR_NONE)
	{
		std::cerr << "Bad IP address specified" << std::endl;
		return -1;
	}
	
	destip = ntohl(destip);
	
	std::cout << std::endl;
	
	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;

	sessparams.SetOwnTimestampUnit(1.0/10.0);		
	
	sessparams.SetAcceptOwnPackets(true);
	transparams.SetPortbase(portbase);
	status = sess.Create(sessparams,&transparams);	
	sess.SetMaximumPacketSize(20480);

	checkerror(status);
	
	RTPIPv4Address addr(destip,destport);
	
	status = sess.AddDestination(addr);
	checkerror(status);
	return 1;
}

int RTP_send(char * sdat, int ndat)
{
	int status,i,num;
	// send the packet
	status = sess.SendPacket((void *)sdat,ndat,96,false,1);
	checkerror(status);

#ifndef RTP_SUPPORT_THREAD
	printf("fuck\n");
		status = sess.Poll();
		checkerror(status);
#endif // RTP_SUPPORT_THREAD
	return 1;
}

int RTP_end()
{
	sess.BYEDestroy(RTPTime(10,0),0,0);

#ifdef WIN32
	WSACleanup();
#endif // WIN32
	return 1;
}

