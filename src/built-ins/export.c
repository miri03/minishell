/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 22:18:29 by meharit           #+#    #+#             */
/*   Updated: 2023/04/19 22:36:41 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_lstsize_env(t_env *lst)
{
    int count;

    count = 0;
    while (lst)
    {
        lst = lst->next;
        count++;
    }
    return (count);
}

// void    export_env(t_env *env)
// {
//     char **tab;

//     tab = (char **)malloc(sizeof(char*) * ft_lstsize_env(env));
    
// }

void	ft_export(t_env *dup_env)
{

    printf("%d\n", ft_strcmp(dup_env->key,dup_env->next->key));
    // export_env(dup_env);
}

