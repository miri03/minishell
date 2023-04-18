/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 21:42:54 by meharit           #+#    #+#             */
/*   Updated: 2023/04/18 05:22:01 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void make_second_node_head(t_env **head_ptr)
{
    t_env *second_node;

    if (*head_ptr == NULL || (*head_ptr)->next == NULL)
        return;
    second_node = (*head_ptr)->next;
    (*head_ptr)->next = second_node->next;
    *head_ptr = second_node;
}

void    unset_var(t_env *env, int index, t_env **head)
{
    t_env   *new;
    t_env   *tmp;
    int     i;
    (void) head;

    i = 0;
    
    while (env)
    {
        if (index == 0)
        {
            make_second_node_head(head);
            break ;
        }

        else if (i == index - 1)
        {
            new = (env->next)->next;
            tmp = env->next;
            free (tmp->key);
            free (tmp->value);
            env->next = new;
            break;
        }
        i++;
        env = env->next;
    }
}

void	ft_unset(t_env **dup_env, t_cmd *cmd)
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
            tmp = *dup_env;
            while (tmp)
            {
                if (ft_strcmp(tmp->key, cmd->cmd[i]) == 0)
                {
                    unset_var(*dup_env, index, dup_env);
                    break ;
                }
                index++;
                tmp = tmp->next;
            }
            i++;
        }
    }
}
