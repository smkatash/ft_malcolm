#include "ft_malcolm.h"

void	logMACAddress(char *msg, unsigned char *mac)
{
	fprintf(stdout, "%s MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n", msg,
                        mac[0], mac[1], mac[2],
                        mac[3], mac[4], mac[5]);
}

void    logIPAddress(char *msg, unsigned char *ip)
{
    fprintf(stdout, "%s IP address: %u.%u.%u.%u\n", msg, ip[0], ip[1], ip[2], ip[3]);
}