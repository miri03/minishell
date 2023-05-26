/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 17:47:29 by meharit           #+#    #+#             */
/*   Updated: 2023/05/26 01:13:26 by meharit          ###   ########.fr       */
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
	int	dup_out;

	exec.built_in = 1;
	redir_in(table, NULL);
	dup_out = redir_out(table, NULL);
	which_builtin(cmd, table, env);
	dup2(exec.std_in, STDIN_FILENO);
	dup2(exec.std_out, STDOUT_FILENO);
	exec.built_in = 0;
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