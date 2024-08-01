#ifndef PACK_H
#define PACK_H

unsigned char unpack_u8(unsigned char **buf);
short unpack_u16(unsigned char **buf);
unsigned char* unpack_string(unsigned char **buf, short len);

#endif // !PACK_H
