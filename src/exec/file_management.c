/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 18:15:35 by meharit           #+#    #+#             */
/*   Updated: 2023/06/11 17:56:47 by meharit          ###   ########.fr       */
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
	exec.n_herdoc = n;
	return (n);
}

// export _*

// leaks head of export

// rm current directory & pwd

// oldpwd no env

int	open_herdoc(t_cmd *table)
{
	t_redi	*tmp_in;
	int		herdo;
	int		h;
	int		i;
	char	*line;
	int		p_id;
	int		status;

	h = 0;
	i = 0;
	while (table)
	{
		tmp_in = table->in;
		herdo = n_herdoc(tmp_in);
		if (herdo)
		{
			exec.herdoc_pipe[h] = malloc(sizeof(t_exec) * 2);
			if (pipe(exec.herdoc_pipe[h]) == -1)
				perror("pipe\n");
			dprintf(2, "herdoc[%d][0] = %d herdoc[%d][1] = %d\n", h, exec.herdoc_pipe[h][0], h, exec.herdoc_pipe[h][1]);
		}
		while (tmp_in)
		{
			if (tmp_in->type == heredoc)
			{
				p_id = fork();
				if (!p_id)
				{
					set_default();
					while (1)
					{
						line = readline(">");
						if (!line || !ft_strcmp(line, tmp_in->file))
						{
							free (line);
							break ;
						}
						
						if (herdo == 1)
						{
							write(exec.herdoc_pipe[h][1], line, ft_strlen(line));
							write(exec.herdoc_pipe[h][1], "\n", 1);
						}
						free (line);
					}
					
					
					if (!herdo)
					{
						close(exec.herdoc_pipe[h][1]);
					}
					exit (exec.g_exit_status);
				}
				else
				{
					herdo--;
					waitpid(p_id, &status, 0);
					if (WIFSIGNALED(status))
					{
						free(exec.herdoc_pipe[h]);
						exec.g_exit_status = WTERMSIG(status) + 128;
						return(1);
					}
				}
			}
			tmp_in = tmp_in->next; 
		}
		table = table->next;
		h++;	
	}
	return (0);
}

// <--herdoc readme--> //

// aloceted as much of nodes I have for herdoc pipes
// check each node of the command table 
	// put the number of herdocs in each redir_in in the variable herdo
	// if there is at least one herdoc open one pipe to fill it whith the content of the last one
	// loop around redir_in
		// check if I have a herdoc
		// fork to set signals to default
		// open herdocs
		// but only put the content of the last of in the pipe
		// close the write end of the pipe
		// exit from the child process
		// meanwhile in the parent wait for the child and stop executing if I got a signal
		// keep looping around the redir_in until the last in that node
	// keep looping around the nodes until the last
	// add up to the index of herdoc pipes

// << m cat <<g cat

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
				exec.g_exit_status = 1;
				if (exec.built_in == 0)
					exit(exec.g_exit_status);
				return ;
			}
			dup2(fd, 0);
		}
		else
		{
			dup2(exec.herdoc_pipe[i][0], STDIN_FILENO);
			close(exec.herdoc_pipe[i][0]);
			close(exec.herdoc_pipe[i][1]);
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
				exec.g_exit_status = 1;
				exit(exec.g_exit_status);
			}
			out = out->next;
		}
		dup2(fd, 1);
	}
	return (fd);
}
