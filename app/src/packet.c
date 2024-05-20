#include "ft_malcolm.h"


static int process_arp_packet(unsigned char* buffer, t_machine *devices)
{
    struct arppckt *arp_packet = (struct arppckt *)buffer;
    int op_code = -1;
    
    if (ntohs(arp_packet->ar_pro) != ETH_P_IP || ntohs(arp_packet->ar_hrd) != ARPHRD_ETHER) {
        return (1);
    }

    op_code = ntohs(arp_packet->ar_op);
    if (op_code != ARPOP_REQUEST) {
        return (1);
    }
    ft_memcpy(&(devices->sa_src).sin_addr, arp_packet->ar_tip, IPV4_LENGTH);
    unsigned char *ar_sha = arp_packet->ar_sha;
    unsigned char *ar_tha = arp_packet->ar_tha;
    unsigned char *ar_sip = arp_packet->ar_sip;
    unsigned char *ar_tip = arp_packet->ar_tip;

    printf("ARP Packet Info:\n");
    printf("  Operation: %s\n", op_code == ARPOP_REQUEST ? "ARP Request" : "ARP Reply");
    logIPAddress(" Sender ", ar_sip);
    logIPAddress(" Target ", ar_tip);
    logMACAddress(" Sender ", ar_sha);
    logMACAddress(" Target ", ar_tha);

    return (0);
}

int process_ethernet_packet(unsigned char *buffer, t_machine *devices) {
    struct ethhdr *eth_header = (struct ethhdr *)buffer;
    unsigned char *src_mac = eth_header->h_source;
    unsigned char *dst_mac = eth_header->h_dest;
    char src_mac_str[MAC_ADDRESS_MAX_LEN];
    printf("Ethernet Header Info:\n");
    logMACAddress(" Sender ", src_mac);
    logMACAddress(" Target ", dst_mac);
    
    snprintf(src_mac_str, sizeof(src_mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
             src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5]);

    printf("%s and %s\n", devices->mac_dst, src_mac_str);
    if (ft_strcmp(devices->mac_dst, src_mac_str))
    {
        return (1);
    }

    return process_arp_packet(buffer + sizeof(struct ethhdr), devices);
}

void    create_arp_packet(unsigned char *packet, int type, t_machine *device)
{
    struct arppckt arp;

    ft_memset(&arp, 0, sizeof(struct arppckt));
    arp.ar_hrd = htons(1); // Ethernet
    arp.ar_pro = htons(0x0800); // IPv4 0x0800
    arp.ar_hln = ETH_ALEN; // MAC address length
    arp.ar_pln = IPV4_LENGTH; // IP address length
    arp.ar_op = htons(type); // ARP reply
    ft_memcpy(arp.ar_sha, device->mac_src_addr, ETH_ALEN);
    ft_memcpy(arp.ar_sip, &(device->sa_src).sin_addr, IPV4_LENGTH);
    ft_memcpy(arp.ar_tha, device->mac_dst_addr, ETH_ALEN);
    ft_memcpy(arp.ar_tip, &(device->sa_dst).sin_addr, IPV4_LENGTH);
    ft_memcpy(packet, &arp, sizeof(arp));
}

void    create_ethernet_packet(unsigned char *packet, t_machine *device)
{
    struct ethhdr ehdr;

    ft_memset(&ehdr, 0, sizeof(struct ethhdr));
    ft_memcpy(ehdr.h_source, device->mac_src_addr, sizeof(device->mac_src_addr));
    ft_memcpy(ehdr.h_dest, device->mac_dst_addr, sizeof(device->mac_dst_addr));
    ehdr.h_proto = htons(ETH_P_ARP);
    ft_memcpy(packet, &ehdr, sizeof(ehdr));
}

void    create_reply_packet(unsigned char *packet, t_machine *device)
{
    create_ethernet_packet(packet, device);
    create_arp_packet((packet + sizeof(struct ethhdr)), 2, device);
}