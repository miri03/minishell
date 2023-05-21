/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:45:07 by meharit           #+#    #+#             */
/*   Updated: 2023/05/21 16:45:08 by meharit          ###   ########.fr       */
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
        printf("J[2]===%d\n", j);
        dup2(pipes[1][0], 0);
        dup2(pipes[0][1], 1); ///

        close(pipes[1][1]);
        close(pipes[0][0]);
    }
    else
    {
        printf("J[1]====%d\n", j);
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
    dprintf(2,"here\n");
	execve(cmd_path, table->cmd, find_path(env));
	ft_putstr_fd("problem exec\n", 2);
}

void    make_pipes(int **pipes)
{
    int i;

    i = 0;
    while (i < 2)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) == -1)
			perror("pipe");
        dprintf(2, "in[%d][0] = %d out[%d][1] %d\n", i, pipes[i][0], i, pipes[i][1]);
		i++;
	}
}

void    pipes(t_env *env, t_cmd *table)
{
	int **pipes;
	int *f_pid;
	int i;
	int j;
	int	status;
    int tbl_len;

	j = 0;
	i = 0;
    tbl_len = table_len(table);
	pipes = (int **)malloc(sizeof(int *) * 2);
	f_pid = (int *)malloc(sizeof(int) * tbl_len);  //if builtin ??
    make_pipes(pipes);
	while (table)
	{
		if (!is_builtin(table->cmd[0]))
		{
			f_pid[i] = fork();
            if (f_pid[i])
                dprintf(2, "%d==>%d\n", i, f_pid[i]);
            if (j == 0 && !f_pid[i])
                first_cmd(table->cmd[0], table, env, pipes);

            if (j < tbl_len - 1 && !f_pid[i])
                cmds(table->cmd[0], table, env, pipes, j);

            if (j == tbl_len - 1 && !f_pid[i])
                last_cmd(table->cmd[0], table, env, pipes, tbl_len, j);
            
            if (j == tbl_len - 1)
            {
                close(pipes[1][0]);
                close(pipes[1][1]);
                close(pipes[0][1]);
                close(pipes[0][0]);
            }

//// cat << del | head -n 3 | cat | wc -l        
			i++;
		}
		j++;
		
		table = table->next;
	}
	wait_all(f_pid, &status, i, pipes);
}

void wait_all(int *pid, int *status, int last, int **pipes)
{
    int i;
    (void)status;
    (void)pid;

    i = 0;
    close(pipes[1][0]);
    close(pipes[1][1]);
    close(pipes[0][1]);
    close(pipes[0][0]);
    while (i < last)
    {
        waitpid(-1, NULL, 0);
        i++;
    }
}
