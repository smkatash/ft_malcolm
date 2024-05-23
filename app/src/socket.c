#include "ft_malcolm.h"

int init_socket(t_malcolm  *data)
{
    if ((data->sock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1) {
        log_error(strerror(errno));
        return (1);
    }
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    if (setsockopt(data->sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        log_error(strerror(errno));
        close(data->sock_fd);
        return (1);
    }
    return (0);
}

void    init_sockaddr_dest(t_malcolm  *data)
{
    ft_memset(&(data)->sockaddr_target, 0, sizeof(struct sockaddr_ll));
    data->sockaddr_target.sll_ifindex = data->ifindex;
    data->sockaddr_target.sll_family = AF_PACKET;
    data->sockaddr_target.sll_protocol = htons(ETH_P_ARP);
    data->sockaddr_target.sll_hatype = htons(ARPHRD_ETHER);
    data->sockaddr_target.sll_pkttype = 0;  // default -> unspecified
    data->sockaddr_target.sll_halen = ETH_ALEN;
    ft_memcpy(data->sockaddr_target.sll_addr, data->target_mac, sizeof(data->target_mac));
}