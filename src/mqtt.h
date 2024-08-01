#ifndef MQTT_H
#define MQTT_H

#include <stdlib.h>

enum pkt_types {
    CONNECT = 1,
    CONNACK = 2,
    PUBLISH = 3,
    PUBACK = 4,
    PUBREC = 5,
    PUBREL = 6,
    PUBCOMP = 7,
    SUBSCRIBE = 8,
    SUBACK = 9,
    UNSUBSCRIBE = 10,
    UNSUBACK = 11,
    PINGREQ = 12,
    PINGRESP = 13,
    DISCONNECT = 14,
};

enum qos_level { AT_MOST_ONCE, AT_LEAST_ONCE, EXACTLY_ONCE };

union fixed_header {
    unsigned char byte;
    struct {
        unsigned retain: 1;
        unsigned qos: 2;
        unsigned dup: 1;
        unsigned pkt_type: 4;
    } bits;
};

struct connect_pkt {
    union fixed_header header;
    union {
        unsigned char byte;
        struct {
            unsigned reserved: 1;
            unsigned clean_session: 1;
            unsigned will: 1;
            unsigned will_qos: 2;
            unsigned will_retain: 1;
            unsigned password: 1;
            unsigned username: 1;
        } bits;
    };
    struct {
        unsigned short keepalive;
        unsigned char *client_id;
        unsigned char *will_topic;
        unsigned char *will_message;
        unsigned char *username;
        unsigned char *password;
    } payload;
};

enum connect_return {
    ACCEPTED = (char)0x00,
    REFUSED_PROTOCOL_VERSION = (char)0x01,
    REFUSED_ID_REJECTED = (char)0x02,
    REFUSED_SERVER_UNAVAILABLE = (char)0x03,
    REFUSED_BAD_CREDENTIALS = (char)0x04,
    REFUSED_NOT_AUTHORIZED = (char)0x05,
};

struct connack_pkt {
    union fixed_header header;
    union {
        unsigned char byte;
        struct {
            unsigned session_present: 1;
            unsigned reserved: 7;
        } bits;
    };
    unsigned char connect_return;
};

struct publish_pkt {
    union fixed_header header;
    unsigned char *topic_name;
    unsigned short pkt_id;
    unsigned char *message;
};

// for PUBACK, PUBREC, PUBREL, PUBCOMP, UNSUBACK packets
struct ack_pkt {
    union fixed_header header;
    unsigned short pkt_id;
};

struct subscribe_pkt {
    union fixed_header header;
    unsigned short pkt_id;
    unsigned short topics_len;
    struct {
        unsigned char *topic_name;
        unsigned char qos;
    } *topic;
};

enum sub_rc {
    SUCCESS_QOS0 = (char)0x00,
    SUCCES_QOS1 = (char)0x01,
    SUCCESS_QOS2 = (char)0x02,
    FAILURE = (char)0x80,
};

struct suback_pkt {
    union fixed_header header;
    unsigned short pkt_id;
    unsigned short rc_len;
    unsigned char* rc;
};

struct unsubscribe_pkt {
    union fixed_header header;
    unsigned short pkt_id;
    unsigned short topics_len;
    unsigned char **topics;
};

union mqtt_pkt {
    struct connect_pkt connect;
    struct connack_pkt connack;
    struct publish_pkt publish;
    struct ack_pkt ack;
    struct subscribe_pkt subscribe;
    struct suback_pkt suback;
    struct unsubscribe_pkt unsubscribe;
};

struct connect_pkt* get_connect_pkt(unsigned char *buf, union fixed_header *header);

unsigned char* pack_connect_pkt(struct connect_pkt *pkt);

#endif // !MQTT_H
