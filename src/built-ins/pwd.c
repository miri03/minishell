/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 18:33:29 by meharit           #+#    #+#             */
/*   Updated: 2023/05/26 18:35:18 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(int fork)
{
    char *ptr;

	ptr = getcwd(NULL, 0);
	printf("%s\n", ptr);
	free(ptr);
	g_exit_status = 0;
	if (fork)
		exit (g_exit_status);
}
