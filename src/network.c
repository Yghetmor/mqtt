#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "network.h"
#include "mqtt.h"

int get_pkt(int sock, union mqtt_pkt *out_pkt)
{
    union fixed_header pkt_header;
    if (recv(sock, &pkt_header, 1, 0) != 1)
    {
        fprintf(stderr, "Problem getting packet header");
        return -1;
    }

    int multiplier = 1;
    int remaining_length = 0;
    unsigned char encoded_byte;
    do {
        if (recv(sock, &encoded_byte, 1, 0) != 1)
        {
            fprintf(stderr, "Problem getting remaining length");
            return -1;
        }
        remaining_length += (int)(encoded_byte & 127) * multiplier;
        multiplier *= 128;
        if (multiplier > 128*128*128)
        {
            fprintf(stderr, "Malformed remaining length");
            return -1;
        }
    } while (encoded_byte & 128);

    unsigned char *const pkt_buf = malloc(remaining_length * sizeof(char));
    unsigned char *rem_ptr = pkt_buf;
    ssize_t tot = 0;
    do {
        ssize_t rc = 0;
        if ((rc = recv(sock, &rem_ptr, tot, 0)) < 0)
        {
            fprintf(stderr, "Error while getting rest of packet");
            return -1;
        }
        tot += rc;
        rem_ptr += rc * sizeof(char);
    } while (tot != remaining_length);

    // Use pkt_buf to get the packet;
    struct connect_pkt* packet;
    if ((packet = get_connect_pkt(pkt_buf, &pkt_header)) == NULL)
    {
        fprintf(stderr, "Error while getting connect packet");
        return -1;
    }

    free(pkt_buf);

    out_pkt->connect = *packet;
    return 0;
}

int send_pkt(int sock, union mqtt_pkt *in_pkt)
{

}
