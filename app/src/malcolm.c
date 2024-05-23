#include "ft_malcolm.h"


static int    init_malcolm(t_malcolm  *data)
{
    int ret = 0;

    ret = get_interface(data);
    if (ret)
    {
        log_error("failed to get network interface");     
        return (ret);
    }
    ret = init_socket(data);
    if (ret)
    {      
        log_error("failed to create a socket");      
        return (ret);
    }
    init_sockaddr_dest(data);
    ft_memset(data->broadcast_mac, 0xFF, ETH_ALEN);
    return (ret);
}

int ft_malcolm(t_malcolm *input)
{
    int ret = 0;

    ret = init_malcolm(input);
    if (ret)
    {
        close(input->sock_fd);
        return (ret);
    }
    if (!input->gratutious) 
    {
        ret = listen_to_broadcast(input);
        if (ret)
        {
            close(input->sock_fd);
            return (ret);
        }
    }
    return (send_reply(input));
}