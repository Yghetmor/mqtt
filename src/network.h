#ifndef NETWORK_H
#define NETWORK_H

#include "mqtt.h"

int get_pkt(int sock);
int send_pkt(int sock);

#endif // !NETWORK_H
