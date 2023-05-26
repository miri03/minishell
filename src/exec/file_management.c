/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 18:15:35 by meharit           #+#    #+#             */
/*   Updated: 2023/05/26 16:15:28 by meharit          ###   ########.fr       */
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
	return (n);
}

void	open_herdoc(t_cmd *table)
{
	t_redi	*tmp_in;
	int		herdo;
	char	*line;

	tmp_in = table->in;
	herdo = n_herdoc(tmp_in);
	while (tmp_in)
	{
		if (tmp_in->type == heredoc)
		{
			exec.herdoc_pipe = malloc(sizeof(t_exec) * 2);
			if (pipe(exec.herdoc_pipe) == -1)
				perror("pipe\n");
			printf("in = %d out = %d\n", exec.herdoc_pipe[0], exec.herdoc_pipe[1]);
			while (1)
			{
				line = readline(">");
				if (!line || !ft_strcmp(line, tmp_in->file))
					break ;
				if (herdo == 1)
				{
					write(exec.herdoc_pipe[1], line, ft_strlen(line));
					write(exec.herdoc_pipe[1], "\n", 1);
				}
				free (line);
			}
			herdo--;
			if (!herdo)
				close(exec.herdoc_pipe[1]);
		}
		tmp_in = tmp_in->next;
	}
}

void	redir_in(t_cmd *table)
{
	int		fd;
	t_redi	*r_in;

	r_in = table->in;
	while (r_in)
	{
		if (r_in->type == in)
		{
			dprintf(2, "--------------->redir_in\n");
			fd = open(r_in->file, O_RDONLY);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(r_in->file, 2);
				perror(" ");
				g_exit_status = 1;
				if (exec.built_in == 0)
					exit(g_exit_status);
				return ;
			}
			dup2(fd, 0);
		}
		else
		{
			dprintf(2, "--------------->redir_herd\n");
			dup2(exec.herdoc_pipe[0], STDIN_FILENO);
			close(exec.herdoc_pipe[0]);
		}
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
				g_exit_status = 1;
				exit(g_exit_status);
			}
			out = out->next;
		}
		dup2(fd, 1);
	}
	return (fd);
}
