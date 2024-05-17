#include "ft_malcolm.h"


int	is_ipv4(const char *ip)
{
	struct	in_addr	ipv4_addr;
	return	(inet_aton(ip, &ipv4_addr));
}

int	is_mac_address(const char *mac)
{
	int		i;
	int		counter;

	if (ft_strlen(mac) != MAC_ADDRESS_MAX_LEN)
	{
		return (0);
	}

	i = 0;
	counter = 0;
	while (mac[i])
	{
		if (counter <= 1)
		{
			if (!ft_isxdigit(mac[i]))
			{
				return (0);
			}
			counter++;
		} 
		else
		{
			if ((mac[i] != ':') && (mac[i] != '-'))
			{
				return (0);
			}
			counter = 0;
		}
		i++;
	}
	return (1);
}

int		convert_hex_to_int(char hex)
{
	if (hex >= '0' && hex <= '9')
	{
		return (hex - '0');
	}
	else if (hex >= 'A' && hex <= 'F')
	{
		return (hex - 'A' + 10);
    }
	else if (hex >= 'a' && hex <= 'f')
	{
        return (hex - 'a' + 10);
    }
	else
	{
        return (-1);
    }
}

int	convert_mac_address_to_bytes(char *macAddr, unsigned char *mac_dst_addr)
{
	size_t	idx = 0;
	int		idx_mac = 0;
	int		c1;
	int		c2;
	char	byte;
	unsigned char	mac_addr[MAC_LENGTH];

	while (idx < ft_strlen(macAddr) && idx_mac < MAC_LENGTH)
	{
        c1 = convert_hex_to_int(macAddr[idx]);
		c2 = convert_hex_to_int(macAddr[idx + 1]);
		if (c1 < 0 || c2 < 0)
		{
			// error
			return (0);
		}
 		byte = (c1 << 4) | c2;
		mac_addr[idx_mac] = byte;
		idx_mac++;
		idx += 3;
	}

	while (idx_mac < MAC_LENGTH)
	{
		mac_addr[idx_mac] = 0;
		idx_mac++;
	}
	ft_memcpy(mac_dst_addr, mac_addr, MAC_LENGTH);
	return (1);
}

void	printMACaddress(char *msg, unsigned char *mac)
{
	fprintf(stdout, "%s MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n", msg,
                        mac[0], mac[1], mac[2],
                        mac[3], mac[4], mac[5]);
}