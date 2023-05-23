/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 17:47:29 by meharit           #+#    #+#             */
/*   Updated: 2023/05/23 18:47:17 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd , "exit") || !ft_strcmp(cmd , "env") || !ft_strcmp(cmd , "pwd") || 
		!ft_strcmp(cmd , "unset") ||!ft_strcmp(cmd , "cd") || !ft_strcmp(cmd , "echo") || !ft_strcmp(cmd , "export")) //
		return (1);
	return (0);
}

void	exec_builtin(char *cmd, t_cmd *table, t_env **env)
{
	int	in;
	int	out;
	int	dup_in;
	int	dup_out;


	in = dup(0);
	out = dup(1);
	dup_in = redir_in(table, NULL);
	dup_out = redir_out(table, NULL);
	which_builtin(cmd, table, env);
	dup2(in, 0);
	dup2(out, 1);
	close(dup_in);
	close(dup_out);
	close(in);
	close(out);
}

void	which_builtin(char *cmd, t_cmd *table, t_env **env)
{
	if (!ft_strcmp(cmd , "exit"))
	{
		ft_exit(table);
		return ;
	}
	if (!ft_strcmp(cmd , "env"))
	{
		ft_env(env);
		return ;
	}
	if (!ft_strcmp(cmd , "pwd"))
	{
		ft_pwd();
		return ;
	}
	if (!ft_strcmp(cmd , "unset"))
	{
		ft_unset(env, table);
		return ;
	}
	if (!ft_strcmp(cmd , "cd"))
	{
		ft_cd(table, env);
		return ;
	}
	if (!ft_strcmp(cmd , "echo"))
	{
		ft_echo(table);
		return ;
	}
	if (!ft_strcmp(cmd , "export"))
	{
		ft_export(*env, table);
		return ;
	}
}