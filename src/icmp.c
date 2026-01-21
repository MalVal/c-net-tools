#include "icmp.h"

#include <unistd.h>
#include <string.h>

int create_icmp_echo_packet(uint16_t sequence, const uint8_t *payload, size_t payload_size, uint8_t *out_buffer) {
    uint16_t identifier = (uint16_t)(getpid() & 0xFFFF);

    icmp_echo_t header;
    memset(&header, 0, sizeof(header));
    header.type = 8;       // Echo Request
    header.code = 0;
    header.identifier = identifier;
    header.sequence = sequence;
    header.checksum = 0;

    memcpy(out_buffer, &header, sizeof(header));
    memcpy(out_buffer + sizeof(header), payload, payload_size);

    uint16_t checksum = icmp_checksum(out_buffer, sizeof(header) + payload_size);

    ((icmp_echo_t *)out_buffer)->checksum = checksum;

    return 0;
}

uint16_t icmp_checksum(const void *buf, size_t len) {
    uint32_t sum = 0;
    const uint16_t *data = buf;

    while (len > 1) {
        sum += *data++;
        len -= 2;
    }

    if (len == 1) {
        sum += *((uint8_t*)data);
    }

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return ~sum;
}