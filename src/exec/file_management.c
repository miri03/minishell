/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 18:15:35 by meharit           #+#    #+#             */
/*   Updated: 2023/06/16 12:34:17 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
			if (herdoc_redir_in(tmp_in, h, env, &herdo))
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

void	open_out_files(t_cmd *table)
{
	t_redi	*r_out;
	int		fd;

	r_out = table->out;
	if (!g_exec.f_out)
	{
		while (r_out)
		{
			if (r_out->type == out)
			{
				fd = open(r_out->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd == -1)
					error_open_out(r_out->file);
				r_out = r_out->next;
			}
		}
	}
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
			printf("here\n");
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

// t > e > s < o must create outfiles (o in file)
// t < e < s > o create nothing		  (o out file)

void	error_open_out(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	perror(" ");
	g_exec.g_exit_status = 1;
	exit(g_exec.g_exit_status);
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
			if (out->type == append)
				fd = open(out->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd = open(out->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				error_open_out(out->file);
			out = out->next;
		}
		dup2(fd, 1);
	}
	return (fd);
}
