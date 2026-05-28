/* process.c     Handle processing and routing of AX25 frames
 *
 * Copyright 1991, Michael Westerhof, Sun Microsystems, Inc.
 * This software may be freely used, distributed, or modified, providing
 * this header is not removed.
 *
 * This is the only module that knows about the internal structure of
 * AX25 frames.
 */
/*
 * Adapted for use with TFKISS by Mark Wahl, DL4YBG, 960309
 * Updated for C99/64-bit correctness: ANSI prototypes, fixed printf misuse
 * in dump_ax25frame (was calling printf with format in wrong argument).
 */

#include "config.h"

#ifdef USE_AXIP
#include "axip.h"
#include <stdio.h>
#include <string.h>

#define IS_LAST(p)      (((*(p+6))&0x01)!=0)
#define NOT_LAST(p)     (((*(p+6))&0x01)==0)
#define REPEATED(p)     (((*(p+6))&0x80)!=0)
#define NOTREPEATED(p)  (((*(p+6))&0x80)==0)
#define IS_ME(p)        addrmatch(p,mycallsign)
#define NOT_ME(p)       (!addrmatch(p,mycallsign))
#define ARE_DIGIS(f)    (((*(f+13))&0x01)==0)
#define NO_DIGIS(f)     (((*(f+13))&0x01)!=0)
#define SETREPEATED(p)  (*(p+6))|=0x80
#define SETLAST(p)      (*(p+6))|=0x01

/*
 * Declaration of external functions
 */
extern void send_kiss(unsigned char type, unsigned char *buf, int len);
extern void put_error(char *str);

/*
 * Initialize the process variables
 */
void
process_init(void)
{
}

/*
 * handle a frame given us by the kiss routines.
 */
int
from_kiss(unsigned char *buf, int l)
{
	unsigned char *a, *ipaddr;

	if (l < 15) {
		LOGL2("from_kiss: dumped - length wrong!");
		return 0;
	}

	if (loglevel > 2) dump_ax25frame("from_kiss: ", buf, l);

	if (digi) {               /* if we are in digi mode */
		a = next_addr(buf);
		if (NOT_ME(a)) {
			return 0;
		}
		if (a == buf) {     /* must be a digi */
			return 0;
		}
		SETREPEATED(a);
		a = next_addr(buf);     /* find who gets it after us */
	} else {                /* must be tnc mode */
		a = next_addr(buf);
	}                       /* end of tnc mode */

	ipaddr = call_to_ip(a);
	if(ipaddr==NULL){
		if (digi) return 1;
		return 0;
	}
	/* WARNING -- we assume here that the buffer is large enough to have
	 * two bytes tacked on... This is assured in the current version by
	 * "from_kiss" -- the packet is dumped as too large if there will not
	 * be sufficient space.  Use caution!
	 */
	add_crc(buf, l);
	send_ip(buf, l+2, ipaddr);
	return 1;
}

/*
 * handle a frame given us by the IP routines.
 */
void
from_ip(unsigned char *buf, int l)
{
	int port = 0;
	unsigned char *a;

	if (!ok_crc(buf, l)) {
		LOGL2("from_ip: dumped - CRC incorrect!");
		return;
	}
	l = l - 2;      /* dump the blasted CRC */

	if (l < 15) {
		LOGL2("from_ip: dumped - length wrong!");
		return;
	}

	if (loglevel > 2) dump_ax25frame("from_ip: ", buf, l);

	if (digi) {               /* if we are in digi mode */
		a = next_addr(buf);
		if (NOT_ME(a)) {
			LOGL2("from_ip: (digi) dumped - not for me!");
			return;
		}
		if (a == buf) {     /* must be a digi */
			LOGL2("from_ip: (digi) dumped - I am destination!");
			return;
		}
		SETREPEATED(a);
	} else {                /* must be tnc mode */
		a = next_addr(buf);
	}                       /* end of tnc mode */
	send_kiss(port, buf, l);
}

/*
 * return true if the addresses supplied match
 */
int
addrmatch(unsigned char *a, unsigned char *b)
{
	if ((*a == '\0') || (*b == '\0')) return 0;

	if ((*a++ ^ *b++) & 0xfe) return 0;   /* "K" */
	if ((*a++ ^ *b++) & 0xfe) return 0;   /* "A" */
	if ((*a++ ^ *b++) & 0xfe) return 0;   /* "9" */
	if ((*a++ ^ *b++) & 0xfe) return 0;   /* "W" */
	if ((*a++ ^ *b++) & 0xfe) return 0;   /* "S" */
	if ((*a++ ^ *b++) & 0xfe) return 0;   /* "B" */
	if ((*a++ ^ *b++) & 0x1e) return 0;   /* ssid */
	return 1;
}

/*
 * return pointer to the next station to get this packet
 */
unsigned char *
next_addr(unsigned char *f)
{
	unsigned char *a;

/* If no digis, return the destination address */
	if (NO_DIGIS(f)) return f;

/* check each digi field.  The first one that hasn't seen it is the one */
	a = f + 7;
	do {
		a += 7;
		if (NOTREPEATED(a)) return a;
	} while (NOT_LAST(a));

/* all the digis have seen it.  return the destination address */
	return f;       
}

/*
 * tack on the CRC for the frame.
 */
void
add_crc(unsigned char *buf, int l)
{
	unsigned short int u;

	u = compute_crc(buf, l);
	buf[l]   = u & 0xff;           /* lsb first */
	buf[l+1] = (u >> 8) & 0xff;    /* msb next */
}

/*
 * Dump AX25 frame.
 */
void
dump_ax25frame(char *t, unsigned char *buf, int l)
{
#ifdef DEBUG
	int i;
#endif
	unsigned char *a;
	char tmpstr[256];
	char hlpstr[256];

	/* original code called printf(tmpstr, ...) with the format in the wrong
	 * argument position; use snprintf throughout for correctness and safety. */
	snprintf(hlpstr, sizeof(hlpstr), "%s AX25: (l=%3d)   ", t, l);

	if (l < 15) {
		strncat(hlpstr, "Bogus size...", sizeof(hlpstr) - strlen(hlpstr) - 1);
		put_error(hlpstr);
		return;
	}

	snprintf(tmpstr, sizeof(tmpstr), "%s -> ", call_to_a(buf+7));
	strncat(hlpstr, tmpstr, sizeof(hlpstr) - strlen(hlpstr) - 1);
	snprintf(tmpstr, sizeof(tmpstr), "%s", call_to_a(buf));
	strncat(hlpstr, tmpstr, sizeof(hlpstr) - strlen(hlpstr) - 1);

	if (ARE_DIGIS(buf)) {
		strncat(hlpstr, " v", sizeof(hlpstr) - strlen(hlpstr) - 1);
		a = buf + 7;
		do {
			a += 7;
			snprintf(tmpstr, sizeof(tmpstr), " %s", call_to_a(a));
			strncat(hlpstr, tmpstr, sizeof(hlpstr) - strlen(hlpstr) - 1);
			if (REPEATED(a))
				strncat(hlpstr, "*", sizeof(hlpstr) - strlen(hlpstr) - 1);
		} while (NOT_LAST(a));
	}

	put_error(hlpstr);

#ifdef DEBUG
	for (i = 0; i < l; i++) (void)printf("%02x ", buf[i]);
	(void)printf("\n");
#endif

	(void)fflush(stdout);
}
#endif
