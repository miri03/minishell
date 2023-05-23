/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_proc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:32:33 by meharit           #+#    #+#             */
/*   Updated: 2023/05/23 16:29:41 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    first_cmd(char *cmd, t_cmd *table, t_env *env, int **pipes)
{
	char    *cmd_path;

    close(pipes[1][0]);
    close(pipes[1][1]);
    close(pipes[0][0]);
	cmd_path = cmd_exist(table, env);
	redir_in(table, cmd_path);
	redir_out(table, cmd_path); /// ?
	dup2(pipes[0][1], 1);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		g_exit_status = 127;
		exit (g_exit_status);
	}     //if no infile or outfile
	execve(cmd_path, table->cmd, find_path(env));
	ft_putstr_fd("problem exec\n", 2);
}

void    last_cmd(char *cmd, t_cmd *table, t_env *env, int **pipes, int len, int j)
{
    char    *cmd_path;

    close(pipes[0][1]);
    cmd_path = cmd_exist(table, env);
    redir_in(table, cmd_path); /// ?
	redir_out(table, cmd_path);

    if (len == 2)
    {
        close(pipes[1][0]);
        close(pipes[1][1]);
        dup2(pipes[0][0], 0);
    }
    if (j % 2 == 0)
    {
        dup2(pipes[1][0], 0);
        close(pipes[0][0]);
        close(pipes[0][1]);
        close(pipes[1][1]);
    }
    if (j % 2 != 0)
    {
        dup2(pipes[0][0], 0);
        close(pipes[0][0]);
        close(pipes[0][1]);
        close(pipes[1][1]);
    }
        
    if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		g_exit_status = 127;
		exit (g_exit_status);
	}     //if no infile or outfile
	execve(cmd_path, table->cmd, find_path(env));
	ft_putstr_fd("problem exec\n", 2);
}

void    cmds(char *cmd, t_cmd *table, t_env *env, int **pipes, int j)
{
     char    *cmd_path;

    cmd_path = cmd_exist(table, env);
    redir_in(table, cmd_path); /// ?
	redir_out(table, cmd_path);
    if (j % 2 == 0)
    {
        // printf("J[2]===%d\n", j);
        dup2(pipes[1][0], 0);
        dup2(pipes[0][1], 1); ///

        close(pipes[1][1]);
        close(pipes[0][0]);
    }
    else
    {
        // printf("J[1]====%d\n", j);
        dup2(pipes[0][0], 0);
        dup2(pipes[1][1], 1);

        close(pipes[0][1]);
        close(pipes[1][0]);
    }
    
    if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		g_exit_status = 127;
		exit (g_exit_status);
	}     //if no infile or outfile
	execve(cmd_path, table->cmd, find_path(env));
	ft_putstr_fd("problem exec\n", 2);
}