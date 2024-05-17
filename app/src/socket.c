#include "ft_malcolm.h"

int init_socket(t_machine  *devices)
{
    if ((devices->sock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1) {
        fprintf(stderr, "ft_malcolm: %s\n", strerror(errno));
        return (0);
    }
    return (1);
}

void    init_sockaddr_dest(t_machine  *devices)
{
    ft_memcpy(devices->sockaddr_dst.sll_addr, devices->mac_dst_addr, sizeof(devices->mac_dst_addr));
    devices->sockaddr_dst.sll_family = AF_PACKET;
    devices->sockaddr_dst.sll_protocol = htons(ETH_P_ARP);
    devices->sockaddr_dst.sll_ifindex = devices->ifindex;
    devices->sockaddr_dst.sll_hatype = htons(ARPHRD_ETHER);
    devices->sockaddr_dst.sll_pkttype = 0; // default -> unspecified
    devices->sockaddr_dst.sll_halen = MAC_LENGTH;
    printMACaddress("Target socket ", devices->sockaddr_dst.sll_addr);
}