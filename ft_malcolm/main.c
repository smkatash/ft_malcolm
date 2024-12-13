#include "ft_malcolm.h"

int	g_exit_code = 0;

static void	sighandler(int sig)
{
	if (sig == SIGINT)
		g_exit_code = 128 + sig;
}

int main(int argc, char** argv)
{
    t_malcolm   input;

    if (argc < 5 || argc > 8)
    {
        log_error(USAGE_INFO);
        return (EXIT_FAILURE);
    }
    if (signal(SIGINT, &sighandler) == SIG_ERR) {
        log_error("failed signal handler");
        return (EXIT_FAILURE);
    }
    ft_memset(&input, 0, sizeof(t_malcolm));
    if (parse_input(argc, argv, &input))
        return (EXIT_FAILURE);
    return (ft_malcolm(&input));
}


// ./ft_malcolm 192.168.0.5 02:42:c0:a8:00:05 192.168.0.6 02:42:c0:a8:00:06