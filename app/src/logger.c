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

void    log_error(const char *msg)
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
    fprintf(stdout, "\n\t\t\t[     Packet Information     ]\n");
    struct ethhdr *eth_header = (struct ethhdr *)packet;
    struct arppckt *arp_packet = (struct arppckt *)(packet + sizeof(struct ethhdr));
    fprintf(stdout, "Ethernet Header Info:\n");
    log_mac_address(" - Sender mac address: ", eth_header->h_source);
    log_mac_address(" - Target mac address: ", eth_header->h_dest);
    fprintf(stdout, " - Protocol: %s\n", ETH_PROTOCOL_TO_STRING(eth_header->h_proto));

    fprintf(stdout, "ARP Packet Info:\n");
    fprintf(stdout, " - Format of hardware address: %s\n", ARP_HRD_TO_STRING(arp_packet->ar_hrd));
    fprintf(stdout, " - Format of protocol address: %s\n", ARP_PRO_TO_STRING(arp_packet->ar_pro));
    fprintf(stdout, " - Length of hardware address: %u\n", arp_packet->ar_hln);
    fprintf(stdout, " - Length of protocol address: %u\n", arp_packet->ar_pln);
    fprintf(stdout, " - ARP opcode: %s\n", ARP_OPCODE_TO_STRING(arp_packet->ar_op));
    log_mac_address(" - Sender mac address: ", arp_packet->ar_sha);
    log_mac_address(" - Target mac address: ", arp_packet->ar_tha);
    log_ip_address(" - Sender IP address:", arp_packet->ar_sip);
    log_ip_address(" - Target IP address:", arp_packet->ar_tip);
}