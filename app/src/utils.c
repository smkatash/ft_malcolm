#include "ft_malcolm.h"

int	is_mac_address(const char *mac)
{
	int		i = 0;
	int		counter = 0;

	if (ft_strlen(mac) != MAC_ADDRESS_MAX_LEN - 1)
		return (1);

	while (mac[i])
	{
		if (counter <= 1)
		{
			if (!ft_isxdigit(mac[i]))
				return (1);
			counter++;
		} 
		else
		{
			if ((mac[i] != ':') && (mac[i] != '-'))
				return (1);
			counter = 0;
		}
		i++;
	}
	return (0);
}

int		convert_hex_to_int(char hex)
{
	if (hex >= '0' && hex <= '9')
		return (hex - '0');
	else if (hex >= 'A' && hex <= 'F')
		return (hex - 'A' + 10);
	else if (hex >= 'a' && hex <= 'f')
        return (hex - 'a' + 10);
	else
        return (-1);
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
			return (1);
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
	return (0);
}

// CRC32 lookup table
void generate_crc32_table(uint32_t table[256]) {
    uint32_t polynomial = 0xEDB88320;
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (uint32_t j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ polynomial;
            else
                crc = crc >> 1;
        }
        table[i] = crc;
    }
}

//  CRC32 checksum
uint32_t calculate_crc32(const unsigned char *data, size_t length) {
    uint32_t crc32_table[256];
    generate_crc32_table(crc32_table);
    
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; i++) {
        uint8_t byte = data[i];
        uint32_t lookup_index = (crc ^ byte) & 0xFF;
        crc = (crc >> 8) ^ crc32_table[lookup_index];
    }
    return crc ^ 0xFFFFFFFF;
}