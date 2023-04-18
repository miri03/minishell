/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 18:33:29 by meharit           #+#    #+#             */
/*   Updated: 2023/04/18 04:50:09 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    ft_pwd(t_cmd *cmd)
{
    char *ptr;

	if (cmd_len(cmd->cmd) == 1)
	{
		ptr = getcwd(NULL, 0);
		printf("%s\n", ptr);
	}
	else
		ft_putstr_fd("pwd: too many arguments\n", 2);
}
