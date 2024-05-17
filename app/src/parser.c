#include "ft_malcolm.h"

char *parse_ip_address(char *addr)
{
	if (!is_ipv4(addr))
	{
		return (NULL);
	}
	return addr;
}

char *parse_mac_address(char *addr)
{
	if (!is_mac_address(addr))
	{
		return (NULL);
	}
	return addr;
}

int parse_input(char **args, t_machine  *devices)
{
	char *ip_source = parse_ip_address(args[1]);
    char *ip_target = parse_ip_address(args[3]);
	if ((!ip_source) || (!ip_target))
	{
		fprintf(stderr, "ft_malcolm: unknown host or invalid IP address\n");
		return (0);
	}
	char *mac_source = parse_mac_address(args[2]);
    char *mac_target = parse_mac_address(args[4]);
	if ((!mac_source) || (!mac_target))
	{
		fprintf(stderr, "ft_malcolm: invalid mac address\n");
		return (0);
	}

	ft_strlcpy(devices->ip_src, parse_ip_address(ip_source), ft_strlen(ip_source) + 1);
	ft_strlcpy(devices->ip_dst, parse_ip_address(ip_target), ft_strlen(ip_target) + 1);
	ft_strlcpy(devices->mac_src, parse_mac_address(mac_source), ft_strlen(mac_source) + 1);
	ft_strlcpy(devices->mac_dst, parse_mac_address(mac_target), ft_strlen(mac_target) + 1);

	// TODO do i need whole struct or only sin?
    inet_pton(AF_INET, devices->ip_src, &(devices->sa_src.sin_addr));
    inet_pton(AF_INET, devices->ip_dst, &(devices->sa_dst.sin_addr));

	return (convert_mac_address_to_bytes(devices->mac_dst, devices->mac_dst_addr));
}