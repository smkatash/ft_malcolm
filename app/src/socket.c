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
    ft_memset(&(devices)->sockaddr_dst, 0, sizeof(struct sockaddr_ll));
    devices->sockaddr_dst.sll_ifindex = devices->ifindex;
    devices->sockaddr_dst.sll_family = AF_PACKET;
    devices->sockaddr_dst.sll_protocol = htons(ETH_P_ARP);
    devices->sockaddr_dst.sll_hatype = htons(ARPHRD_ETHER);
    devices->sockaddr_dst.sll_pkttype = (PACKET_BROADCAST);  // default -> unspecified
    devices->sockaddr_dst.sll_halen = ETH_ALEN;
    ft_memcpy(devices->sockaddr_dst.sll_addr, devices->mac_dst_addr, sizeof(devices->mac_dst_addr));
    char buffer[16];
    printf("Interface index: %s\n", if_indextoname(devices->sockaddr_dst.sll_ifindex, buffer));
    logMACAddress("Target socket ", devices->sockaddr_dst.sll_addr);
}