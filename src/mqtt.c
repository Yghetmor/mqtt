#include <string.h>
#include <netinet/in.h>
#include "mqtt.h"
#include "pack.h"

struct connect_pkt* get_connect_pkt(unsigned char *buf, union fixed_header *header)
{
    buf += 8;
    struct connect_pkt *con_pkt = malloc(sizeof(struct connect_pkt));
    con_pkt->header = *header;
    con_pkt->byte = unpack_u8(&buf);
    con_pkt->payload.keepalive = unpack_u16(&buf);

    short id_len = unpack_u16(&buf);
    unsigned char *client_id = unpack_string(&buf, id_len);

    if (con_pkt->bits.will)
    {
        short will_topic_len = unpack_u16(&buf);
        con_pkt->payload.will_topic = unpack_string(&buf, will_topic_len);
        short will_message_len = unpack_u16(&buf);
        con_pkt->payload.will_message = unpack_string(&buf, will_message_len);
    }
    else 
    {
        con_pkt->payload.will_topic = con_pkt->payload.will_message = NULL;
    }

    if (con_pkt->bits.username)
    {
        short username_len = unpack_u16(&buf);
        con_pkt->payload.username = unpack_string(&buf, username_len);
    }
    else
    {
        con_pkt->payload.username = NULL;
    }

    if (con_pkt->bits.password)
    {
        short pass_len = unpack_u16(&buf);
        con_pkt->payload.password = unpack_string(&buf, pass_len);
    }

    return con_pkt;
}
