/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_uti.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 15:55:38 by meharit           #+#    #+#             */
/*   Updated: 2023/06/13 17:03:45 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	make_pipes(int **pipes)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) == -1)
			perror("pipe");
		i++;
	}
}

void	wait_all(int *pid, int last)
{
	int	i;
	int	status;

	i = 0;
	close(exec.pipes[1][0]);
	close(exec.pipes[1][1]);
	close(exec.pipes[0][1]);
	close(exec.pipes[0][0]);
	waitpid(pid[last - 1], &status, 0);
	if (WIFEXITED(status))
		exec.g_exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		exec.g_exit_status = WTERMSIG(status) + 128;
		return ;
	}
	while (i < last - 1)
	{
		waitpid(-1, NULL, 0);
		i++;
	}
}

void	close_herdoc(t_redi *in, int i)
{
	if (n_herdoc(in))
	{
		close(exec.herdoc_pipe[i][0]);
		close(exec.herdoc_pipe[i][1]);
		free(exec.herdoc_pipe[i]);
	}
}
