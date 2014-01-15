#ifndef _RTPH_H
#define _RTPH_H

int RTP_init();
int RTP_send(char * sdat, int ndat);
int RTP_end();

#endif