#include "ft_malcolm.h"


static int process_arp_packet(unsigned char* buffer)
{
    struct arphdr *arp_header = (struct arphdr *)buffer;
    int op_code = -1;
    struct in_addr sender_ip, target_ip;
    unsigned char *sender_mac, *target_mac;
    char sender_mac_str[MAC_ADDRESS_MAX_LEN + 1], target_mac_str[MAC_ADDRESS_MAX_LEN + 1];
    
    if (ntohs(arp_header->ar_pro) != ETH_P_IP || ntohs(arp_header->ar_hrd) != ARPHRD_ETHER) {
        return (1);
    }

    op_code = ntohs(arp_header->ar_op);
    if (op_code != ARPOP_REQUEST) {
        return (1);
    }
    sender_ip.s_addr = *((unsigned long*)(buffer + sizeof(struct arphdr) + sizeof(unsigned short) * 2));
    target_ip.s_addr = *((unsigned long*)(buffer + sizeof(struct arphdr) + sizeof(unsigned short) * 2 + arp_header->ar_hln + arp_header->ar_pln));
    sender_mac = buffer + sizeof(struct arphdr) + sizeof(unsigned short) * 2 + arp_header->ar_hln + arp_header->ar_pln + arp_header->ar_hln;
    target_mac = buffer + sizeof(struct arphdr) + sizeof(unsigned short) * 2 + arp_header->ar_hln + arp_header->ar_pln + arp_header->ar_hln + arp_header->ar_pln;
    snprintf(sender_mac_str, sizeof(sender_mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
             sender_mac[0], sender_mac[1], sender_mac[2], sender_mac[3], sender_mac[4], sender_mac[5]);
    snprintf(target_mac_str, sizeof(target_mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
             target_mac[0], target_mac[1], target_mac[2], target_mac[3], target_mac[4], target_mac[5]);

    printf("ARP Packet Info:\n");
    printf("  Operation: %s\n", op_code == ARPOP_REQUEST ? "ARP Request" : "ARP Reply");
    printf("  Sender IP: %s\n", inet_ntoa(sender_ip));
    printf("  Sender MAC: %s\n", sender_mac_str);
    printf("  Target IP: %s\n", inet_ntoa(target_ip));
    printf("  Target MAC: %s\n", target_mac_str);
    return (0);
}

int process_ethernet_packet(unsigned char *buffer, t_machine *devices) {
    struct ethhdr *eth_header = (struct ethhdr *)buffer;
    unsigned char *src_mac = eth_header->h_source;
    unsigned char *dst_mac = eth_header->h_dest;

   char src_mac_str[MAC_ADDRESS_MAX_LEN + 1], dst_mac_str[MAC_ADDRESS_MAX_LEN + 1];
    snprintf(src_mac_str, sizeof(src_mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
             src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5]);
    snprintf(dst_mac_str, sizeof(dst_mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
             dst_mac[0], dst_mac[1], dst_mac[2], dst_mac[3], dst_mac[4], dst_mac[5]);
    printf("Ethernet Header Info:\n");
    printf("  Source MAC: %s\n", src_mac_str);
    printf("  Destination MAC: %s\n", dst_mac_str);

    if (ft_strcmp(devices->mac_dst, src_mac_str))
    {
        return (1);
    }

    return process_arp_packet(buffer + sizeof(struct ethhdr));
}

// void    create_arp_reply(unsigned char *packet, t_machine *device)
//{

//}