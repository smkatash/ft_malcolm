#include "ft_malcolm.h"

static int    get_interface_address(struct ifaddrs *ifap ,char *ifname, t_machine  *devices)
{
    struct ifaddrs  *ifa;
    int idx = 1;
    int ret = 0;

    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ft_strcmp(ifname, ifa->ifa_name) == 0 && ifa->ifa_addr->sa_family == AF_PACKET) {
            struct sockaddr_ll *s = (struct sockaddr_ll*)ifa->ifa_addr;
            if (s->sll_hatype == ARPHRD_LOOPBACK)
            {
                continue;
            }

            if (s->sll_halen == MAC_LENGTH) {
                ft_memcpy(devices->mac_if, s->sll_addr, MAC_LENGTH);
                fprintf(stdout, "Found correct interface: %s\n", ifa->ifa_name);
                printMACaddress("Interface", devices->mac_if);
                devices->ifindex = s->sll_ifindex;
                ret = 1;
                break;
            }
        }
        idx++;
    }
    return (ret);
}


int get_interface(t_machine  *devices)
{
    struct ifaddrs *ifap, *ifa, *tmp;
    struct sockaddr_in *sa;
    char *addr;
    int ret = 0;

    uint32_t netip_src = devices->sa_src.sin_addr.s_addr;
    uint32_t netip_dst = devices->sa_dst.sin_addr.s_addr;


    getifaddrs (&ifap);
    tmp = ifap;
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            uint32_t netip = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr.s_addr;
            uint32_t netmask = ((struct sockaddr_in *)ifa->ifa_netmask)->sin_addr.s_addr;;
            uint32_t netstart = (netip & netmask);
            uint32_t netend = (netstart | ~netmask);
            if ((netip_src >= netstart) && (netip_src <= netend) && (netip_dst >= netstart) && (netip_dst <= netend)) {
                fprintf(stdout, "Found correct interface: %s\tAddress: %s\n", ifa->ifa_name, addr);
                fprintf(stdout, "src: %u\t target: %u\n", netip_src, netip_dst);
                ret = get_interface_address(tmp, ifa->ifa_name, devices);
                if (ret) {
                    struct sockaddr_in *ip_addr = (struct sockaddr_in*)ifa->ifa_addr;
                    ft_memcpy(devices->ip_if, &(ip_addr->sin_addr), IPV4_LENGTH);
                    fprintf(stdout, "IP address: %s\n", inet_ntoa(ip_addr->sin_addr));
                    fprintf(stdout, "IP address: %02x:%02x:%02x:%02x\n",
                        devices->ip_if[0], devices->ip_if[1], devices->ip_if[2],
                        devices->ip_if[3]);
                    break;
                }
            }
        }
    }
    freeifaddrs(ifap);
    return ret;
}

int listen_to_broadcast(t_machine *devices)
{
    unsigned char buffer[sizeof(struct ethhdr) + sizeof(struct arppckt)];
    ssize_t packet_size;
    struct sockaddr_ll recv_addr;
    socklen_t addr_len = sizeof(recv_addr);
    
    while (1) {
        packet_size = recvfrom(devices->sock_fd, &buffer, BUFFER_SIZE, 0, (struct sockaddr *)&recv_addr, &addr_len);
        if (packet_size == -1)
        {
            fprintf(stderr, "Error receiving packet: %s\n", strerror(errno));
            continue;
        }

        if (devices->sockaddr_dst.sll_ifindex != recv_addr.sll_ifindex)
        {
            continue;
        }
        if (!process_ethernet_packet(buffer, devices))
        {
            break;
        }
    }
    return (1);
}
