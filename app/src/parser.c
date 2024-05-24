#include "ft_malcolm.h"

char *parse_mac_address(char *addr)
{
	if (is_mac_address(addr))
		return (NULL);
	return addr;
}

static int parse_options(int argc, char **args, t_malcolm  *data)
{
	for (int i = 5; i < argc; i++)
	{
		if (!data->verbose && !ft_strcmp(VERBOSE, args[i]))
		{
			data->verbose = 1;
			continue ;
		}
		if (!data->gratutious && !ft_strcmp(GRATUTIOUS, args[i]))
		{
			data->gratutious = 1;
			continue ;
		}
		if (!data->hostname && !ft_strcmp(HOSTNAME, args[i]))
		{
			data->hostname = 1;
			continue ;
		}
		log_error(USAGE_INFO);
		return (1);
	}
	return (0);
}

static int parse_address(char **args, t_malcolm  *data)
{
   	if (!inet_pton(AF_INET, args[1], data->spoofed_ip) || \
		!inet_pton(AF_INET, args[3], data->target_ip))
	{
		log_error(strerror(errno));
		return (1);
	}
	char *mac_source = parse_mac_address(args[2]);
    char *mac_target = parse_mac_address(args[4]);
	if ((!mac_source) || (!mac_target))
	{
		log_error("invalid mac address");
		return (1);
	}

	if(convert_mac_address_to_bytes(mac_source, data->spoofed_mac) || \
		convert_mac_address_to_bytes(mac_target, data->target_mac))
	{
		log_error("failed to convert mac address");
		return (1);
	}
	return (0);
}

int	parse_input(int argc, char **args, t_malcolm  *data)
{
	if (parse_address(args, data))
	{
        return (1);
	}
    if (parse_options(argc, args, data))
	{
        return (1);
	}
	return (0);
}