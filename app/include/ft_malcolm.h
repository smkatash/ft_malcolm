#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include <unistd.h>
#include "../libs/libft/libft.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <linux/if_arp.h>


# define MAC_ADDRESS_MAX_LEN 18 // 17 + 1
# define USAGE_INFO "usage: ./ft_malcolm [source ip] [source mac address] [target ip] [target mac address]\n"
# define MAC_LENGTH 6
# define IPV4_LENGTH 4
# define BUFFER_SIZE 2048

/* from header if_arp.h for ethernet */
struct arppckt {
	uint16_t				ar_hrd;		/* format of hardware address	*/
	uint16_t				ar_pro;		/* format of protocol address	*/
	unsigned char			ar_hln;		/* length of hardware address	*/
	unsigned char			ar_pln;		/* length of protocol address	*/
	uint16_t				ar_op;		/* ARP opcode (command)		*/
	 /*
	  *	 Ethernet looks like this : This bit is variable sized however...
	  */
	unsigned char		ar_sha[ETH_ALEN];	/* sender hardware address	*/
	unsigned char		ar_sip[IPV4_LENGTH];		/* sender IP address		*/
	unsigned char		ar_tha[ETH_ALEN];	/* target hardware address	*/
	unsigned char		ar_tip[IPV4_LENGTH];		/* target IP address		*/
};

typedef struct s_machine
{
	char				ip_src[INET_ADDRSTRLEN];
	char				mac_src[MAC_ADDRESS_MAX_LEN];
	char				ip_dst[INET_ADDRSTRLEN];
	char				mac_dst[MAC_ADDRESS_MAX_LEN];
	unsigned char		mac_src_addr[MAC_LENGTH];
	unsigned char		mac_dst_addr[MAC_LENGTH];
	struct	sockaddr_in sa_src;
    struct	sockaddr_in sa_dst;
	unsigned char		ip_if[IPV4_LENGTH];
	unsigned char		mac_if[MAC_LENGTH];
	struct sockaddr_ll	sockaddr_dst; // sockaddr_ll structure is used to specify on which interface to send the packet and which protocol to set in the Ethernet header (EtherType field).
	int					ifindex;
	int					sock_fd;
}			t_machine;

int		is_ipv4(const char *ip);
int		is_mac_address(const char *mac);
int		parse_input(char **args, t_machine  *devices);
int		init_socket(t_machine  *devices);
void	init_sockaddr_dest(t_machine  *devices);
int		convert_mac_address_to_bytes(char *macAddr, unsigned char *mac_dst_addr);
int		ft_malcolm(t_machine *input);
int		get_interface(t_machine  *devices);
int		process_ethernet_packet(unsigned char *buffer, t_machine *devices);
int		listen_to_broadcast(t_machine *devices);
void    create_reply_packet(unsigned char *packet, t_machine *device);
int		send_reply(t_machine *devices);
void	logMACAddress(char *msg, unsigned char *mac);
void    logIPAddress(char *msg, unsigned char *ip);




#endif