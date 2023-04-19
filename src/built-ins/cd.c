/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 21:41:39 by meharit           #+#    #+#             */
/*   Updated: 2023/04/18 23:21:48 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_cd(t_cmd *cmd)
{
	if (cmd_len(cmd->cmd) == 1)
		chdir("/Users/meharit/");
	else if (chdir(cmd->cmd[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(cmd->cmd[1], 2);
		perror(" ");
	}
}
