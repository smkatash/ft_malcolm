#include "ft_malcolm.h"


int main(int argc, char** argv)
{
    t_machine   input;
    if (argc != 5)
    {
        fprintf(stderr, USAGE_INFO);
        return (EXIT_FAILURE);
    }
    ft_memset(&input, 0, sizeof(t_machine));
    if (!parse_input(argv, &input))
    {
        return (EXIT_FAILURE);
    }
    return (ft_malcolm(&input));
}

// ./ft_malcolm 192.168.112.3 02:42:c0:a8:70:03 192.168.112.2 02:42:c0:a8:70:02
