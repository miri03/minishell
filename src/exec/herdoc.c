/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 14:13:03 by meharit           #+#    #+#             */
/*   Updated: 2023/06/13 22:46:01 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	herdoc_redir_in(t_redi *tmp_in, int h, t_env *env, int *herdo)
{
	int	p_id;

	if (tmp_in->type == heredoc)
	{
		p_id = fork();
		if (!p_id)
			herdoc_child(*herdo, tmp_in, env, h);
		else
		{
			(*herdo)--;
			if (parent_herdoc(p_id, h))
				return (1);
		}
	}
	return (0);
}

void	make_herdoc_pipe(int herdo, int h)
{
	if (herdo)
	{
		g_exec.herdoc_pipe[h] = malloc(sizeof(t_exec) * 2);
		if (pipe(g_exec.herdoc_pipe[h]) == -1)
			perror("pipe\n");
	}
}

// // <--herdoc readme--> //

// // aloceted as much of nodes I have for herdoc pipes
// // check each node of the command table
// // put the number of herdocs in each redir_in in the variable herdo
// // if there is at least one herdoc open one pipe to fill it whith the content of
// // the last one loop around redir_in check if I have a herdoc fork to set
// // signals to default open herdocs but only put the content of the last of in
// // the pipe close the write end of the pipe exit from the child process
// // meanwhile in the parent wait for the child and stop executing if I got a
// // signal keep looping around the redir_in until the last in that node keep
// // looping around the nodes until the last add up to the index of herdoc pipes

// // << m cat <<g cat
