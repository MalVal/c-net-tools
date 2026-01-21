#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include "icmp.h"

int tool_ping(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: cnet ping <address>\n");
        return 1;
    }

    const char *address = argv[1];
    printf("pinging %s...\n", address);

    uint8_t payload[32];
    struct timeval tv;
    gettimeofday(&tv, NULL);
    memcpy(payload, &tv, sizeof(tv));
    memset(payload + sizeof(tv), 'A', sizeof(payload) - sizeof(tv));

    uint8_t packet[sizeof(icmp_echo_t) + sizeof(payload)];
    static uint16_t sequence = 0;
    sequence++;
    create_icmp_echo_packet(sequence, payload, sizeof(payload), packet);

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(address);

    ssize_t sent = sendto(sockfd, packet, sizeof(packet), 0,
                          (struct sockaddr *)&dest, sizeof(dest));
    if (sent < 0) {
        perror("sendto");
        close(sockfd);
        return 1;
    }
    printf("packet sent!\n");

    uint8_t recv_buf[1024];
    struct sockaddr_in reply_addr;
    socklen_t addr_len = sizeof(reply_addr);

    ssize_t received = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0,
                                (struct sockaddr *)&reply_addr, &addr_len);
    if (received < 0) {
        perror("recvfrom");
        printf("No reply received (timeout or blocked)\n");
        close(sockfd);
        return 1;
    }

    icmp_echo_t *reply = (icmp_echo_t *)recv_buf;

    if (reply->type == 0 && reply->identifier == (uint16_t)(getpid() & 0xFFFF)) {
        printf("Received ICMP reply, sequence=%u\n", reply->sequence);

        struct timeval now;
        gettimeofday(&now, NULL);

        struct timeval *sent_time = (struct timeval *)(recv_buf + sizeof(icmp_echo_t));
        long rtt_ms = (now.tv_sec - sent_time->tv_sec) * 1000 +
                      (now.tv_usec - sent_time->tv_usec) / 1000;

        printf("RTT = %ld ms\n", rtt_ms);
    } 
    else {
        printf("Received ICMP packet, but not a reply to our ping.\n");
    }

    close(sockfd);
    return 0;
}
