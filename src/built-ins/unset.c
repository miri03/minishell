/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 21:42:54 by meharit           #+#    #+#             */
/*   Updated: 2023/04/15 21:46:23 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    unset_var(t_env *env, int index)
{
    t_env   *next;
    t_env   *tmp;
    t_env   *tmp_;
    int     i;

    i = 0;
    
    while (env)
    {
        if (index == 0)
        {
            tmp = env;
            tmp_ = env->next;
            free(tmp->key);
            free(tmp->value);
            free(tmp);
            env = tmp_;
            // next = (*head)->next;
            // *head = next;
            printf("yes\n");
            break ;
        }

        else if (i == index - 1)
        {
            next = (env->next)->next;
            tmp = env->next;
            free (tmp->key);
            free (tmp->value);
            env->next = next;
            break;
        }
        printf("no\n");
        i++;
        env = env->next;
    }
}

void	ft_unset(t_env *dup_env, t_cmd *cmd)
{
    int     i;
    int     index;
    t_env   *tmp;

    i = 1;
    if (cmd_len(cmd->cmd) >= 2)
    {
        while (cmd->cmd[i])
        {
            index = 0;
            tmp = dup_env;
            while (tmp)
            {
                if (ft_strcmp(tmp->key, cmd->cmd[i]) == 0)
                {
                    printf("INDEX= %d\n", index);
                    unset_var(dup_env, index);
                    break ;
                }
                index++;
                tmp = tmp->next;
            }
            i++;
        }
    }
}
