#include "ft_malcolm.h"

void	log_mac_address(char *msg, unsigned char *mac)
{
	fprintf(stdout, "%s %02x:%02x:%02x:%02x:%02x:%02x\n", msg,
                        mac[0], mac[1], mac[2],
                        mac[3], mac[4], mac[5]);
}

void    log_ip_address(char *msg, unsigned char *ip)
{
    fprintf(stdout, "%s %u.%u.%u.%u\n", msg, ip[0], ip[1], ip[2], ip[3]);
}

void    log_error(char *msg)
{
    fprintf(stderr, "ft_malcolm: %s\n", msg);
}

void   log_msg(char *msg, char *str)
{
    if (!str)
    {
        fprintf(stdout, "%s\n", msg);
        return ;
    }
    fprintf(stdout, "%s %s\n", msg, str);
}

void    log_packet(unsigned char *packet)
{
    // struct ethhdr *eth_header = (struct ethhdr *)packet;
    // struct arppckt *arp_packet = (struct arppckt *)(packet + sizeof(struct ethhdr));
     // printf("Ethernet Header Info:\n");
        // log_mac_address(" Sender ", src_mac);
        // log_mac_address(" Target ", dst_mac);
        // char src_mac_str[18];
        
        // snprintf(src_mac_str, sizeof(src_mac_str), "%02x:%02x:%02x:%02x:%02x:%02x",
        //         src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5]);
        // printf("ARP Packet Info:\n");
        // printf("  Operation: %s\n", op_code == ARPOP_REQUEST ? "ARP Request" : "ARP Reply");
        // log_ip_address(" Sender ", arp_packet->ar_sip);
        // log_ip_address(" Target ", arp_packet->ar_tip);
        // log_mac_address(" Sender ", arp_packet->ar_sha);
        // log_mac_address(" Target ", arp_packet->ar_tha);
        // op_code = ntohs(arp_packet->ar_op);
    // unsigned char *ar_sha = arp_packet->ar_sha;
    // unsigned char *ar_tha = arp_packet->ar_tha;
    // unsigned char *ar_sip = arp_packet->ar_sip;
    // unsigned char *ar_tip = arp_packet->ar_tip;

    // printf("ARP Packet Info:\n");
    // printf("  Operation: %s\n", op_code == ARPOP_REPLY ? "ARP Reply" : "ARP Request");
    // log_ip_address(" Sender ", ar_sip);
    // log_ip_address(" Target ", ar_tip);
    // log_mac_address(" Sender ", ar_sha);
    // log_mac_address(" Target ", ar_tha);
}