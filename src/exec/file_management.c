/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 18:15:35 by meharit           #+#    #+#             */
/*   Updated: 2023/06/13 18:18:02 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	n_herdoc(t_redi *in)
{
	int	n;

	n = 0;
	while (in)
	{
		if (in->type == heredoc)
			n++;
		in = in->next;
	}
	g_exec.n_herdoc = n;
	return (n);
}

// recheck no env

void	make_herdoc_pipe(int herdo, int h)
{
	if (herdo)
	{
		g_exec.herdoc_pipe[h] = malloc(sizeof(t_exec) * 2);
		if (pipe(g_exec.herdoc_pipe[h]) == -1)
			perror("pipe\n");
	}
}

void	herdoc(int herdo, t_redi *tmp_in, t_env *env, int h)
{
	char	*line;

	while (1)
	{
		line = readline(">");
		if (!line || !ft_strcmp(line, tmp_in->file))
		{
			free(line);
			break ;
		}
		if (herdo == 1)
		{
			if (tmp_in->must_exp)
				expand_var(env, &line);
			write(g_exec.herdoc_pipe[h][1], line, ft_strlen(line));
			write(g_exec.herdoc_pipe[h][1], "\n", 1);
		}
		free(line);
	}
}

int	parent_herdoc(int p_id, int h)
{
	int	status;

	waitpid(p_id, &status, 0);
	if (WIFSIGNALED(status))
	{
		free(g_exec.herdoc_pipe[h]);
		g_exec.g_exit_status = WTERMSIG(status) + 128;
		return (1);
	}
	return (0);
}

int	is_herdoc(int herdo, int h, t_redi *tmp_in, t_env *env)
{
	int	p_id;

	p_id = fork();
	if (!p_id)
	{
		set_default();
		herdoc(herdo, tmp_in, env, h);
		if (!herdo)
			close(g_exec.herdoc_pipe[h][1]);
		exit(g_exec.g_exit_status);
	}
	else
	{
		herdo--;
		if (parent_herdoc(p_id, h))
			return (1);
	}
	return (0);
}

void	herdoc_child(int herdo, t_redi *tmp_in, t_env *env, int h)
{
	set_default();
	herdoc(herdo, tmp_in, env, h);
	if (!herdo)
		close(g_exec.herdoc_pipe[h][1]);
	exit(g_exec.g_exit_status);
}

int	herdoc_redir_in(t_redi *tmp_in, int h, t_env *env, int herdo)
{
	int	p_id;
	
	if (tmp_in->type == heredoc)
	{
		p_id = fork();
		if (!p_id)
			herdoc_child(herdo, tmp_in, env, h);
		else
		{
			herdo--;
			if (parent_herdoc(p_id, h))
				return (1);
		}
	}
	return (0);
}

int	open_herdoc(t_cmd *table, t_env *env)
{
	t_redi	*tmp_in;
	int		herdo;
	int		h;

	h = 0;
	while (table)
	{
		tmp_in = table->in;
		herdo = n_herdoc(tmp_in);
		make_herdoc_pipe(herdo, h);
		while (tmp_in)
		{
			if (herdoc_redir_in(tmp_in, h, env, herdo))
				return (1);
			tmp_in = tmp_in->next;
		}
		table = table->next;
		h++;
	}
	return (0);
}

void	is_herdoc_redir(int i)
{
	dup2(g_exec.herdoc_pipe[i][0], STDIN_FILENO);
	close(g_exec.herdoc_pipe[i][0]);
	close(g_exec.herdoc_pipe[i][1]);
}

void	redir_in(t_cmd *table, int i)
{
	int		fd;
	t_redi	*r_in;

	r_in = table->in;
	while (r_in)
	{
		if (r_in->type == in)
		{
			fd = open(r_in->file, O_RDONLY);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(r_in->file, 2);
				perror(" ");
				g_exec.g_exit_status = 1;
				if (g_exec.built_in == 0)
					exit(g_exec.g_exit_status);
				return ;
			}
			dup2(fd, 0);
		}
		else
			is_herdoc_redir(i);
		r_in = r_in->next;
	}
}

int	redir_out(t_cmd *table)
{
	int		fd;
	t_redi	*out;

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
				g_exec.g_exit_status = 1;
				exit(g_exec.g_exit_status);
			}
			out = out->next;
		}
		dup2(fd, 1);
	}
	return (fd);
}

// int	open_herdoc(t_cmd *table, t_env *env)
// {
// 	t_redi	*tmp_in;
// 	int		herdo;
// 	int		h;

// 	h = 0;
// 	while (table)
// 	{
// 		tmp_in = table->in;
// 		herdo = make_herdoc_pipe(h, tmp_in);
// 		while (tmp_in)
// 		{
// 			if (tmp_in->type == heredoc)
// 			{
// 				if (is_herdoc(herdo, h, tmp_in, env))
// 					return (1);
// 			}
// 			tmp_in = tmp_in->next;
// 		}
// 		table = table->next;
// 		h++;
// 	}
// 	return (0);
// }

// void	is_herdoc_redir(int i)
// {
// 	dup2(g_exec.herdoc_pipe[i][0], STDIN_FILENO);
// 	close(g_exec.herdoc_pipe[i][0]);
// 	close(g_exec.herdoc_pipe[i][1]);
// }

// void	redir_in(t_cmd *table, int i)
// {
// 	int		fd;
// 	t_redi	*r_in;

// 	r_in = table->in;
// 	while (r_in)
// 	{
// 		if (r_in->type == in)
// 		{
// 			fd = open(r_in->file, O_RDONLY);
// 			if (fd == -1)
// 			{
// 				ft_putstr_fd("minishell: ", 2);
// 				ft_putstr_fd(r_in->file, 2);
// 				perror(" ");
// 				g_exec.g_exit_status = 1;
// 				if (g_exec.built_in == 0)
// 					exit(g_exec.g_exit_status);
// 				return ;
// 			}
// 			dup2(fd, 0);
// 		}
// 		else
// 			is_herdoc_redir(i);
// 		r_in = r_in->next;
// 	}
// }

// void	error_open(char *file)
// {
// 	ft_putstr_fd("minishell: ", 2);
// 	ft_putstr_fd(file, 2);
// 	perror(" ");
// 	g_exec.g_exit_status = 1;
// 	exit(g_exec.g_exit_status);
// }

// int	redir_out(t_cmd *table)
// {
// 	int		fd;
// 	t_redi	*out;

// 	fd = -1;
// 	out = table->out;
// 	if (out)
// 	{
// 		while (out)
// 		{
// 			if (out->type == 3)
// 				fd = open(out->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 			else
// 				fd = open(out->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			if (fd == -1)
// 				error_open(out->file);
// 			out = out->next;
// 		}
// 		dup2(fd, 1);
// 	}
// 	return (fd);
// }
