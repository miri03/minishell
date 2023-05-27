/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 20:50:24 by meharit           #+#    #+#             */
/*   Updated: 2023/05/27 23:40:21 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	arg_num(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int	cmd_len(char **table)
{
	int	len;

	len = 0;
	while (table[len])
		len++;
	return (len);
}

void	ft_exit(t_cmd *cmd) // handle overflow
{
	ft_putstr_fd("exit\n", 2);
	if (cmd->cmd[1])
	{
		if (!arg_num(cmd->cmd[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(cmd->cmd[1],2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exec.g_exit_status = 255;
			exit(exec.g_exit_status);
		}
		else if (cmd_len(cmd->cmd) > 2)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			exec.g_exit_status = 1;
			return ;
		}
		else if (cmd_len(cmd->cmd) == 2)
			exit (ft_atoi(cmd->cmd[1]));
	}
	exit (0);
}
