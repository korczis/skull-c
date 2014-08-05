#ifndef SKULL_HELPERS_H
#define SKULL_HELPERS_H

/*
 * in_cksum --
 * Checksum routine for Internet Protocol
 * family headers (C Version)
 */
unsigned short cksum(unsigned short *addr, int len);

#endif // SKULL_HELPERS_H
