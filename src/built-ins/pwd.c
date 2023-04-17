#include "../../includes/minishell.h"

void    ft_pwd(void)
{
    char *ptr;

    ptr = getcwd(NULL, 0);
    printf("%s\n", ptr);
}
