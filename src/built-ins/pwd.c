/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 18:33:29 by meharit           #+#    #+#             */
/*   Updated: 2023/06/16 17:47:22 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(t_env *env, int fork)
{
	char	*ptr;

	ptr = getcwd(NULL, 0);
	printf("%p\n", ptr);
	if (!ptr)
	{
		free (ptr);
		ptr = get_pwd(env);
	}
	printf("%s\n", ptr);
	free(ptr);
	g_exec.g_exit_status = 0;
	if (fork)
		exit(g_exec.g_exit_status);
}
