#include "ft_malcolm.h"


static int    init_malcolm(t_machine  *devices)
{
    int ret;

    ret = get_interface(devices);
    if (!ret)
    {
        // Error        
        return (0);
    }
    ret = init_socket(devices);
    if (!ret)
    {
        // Error        
        return (0);
    }
    init_sockaddr_dest(devices);
    return (1);
}

int ft_malcolm(t_machine *input)
{
    int ret = 0;

    ret = init_malcolm(input);
    if (!ret)
    {
        return (ret);
    }
    ret = listen_to_broadcast(input);
    ret = send_reply(input);
    return (ret);
}