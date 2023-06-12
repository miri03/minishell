/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:02:22 by meharit           #+#    #+#             */
/*   Updated: 2023/06/12 17:35:47 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**find_path(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, "PATH") == 0)
			return (ft_split(env->value, ':'));
		env = env->next;
	}
	return (NULL);
}

void	directory_error(char *dir)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(dir, 2);
	ft_putstr_fd(": is a directory\n", 2);
	exec.g_exit_status = 126;
	exit (exec.g_exit_status);
}

char	*cmd_exist(t_cmd *table, t_env *env)
{
	//!ft_strcmp(&table->cmd[0][ft_strlen(table->cmd[0]) - 1], "\\") && 
	
	char	**path;
	char	*cmd;
	char	*test;
	int		i;
	DIR		*ptr_dir;

	i = 0;
	if (!table->cmd)
		return (NULL);
	ptr_dir = opendir(table->cmd[0]);
	if (!ft_strcmp(&table->cmd[0][ft_strlen(table->cmd[0]) - 1], "\\") && ptr_dir)
	{
		directory_error(table->cmd[0]);
		// ft_putstr_fd("minishell: ", 2);
		// ft_putstr_fd(table->cmd[0], 2);
		// ft_putstr_fd(": is a directory\n", 2);
		// exec.g_exit_status = 126;
		// exit (exec.g_exit_status);
	}
	if (table->cmd[0][0] == '/' || (table->cmd[0][0] == '.' && table->cmd[0][1] == '/'))
	{
		if (access(table->cmd[0], F_OK) == 0) //does exist
			return (table->cmd[0]);	
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(table->cmd[0], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exec.g_exit_status = 127;
			exit (exec.g_exit_status);
		}
	}
	if (table->cmd[0][0])
	{
		cmd = ft_my_strjoin("/", table->cmd[0]);
		path = find_path(env);
		if (path)
		{
			while (path[i])
			{
				test = ft_strjoin(path[i], cmd);
				if (access(test, F_OK) == 0)
					return (test);
				i++;
			}
		}
	}
	return (NULL);
}

void	exec_single(t_env **env, t_cmd *table)
{
	char	*cmd_path;
	int		f_pid;
	int		status;

	if (table->cmd && is_builtin(table->cmd[0]))
		exec_builtin(table->cmd[0], table, env);
	else
	{
		f_pid = fork();
		if (!f_pid) // child
		{
			set_default();
			cmd_path = cmd_exist(table, *env);
			redir_in(table, 0);
			redir_out(table);
			if (table->cmd)
			{
				if (!cmd_path || !table->cmd[0][0]) //table->cmd
				{ 
					if (opendir(table->cmd[0]))
						directory_error(table->cmd[0]);
					dprintf(2, "yes\n");
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(table->cmd[0], 2);
					ft_putstr_fd(": command not found\n", 2);
					exec.g_exit_status = 127;
					exit(exec.g_exit_status);
				}
				if (execve(cmd_path, table->cmd, exec.env) == -1)
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(cmd_path, 2);
					perror(" ");
					exec.g_exit_status = 126;
					exit (exec.g_exit_status);
				}
			}
			else
				exit(0);
		}
		else //parent
		{
			if (exec.n_herdoc)
			{
				close(exec.herdoc_pipe[0][0]);
				close(exec.herdoc_pipe[0][1]);
				free (exec.herdoc_pipe[0]);
			}	
			waitpid(f_pid, &status, 0);
			if (WIFSIGNALED(status))
			{
				exec.g_exit_status = WTERMSIG(status) + 128;
				return;
			}
			exec.g_exit_status = WEXITSTATUS(status);
		}
	}
}

int	table_len(t_cmd *table)
{
	int	len;

	len = 0;
	while (table)
	{
		table = table->next;
		len++;
	}
	return (len);
}

t_exec	init_exec()
{
	t_exec	exec;
	
	exec.g_exit_status = 0;
	return (exec);
}

void	execute(t_cmd *table, t_env **dup_env)
{
	int		i;
	
	i = 0;
	if (!table)
		return;
	if (open_herdoc(table, *dup_env))
		return ;
	if (table_len(table) == 1)
		exec_single(dup_env, table);
	else
		multi_cmd(*dup_env, table);
}
