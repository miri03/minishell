/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_uti.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 20:04:04 by meharit           #+#    #+#             */
/*   Updated: 2023/06/12 20:10:34 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_pwd(t_env *env)
{
	while (env)
	{
		if (ft_strcmp("PWD", env->key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*get_home(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, "HOME") == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
