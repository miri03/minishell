/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:02:22 by meharit           #+#    #+#             */
/*   Updated: 2023/05/11 21:37:46 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/history.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>

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

char	*cmd_exist(char *cmd, t_env *env)
{
	char	**path;
	char	*test;
	int		i;

	i = 0;
	if (access(cmd, F_OK) == 0) //does exist
		return (cmd);
	cmd = ft_my_strjoin("/", cmd);
	path = find_path(env);
	if (path)
	{
		while (path[i])
		{
			test = ft_strjoin(path[i],cmd);
			if (access(test, X_OK) == 0)
				return (test);
			i++;
		}
	}
	return (NULL);//
}

void	redir_in(t_cmd *table)
{
	int		fd;
	t_redi	*in;
	char	*line;
	
	in = table->in;
	if (in)
	{
		while (in)
		{
			if (in->type == 0)
			{
				fd = open(in->file, O_RDONLY);
				fd = open (".herdoc", O_CREAT | O_RDWR, 0644);
				while (1)
				{
					line = readline(">");
					if (!line || !ft_strcmp(line, in->file))
						break;
					write(fd, line, ft_strlen(line));
					write(fd, "\n", 1);
				}
				fd = open(".herdoc", O_RDWR);
				dup2(fd, 0);
				unlink(".herdoc");
			}
			else
			{
				fd = open(in->file, O_RDONLY);
				if (fd == -1)
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(in->file, 2);
					perror(" ");
					g_exit_status = 1;
					exit (g_exit_status);
				}
			// dup2(fd, 0); /// problem here
			}
			in = in->next;
		}
		dup2(fd, 0);
	}
}

void	redir_out(t_cmd *table)
{
	int		fd;
	t_redi  *out;

	out = table->out;
	while (out)
	{
		if (out->type == 3)
			fd = open(out->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(out->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(out->file, 2);
			perror(" ");
			g_exit_status = 1;
			exit (g_exit_status);
		}
		out = out->next;
		dup2(fd, 1); /// out of the loop?
	}
}

void	exec(char *cmd, t_env *env, t_cmd *table)
{
	char	*cmd_path;
	int		f_pid;
	int		status;
	
	f_pid = fork();
	if (!f_pid)
	{
		cmd_path = cmd_exist(cmd, env);
		if (!cmd_path)
		{
			printf("command not found\n"); //error mess && exit status
			g_exit_status = 127;
			return ;
		}
		redir_out(table);
		redir_in(table);
		execve(cmd_path, table->cmd, find_path(env));
		printf("error exec\n");	
	}
	else
	{
		waitpid(f_pid, &status, 0);
		g_exit_status = WEXITSTATUS(status);
	}
}

void	check_builin(char *cmd, t_cmd *table, t_env **env)
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
	exec(cmd, *env, table);
}

void	execute(t_cmd *cmd, t_env **dup_env)
{
	int		i;

	i = 0;
	while (cmd)
	{
		check_builin(cmd->cmd[i], cmd, dup_env);
		cmd = cmd->next;
		// i++;
	}
}
