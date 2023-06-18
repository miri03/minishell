/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 23:26:44 by meharit           #+#    #+#             */
/*   Updated: 2023/06/18 19:29:00 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_if_option(char *str, int *opt)
{
	int	i;

	i = 1;
	if (!str)
		return (0);
	if (str[0] == '-' && str[i] == 'n')
	{
		while (str[i])
		{
			if (str[i] != 'n')
				return (0);
			i++;
		}
		*opt = 1;
		return (1);
	}
	return (0);
}

int	n_option(t_cmd *cmd, int *opt)
{
	int	c;

	c = 1;
	while (cmd->cmd[c])
	{
		if (check_if_option(cmd->cmd[c], opt))
			c++;
		else
			break ;
	}
	return (c);
}

void	ft_echo(t_cmd *cmd, int fork)
{
	int	opt;
	int	i;
	int	c;

	opt = 0;
	i = 1;
	c = n_option(cmd, &opt);
	if (opt)
		i += (c - 1);
	while (cmd->cmd[i])
	{
		printf("%s", cmd->cmd[i]);
		if (i < cmd_len(cmd->cmd) - 1)
			printf(" ");
		i++;
	}
	if (!opt)
		printf("\n");
	g_exec.g_exit_status = 0;
	if (fork)
		exit(g_exec.g_exit_status);
}
