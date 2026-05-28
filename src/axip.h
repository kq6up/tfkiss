/* config.h    general configuration info
 *
 * Copyright 1991, Michael Westerhof, Sun Microsystems, Inc.
 * This software may be freely used, distributed, or modified, providing
 * this header is not removed.
 * 
 */

/* 
 * Adapted for use with TFKISS and renamed to axip.h 
 * by Mark Wahl, DL4YBG, 960309
 *
 * Updated for C99/64-bit correctness: full prototypes, stdint types.
 */

#include <stdint.h>

#define IPPROTO_AX25 93
#define DEFAULT_UDP_PORT 10093

#define IP_MODE		0x10
#define UDP_MODE	0x20
#define TTY_MODE	0x30

#ifndef FNDELAY
#define FNDELAY O_NDELAY
#endif

#ifdef TFKISS_MAIN
#define vextern
#else
#define vextern extern
#endif

vextern int udp_mode;                   /* true if we need a UDP socket */
vextern int ip_mode;                    /* true if we need the raw IP socket */
vextern unsigned short my_udp;          /* the UDP port to use (network byte order) */
vextern unsigned char mycallsign[7];    /* My callsign, shifted ASCII with SSID */
vextern int digi;                       /* True if we are connected to a TNC */
vextern int loglevel;                   /* Verbosity level */

#define MAX_FRAME 2048

#define LOGL1 if(loglevel>0)put_error
#define LOGL2 if(loglevel>1)put_error
#define LOGL3 if(loglevel>2)put_error
#define LOGL4 if(loglevel>3)put_error

/* routing.c */
void route_init(void);
void route_add(unsigned char *ip, unsigned char *call, int udpport, int default_rt);
unsigned char *call_to_ip(unsigned char *call);
void dump_routes(void);

/* process.c */
void process_init(void);
void from_ip(unsigned char *buf, int l);
int from_kiss(unsigned char *buf, int l);
int addrmatch(unsigned char *a, unsigned char *b);
unsigned char *next_addr(unsigned char *f);
void add_crc(unsigned char *buf, int l);
void dump_ax25frame(char *t, unsigned char *buf, int l);

void send_ip(unsigned char *buf, int l, unsigned char *targetip);
int a_to_call(char *text, unsigned char *tcall);
char *call_to_a(unsigned char *tcall);
unsigned short int compute_crc(unsigned char *buf, int l);
int ok_crc(unsigned char *buf, int l);
