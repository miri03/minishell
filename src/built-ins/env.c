/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 21:22:25 by meharit           #+#    #+#             */
/*   Updated: 2023/04/18 21:25:54 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

void	ft_env(t_env **dup_env)
{
	t_env *tmp;

	if (!(*dup_env))
	{
		// printf("env empty\n");
		ft_lstadd_back_env(dup_env, ft_lstnew_env("PWD", getcwd(NULL, 0)));
		ft_lstadd_back_env(dup_env, ft_lstnew_env("SHLVL", "1"));
		ft_lstadd_back_env(dup_env, ft_lstnew_env("_", "/usr/bin/env"));
		//add shell level and oldpwd check unset _
	}
	tmp = *dup_env;
	while (tmp)
	{
		printf("%s=%s\n", tmp->key,tmp->value);
		tmp = tmp->next;
	}
}

