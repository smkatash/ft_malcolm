#include "libft.h"


int ft_isxdigit(int c)
{
    if (ft_isdigit(c))
    {
        return (1);
    }
    return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}