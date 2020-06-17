#include <string.h>
#include <errno.h>

#include "FreeD.h"

static double unpack_be24_15(unsigned char *buf)
{
    int r = -1;

    r <<= 8;
    r |= buf[0];
    r <<= 8;
    r |= buf[1];
    r <<= 8;
    r |= buf[2];
    if (!(r & 0x00800000))
        r &= 0x00ffffff;

    return (double)r / 32768.0;
};

static double unpack_be24_6(unsigned char *buf)
{
    int r = -1;

    r <<= 8;
    r |= buf[0];
    r <<= 8;
    r |= buf[1];
    r <<= 8;
    r |= buf[2];
    if (!(r & 0x00800000))
        r &= 0x00ffffff;

    return (double)r / 64.0;
};

static int unpack_be24(unsigned char *buf)
{
    int r = 0;

    r |= buf[0];
    r <<= 8;
    r |= buf[1];
    r <<= 8;
    r |= buf[2];

    return r;
};

int FreeD_D1_unpack(unsigned char *buf, int len, FreeD_D1_t* dst)
{
    memset(dst, 0, sizeof(*dst));

    if (len != FREE_D_D1_PACKET_SIZE)
        return -EINVAL;

    if (buf[0] != 0xD1)
        return -EFAULT;

    dst->ID = buf[1];

    dst->Pan = unpack_be24_15(buf + 2);
    dst->Tilt = unpack_be24_15(buf + 5);
    dst->Roll = unpack_be24_15(buf + 8);

    dst->X = unpack_be24_6(buf + 11);
    dst->Y = unpack_be24_6(buf + 14);
    dst->Z = unpack_be24_6(buf + 17);

    dst->Zoom = unpack_be24(buf + 20);
    dst->Focus = unpack_be24(buf + 23);

    dst->Spare[0] = buf[26];
    dst->Spare[1] = buf[27];

    return 0;
}