#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t  type;
    uint8_t  code;
    uint16_t checksum;
    uint16_t identifier;
    uint16_t sequence;
} icmp_echo_t;

int create_icmp_echo_packet(
    uint16_t sequence,
    const uint8_t *payload,
    size_t payload_size,
    uint8_t *out_buffer
);

uint16_t icmp_checksum(const void *buf, size_t len);

#endif