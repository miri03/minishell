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

int	check_herdoc(t_redi *in)
{
	int	fd;
	char	*line;

	fd = -1;
	while (in)
	{
		if (in->type == 0)
		{
			unlink(".herdoc");
			fd = open (".herdoc", O_CREAT | O_TRUNC | O_RDWR, 0644);
			while (1)
			{
				line = readline(">");
				if (!line || !ft_strcmp(line, in->file))
					break;
				write(fd, line, ft_strlen(line));
				write(fd, "\n", 1);
			}
			close (fd);
			fd = open(".herdoc", O_RDWR);
		}
		in = in->next;
	}
	return (fd);
}

int	redir_in(t_cmd *table, char *cmd_path, char *cmd)
{
	int		fd;
	int		fd_h;
	t_redi	*in;
	
	fd = -1;
	in = table->in;
	if (in)
	{
		fd_h = check_herdoc(in);
		while (in)
		{
			if (in->type)
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
			}
			if (!cmd_path && cmd)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd, 2);
				ft_putstr_fd(": command not found\n", 2);
				g_exit_status = 127;
				exit (g_exit_status);
			}
			in = in->next;
		}
		if (fd_h > 0)
			fd = fd_h;
		dup2(fd, 0);
		unlink(".herdoc");
	}
	return (fd);
}

int	redir_out(t_cmd *table, char *cmd_path, char *cmd)
{
	int		fd;
	t_redi  *out;

	fd = -1;
	out = table->out;
	if (out)
	{
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
			if (!cmd_path && cmd)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd, 2);
				ft_putstr_fd(": command not found\n", 2);
				g_exit_status = 127;
				exit (g_exit_status);
			}
			out = out->next;
		}
		dup2(fd, 1);
	}
	return (fd);
}

void	exec(char *cmd, t_env *env, t_cmd *table)
{
	char	*cmd_path;
	int		f_pid;
	int		status;

	if (is_builtin(cmd))
		exec_builtin(cmd, table, &env);
	else
	{
		f_pid = fork();
		if (!f_pid)
		{
			cmd_path = cmd_exist(cmd, env);
			redir_in(table, cmd_path, cmd);
			redir_out(table, cmd_path, cmd);
			if (!cmd_path)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd, 2);
				ft_putstr_fd(": command not found\n", 2);
				g_exit_status = 127;
				exit (g_exit_status);
			}
			execve(cmd_path, table->cmd, find_path(env));
			dprintf(2, "problem exec\n");
		}
		else
		{
			waitpid(f_pid, &status, 0);
			g_exit_status = WEXITSTATUS(status);
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

void	execute(t_cmd *table, t_env **dup_env)
{
	int		i;

	i = 0;
	if (table_len(table) == 1)
		exec(table->cmd[i], *dup_env, table);
	else
	{
		printf("pipes\n");
		// pipes(*dup_env, table);
	}
}
