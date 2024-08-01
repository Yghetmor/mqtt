#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "pack.h"

unsigned char unpack_u8(unsigned char **buf)
{
    unsigned char output;
    memcpy(&output, *buf, sizeof(char));
    (*buf)++;
    return output;
}

short unpack_u16(unsigned char **buf)
{
    short output;
    memcpy(&output, *buf, sizeof(short));
    (*buf) += sizeof(short);
    return ntohs(output);
}

unsigned char* unpack_string(unsigned char **buf, short len)
{
    unsigned char *output = malloc(len * sizeof(char) + 1);
    memcpy(output, *buf, len * sizeof(char));
    *(output + len) = '\0';
    *buf += len + 1;
    return output;
}
