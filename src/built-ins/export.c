/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 22:18:29 by meharit           #+#    #+#             */
/*   Updated: 2023/05/01 13:31:00 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdio.h>
#include <strings.h>

/* int ft_lstsize_env(t_env *lst)
{
    int count;

    count = 0;
    while (lst)
    {
        lst = lst->next;
        count++;
    }
    return (count);
} */

int	valid_ident(char *ident)
{
	int	i;

	i = 1;
	if (!ft_isalpha(ident[0]))
	{
		if (ident[0] != '\\') //remove \ from ident
			return (0);
	}
	while (ident[i] != '=' && ident[i])
	{
		if (ident[i] != '+' && ident[i] != '=')
		{
			if (!ft_isalpha(ident[i]) && !ft_isdigit(ident[i]))
				return (0);
		}
		if (ident[i] == '+' && ident[i+1] != '=')
			return (0);
		i++;
	}
	return (1);
}

int	env_valid(char *ident)
{
	int	i;

	i = 0;
	while (ident[i])
	{
		if (ident[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

char	*get_key(char *ident)
{
	int	i;

	i = 0;
	while (ident[i] && ident[i] != '=')
		i++;
	return (ft_substr(ident, 0, i));
}

char	*get_value(char *ident)
{
	int i;

	i = 0;
	while (ident[i] && ident[i] != '=')
		i++;
	if (!ident[i])
		return (ft_strdup(""));
	return (ft_substr(ident, i+1, ft_strlen(ident))); //skip =
}
// append for +=

void	ft_export(t_env *dup_env, t_exp **export, t_cmd *table)
{
	(void) dup_env;
	(void) export;
	int	i;

	i = 1;

	// add dup_env->valid to env
	if (cmd_len(table->cmd) == 1)
	{
		while (dup_env)
		{
			printf("declare -x ");
			printf("%s=",dup_env->key);
			printf("\"%s\"\n", dup_env->value);
			dup_env = dup_env->next;
		}
	}
	else
	{
		while (table->cmd[i])
		{
			if (valid_ident(table->cmd[i]))
			{
				if (env_valid(table->cmd[i]))
					ft_lstadd_back_env(&dup_env, ft_lstnew_env(get_key(table->cmd[i]), get_value(table->cmd[i]), 1));
				else
					ft_lstadd_back_env(&dup_env, ft_lstnew_env(get_key(table->cmd[i]), get_value(table->cmd[i]), 0));
			}
			else 
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(table->cmd[i], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
			}
			i++;
		}
	}
}

