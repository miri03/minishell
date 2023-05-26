/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:45:07 by meharit           #+#    #+#             */
/*   Updated: 2023/05/26 21:41:02 by meharit          ###   ########.fr       */
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
		dprintf(2, "in[%d][0] = %d out[%d][1] %d\n", i, pipes[i][0], i,
				pipes[i][1]);
		i++;
	}
}

//// cat << del | head -n 3 | cat | wc -l

void	wait_all(int *pid, int last) //recheck it
{
	int	i;
	int	status;

	i = 0;
	close(exec.pipes[1][0]);
	close(exec.pipes[1][1]);
	close(exec.pipes[0][1]);
	close(exec.pipes[0][0]);
	while (i < last - 1)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
	waitpid(pid[i], &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}

void	multi_cmd(t_env *env, t_cmd *table)
{
	int	i;
	int	tbl_len;
	int	*f_pid;
	
	i = 0;
	tbl_len = table_len(table);
	exec.pipes = (int **)malloc(sizeof(int *) * 2);
	f_pid = (int *)malloc(sizeof(int) * tbl_len);
	make_pipes(exec.pipes);
	while (table)
	{
		f_pid[i] = fork();
		if (f_pid[i] == 0)
		{
			if (i == 0)
			{
				dprintf(2, "cmd 1\n");
				execute_cmds(table->cmd[0], table, env, 0, i);
			}
				
			else if (i == tbl_len - 1)
			{
				dprintf(2, "cmd last\n");
				execute_cmds(table->cmd[0], table, env, 2, i);
			}
				
			else
			{
				dprintf(2, "cmd middle\n");
				execute_cmds(table->cmd[0], table, env, 1, i);
			}
				
		}
		table = table->next;
		i++;	
	}
	wait_all(f_pid, i);
}

// void	multi_cmd(t_env *env, t_cmd *table)
// {
// 	int	**pipes;
// 	int	*f_pid;
// 	int	i;
// 	int	j;
// 	int	tbl_len;
// 	int	buit_in;

// 	j = 0;
// 	i = 0;
// 	tbl_len = table_len(table);
// 	pipes = (int **)malloc(sizeof(int *) * 2);
// 	f_pid = (int *)malloc(sizeof(int) * tbl_len); //if builtin ??
// 	make_pipes(pipes);
// 	while (table)
// 	{
// 		buit_in = 0;
// 		if (!is_builtin(table->cmd[0]))
// 		{
// 			f_pid[i] = fork();
// 			i++;
// 		}
// 		else
// 			buit_in = 1;
// 		if (j == 0 && (!f_pid[i] || buit_in))
// 			first_cmd(table->cmd[0], table, env, pipes);
// 		if (j < tbl_len - 1 && (!f_pid[i] || buit_in))
// 			cmds(table->cmd[0], table, env, pipes, j);
// 		if (j == tbl_len - 1 && (!f_pid[i] || buit_in))
// 			last_cmd(table->cmd[0], table, env, pipes, tbl_len, j);
// 		if (j == tbl_len - 1)
// 		{
// 			close(pipes[1][0]);
// 			close(pipes[1][1]);
// 			close(pipes[0][1]);
// 			close(pipes[0][0]);
// 		}	
// 		j++;
// 		table = table->next;
// 	}
// 	wait_all(f_pid, i, pipes);
// }



