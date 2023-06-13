/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:45:07 by meharit           #+#    #+#             */
/*   Updated: 2023/06/13 17:05:46 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	open_uno(int i)
{
	if (i % 2 == 0)
		pipe(exec.pipes[1]);
	else
		pipe(exec.pipes[0]);
}

void	parent_multi(int i, int tbl_len, t_redi *in)
{
	if (i == 0)
	{
		close(exec.pipes[1][0]);
		close(exec.pipes[1][1]);
		close(exec.pipes[0][1]);
	}
	else if (i == tbl_len - 1)
	{
		close(exec.pipes[0][0]);
		close(exec.pipes[1][0]);
		close(exec.pipes[1][1]);
		close(exec.pipes[0][1]);
	}
	else if (i % 2 == 0)
	{
		close(exec.pipes[0][1]);
		close(exec.pipes[1][0]);
	}
	else // i % 2 != 0
	{
		close(exec.pipes[0][0]);
		close(exec.pipes[1][1]);
	}
	close_herdoc(in, i);
	open_uno(i);
}

void	child_multi(t_env *env, t_cmd *table, int i, int tbl_len)
{
	set_default();
	if (i == 0)
		execute_cmds(table, env, 0, i);
	else if (i == tbl_len - 1)
		execute_cmds(table, env, 2, i);
	else
		execute_cmds(table, env, 1, i);
}

void	multi_cmd(t_env *env, t_cmd *table)
{
	int	i;
	int	tbl_len;
	int	*f_pid;

	i = 0;
	tbl_len = table_len(table);
	exec.pipes = (int **)malloc(sizeof(int *) * 2);
	make_pipes(exec.pipes);
	f_pid = (int *)malloc(sizeof(int) * tbl_len);
	while (table)
	{
		f_pid[i] = fork();
		if (f_pid[i] == 0) // child proc
			child_multi(env, table, i, tbl_len);
		// back to parent
		else
			parent_multi(i, tbl_len, table->in);
		table = table->next;
		i++;
	}
	wait_all(f_pid, i);
	free(exec.pipes[0]);
	free(exec.pipes[1]);
	free(exec.pipes);
	free(f_pid);
}
