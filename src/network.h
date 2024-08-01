#ifndef NETWORK_H
#define NETWORK_H

#include "mqtt.h"

int get_pkt(int sock, union mqtt_pkt *out_pkt);
int send_pkt(int sock, union mqtt_pkt *in_pkt);

#endif // !NETWORK_H
