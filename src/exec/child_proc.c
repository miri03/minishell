/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_proc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:32:33 by meharit           #+#    #+#             */
/*   Updated: 2023/05/27 23:40:21 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// ls | cat | grep ".c" | wc -l

void    dup_it(int phase, int i)
{
    if (phase == 0)  //first cmd
    {
        dup2(exec.pipes[0][1], 1);
    }

    if (i % 2 == 0)
    {
        if (phase == 1) // middle cmd
        {
            dup2(exec.pipes[0][1], 1); // i % 2 == 0
            dup2(exec.pipes[1][0], 0);
        }
        if (phase == 2) //last cmd
        {
            dup2(exec.pipes[1][0], 0);
        }
    }

    else
    {
        if (phase == 1) //middle cmd
        {
            dup2(exec.pipes[1][1], 1); // i % 2 != 0
            dup2(exec.pipes[0][0], 0);
        }
        
        if (phase == 2) //last cmd
        {
            dup2(exec.pipes[0][0], 0);
        }
    }
    close(exec.pipes[1][1]);
    close(exec.pipes[0][0]);
    close(exec.pipes[0][1]);
    close(exec.pipes[1][0]);
}

void    execute_cmds(t_cmd *table, t_env *env, int phase, int i)
{
    char    *cmd_path;
    char    *cmd;

    cmd_path = cmd_exist(table, env);
    dup_it(phase, i);
    redir_in(table, i);
    redir_out(table);
    if (table->cmd) // there is a commad
    {
        cmd = table->cmd[0];
        if (is_builtin(cmd))
            which_builtin(cmd, table, &env, TRUE); // break; ??
        if (!cmd_path)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd, 2);
            ft_putstr_fd(": command not found\n", 2);
            exec.g_exit_status = 127;
            exit (exec.g_exit_status);
        }
        dprintf(2, "--------->execve\n");
        execve(cmd_path, table->cmd, exec.env);
        dprintf(2, "!!!!!exec prob!!!!\n");
    }
    exit (exec.g_exit_status);
}

// void    first_cmd(char *cmd, t_cmd *table, t_env *env, int **pipes)
// {
// 	char    *cmd_path;
//     int     built_in;

//     built_in = 0;
//     if (is_builtin(cmd))
//         built_in = 1;

//     close(pipes[1][0]);
//     close(pipes[1][1]);
//     close(pipes[0][0]);

// 	cmd_path = cmd_exist(table, env);
//     if (built_in)
//         cmd_path = NULL;
// 	redir_in(table, cmd_path);
// 	redir_out(table, cmd_path); /// ?
// 	dup2(pipes[0][1], 1);
// 	if (!cmd_path && !built_in)
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(cmd, 2);
// 		ft_putstr_fd(": command not found\n", 2);
// 		exec.g_exit_status = 127;
// 		exit (exec.g_exit_status);
// 	}     //if no infile or outfile
//     if (built_in)
//     {
//         which_builtin(cmd, table, &env);
//         // dprintf(2, "stat = %d\n", exec.g_exit_status);
//         return;
//     }
// 	execve(cmd_path, table->cmd, find_path(env));
// 	ft_putstr_fd("problem exec\n", 2);
// }

// void    cmds(char *cmd, t_cmd *table, t_env *env, int **pipes, int j)
// {
//     char    *cmd_path;
//     int     built_in;

//     built_in = 0;
//     if (is_builtin(cmd))
//         built_in = 1;

//     cmd_path = cmd_exist(table, env);
//     if (built_in)
//         cmd_path = NULL;
//     redir_in(table, cmd_path); /// ?
// 	redir_out(table, cmd_path);
//     if (j % 2 == 0)
//     {
//         // printf("J[2]===%d\n", j);
//         dup2(pipes[1][0], 0);
//         dup2(pipes[0][1], 1); ///

//         close(pipes[1][1]);
//         close(pipes[0][0]);
//     }
//     else
//     {
//         // printf("J[1]====%d\n", j);
//         dup2(pipes[0][0], 0);
//         dup2(pipes[1][1], 1);

//         close(pipes[0][1]);
//         close(pipes[1][0]);
//     }
    
//     if (!cmd_path && !built_in)
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(cmd, 2);
// 		ft_putstr_fd(": command not found\n", 2);
// 		exec.g_exit_status = 127;
// 		exit (exec.g_exit_status);
// 	}     //if no infile or outfile
//     if (built_in)
//     {
//         which_builtin(cmd, table, &env);
//         dprintf(2, "stat = %d\n", exec.g_exit_status);
//         return;
//     }
// 	execve(cmd_path, table->cmd, find_path(env));
// 	ft_putstr_fd("problem exec\n", 2);
// }

// void    last_cmd(char *cmd, t_cmd *table, t_env *env, int **pipes, int len, int j)
// {
//     char    *cmd_path;
//     int     built_in;

//     built_in = 0;
//     if (is_builtin(cmd))
//         built_in = 1;
    
//     close(pipes[0][1]);
//     cmd_path = cmd_exist(table, env);
//     if (built_in)
//         cmd_path = NULL;
//     redir_in(table, cmd_path); /// ?
// 	redir_out(table, cmd_path);

//     if (len == 2)
//     {
//         close(pipes[1][0]);
//         close(pipes[1][1]);
//         dup2(pipes[0][0], 0);
//     }
//     if (j % 2 == 0)
//     {
//         dup2(pipes[1][0], 0);
//         close(pipes[0][0]);
//         close(pipes[0][1]);
//         close(pipes[1][1]);
//     }
//     if (j % 2 != 0)
//     {
//         dup2(pipes[0][0], 0);
//         close(pipes[0][0]);
//         close(pipes[0][1]);
//         close(pipes[1][1]);
//     }
        
//     if (!cmd_path && !built_in)
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(cmd, 2);
// 		ft_putstr_fd(": command not found\n", 2);
// 		exec.g_exit_status = 127;
// 		exit (exec.g_exit_status);
// 	}     //if no infile or outfile
//     if (built_in)
//     {
//         which_builtin(cmd, table, &env);
//         dprintf(2, "stat = %d\n", exec.g_exit_status);
//         return;
//     }
//     // while(1);
// 	execve(cmd_path, table->cmd, find_path(env));
// 	ft_putstr_fd("problem exec\n", 2);
// }
