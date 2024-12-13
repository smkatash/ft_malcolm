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
#include <signal.h>
#include <limits.h>

# define MAC_ADDRESS_STRING_LEN 18 // 17 + 1
# define MAC_ADDRESS_BYTE_LENGTH 6
# define IPV4_LENGTH 4
# define MIN_ETHERNET_FRAME_SIZE 64 
# define ETH_FRAME_SIZE (sizeof(struct arppckt) + sizeof(struct ethhdr))
# define VERBOSE "-v"
# define HOSTNAME "-h"
# define GRATUTIOUS "-g"
# define USAGE_INFO "usage: ./ft_malcolm [source ip] [source mac address] [target ip] [target mac address] "\
					"[...options]\n"\
					"\t\t-v	verbose mode to print packet information.\n"\
					"\t\t-g	gratuitous ARP broadcast.\n"\
					"\t\t-h	hostname resolution for IPv4 addresses.\n"
// frame minimum acceptable length of 64 bytes (ethhdr 14 bytes + arp packet 28 bytes + 4 bytes trailing) + 18 of padding

#define ARP_REQUEST 1
#define ARP_REPLY 2
#define RARP_REQUEST 3
#define RARP_REPLY 4
#define DRARP_REQUEST 5
#define DRARP_REPLY 6
#define DRARP_ERROR 7
#define INARP_REQUEST 8
#define INARP_REPLY 9

#define ETH_PROTOCOL_TO_STRING(protocol) (\
    (protocol) == htons(ETH_P_IP) ? "Internet Protocol (IP)" : \
    (protocol) == htons(ETH_P_ARP) ? "Address Resolution Protocol (ARP)" : \
    (protocol) == htons(ETH_P_RARP) ? "Reverse Address Resolution Protocol (RARP)" : \
    (protocol) == htons(ETH_P_8021Q) ? "802.1Q VLAN Extended Header" : \
    (protocol) == htons(ETH_P_IPV6) ? "IPv6 over blueblook" : \
    (protocol) == htons(ETH_P_SLOW) ? "Ethernet slow protocols" : \
    (protocol) == htons(ETH_P_AOE) ? "ATA over Ethernet" : \
    (protocol) == htons(ETH_P_LLDP) ? "Link Layer Discovery Protocol" : \
    (protocol) == htons(ETH_P_FCOE) ? "Fibre Channel over Ethernet" : \
    (protocol) == htons(ETH_P_FIP) ? "FCoE Initialization Protocol" : \
    "Unknown Protocol")

# define ARP_OPCODE_TO_STRING(opcode) (\
    (opcode) == htons(ARPOP_REQUEST) ? "ARP Request" : \
    (opcode) == htons(ARP_REPLY) ? "ARP Reply" : \
    (opcode) == htons(RARP_REQUEST) ? "RARP Request" : \
    (opcode) == htons(RARP_REPLY) ? "RARP Reply" : \
    (opcode) == htons(DRARP_REQUEST) ? "DRARP Request" : \
    (opcode) == htons(DRARP_REPLY) ? "DRARP Reply" : \
    (opcode) == htons(DRARP_ERROR) ? "DRARP Error" : \
    (opcode) == htons(INARP_REQUEST) ? "InARP Request" : \
    (opcode) == htons(INARP_REPLY) ? "InARP Reply" : \
    "Unknown Opcode")

# define ARP_HRD_TO_STRING(hrd) (\
    (hrd) == htons(ARPHRD_ETHER) ? "Ethernet" : \
    (hrd) == htons(ARPHRD_IEEE802) ? "IEEE 802" : \
    "Unknown Hardware Type")

# define ARP_PRO_TO_STRING(pro) (\
    (pro) == htons(ETH_P_IP) ? "IPv4" : \
    (pro) == htons(ETH_P_ARP) ? "ARP" : \
    "Unknown Protocol Type")

/* from header if_arp.h for ethernet */
struct arppckt {
	uint16_t			ar_hrd;		/* format of hardware address	*/
	uint16_t			ar_pro;		/* format of protocol address	*/
	unsigned char		ar_hln;		/* length of hardware address	*/
	unsigned char		ar_pln;		/* length of protocol address	*/
	uint16_t			ar_op;		/* ARP opcode (command)		*/
	 /*
	  *	 Ethernet looks like this : This bit is variable sized however...
	  */
	unsigned char		ar_sha[ETH_ALEN];	/* sender hardware address	*/
	unsigned char		ar_sip[IPV4_LENGTH];		/* sender IP address		*/
	unsigned char		ar_tha[ETH_ALEN];	/* target hardware address	*/
	unsigned char		ar_tip[IPV4_LENGTH];		/* target IP address		*/
};

typedef struct s_malcolm
{
	int					sock_fd;
	unsigned char		spoofed_mac[ETH_ALEN];
	unsigned char		spoofed_ip[IPV4_LENGTH];
	unsigned char		target_mac[ETH_ALEN];
	unsigned char		target_ip[IPV4_LENGTH];
	unsigned char		initial_target_ip[IPV4_LENGTH];
	unsigned char		broadcast_addr[ETH_ALEN];
	struct sockaddr_ll	sockaddr_target; // sockaddr_ll structure is used to specify on which interface to send the packet and which protocol to set in the Ethernet header (EtherType field).
	unsigned int		ifindex;
	char				ifname[IFNAMSIZ];
	int					verbose;
	int					gratutious;
	int					hostname;
}			t_malcolm;

extern int	g_exit_code;

int			is_mac_address(const char *mac);
int			parse_input(int argc, char **args, t_malcolm  *data);
int			init_socket(t_malcolm  *data);
int			convert_mac_address_to_bytes(char *macAddr, unsigned char *mac_dst_addr);
int			ft_malcolm(t_malcolm *input);
int			get_interface(t_malcolm  *data);
int			process_packet(unsigned char *buffer, t_malcolm *data);
int			listen_to_broadcast(t_malcolm *data);
int			send_reply(t_malcolm *data);
uint32_t	calculate_crc32(const unsigned char *data, size_t length);
void		init_sockaddr_dest(t_malcolm  *data);
void		create_reply_packet(unsigned char *packet, t_malcolm *data);
void		resolve_hostname_from_ip(char *dest, unsigned char *ip_addr);
void		log_mac_address(char *msg, unsigned char *mac);
void		log_ip_address(char *msg, unsigned char *ip);
void		log_error(const char *msg);
void		log_msg(char *msg, char *str);
void		log_packet(unsigned char *packet);


#endif