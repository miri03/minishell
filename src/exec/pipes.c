/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 16:45:07 by meharit           #+#    #+#             */
/*   Updated: 2023/05/23 16:18:33 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    make_pipes(int **pipes)
{
    int i;

    i = 0;
    while (i < 2)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) == -1)
			perror("pipe");
        dprintf(2, "in[%d][0] = %d out[%d][1] %d\n", i, pipes[i][0], i, pipes[i][1]);
		i++;
	}
}

void    pipes(t_env *env, t_cmd *table)
{
	int **pipes;
	int *f_pid;
	int i;
	int j;
    int tbl_len;

	j = 0;
	i = 0;
    tbl_len = table_len(table);
	pipes = (int **)malloc(sizeof(int *) * 2);
	f_pid = (int *)malloc(sizeof(int) * tbl_len);  //if builtin ??
    make_pipes(pipes);
	while (table)
	{
		if (!is_builtin(table->cmd[0]))
		{
			f_pid[i] = fork();
            // if (f_pid[i])
            //     dprintf(2, "%d==>%d\n", i, f_pid[i]);
            if (j == 0 && !f_pid[i])
                first_cmd(table->cmd[0], table, env, pipes);

            if (j < tbl_len - 1 && !f_pid[i])
                cmds(table->cmd[0], table, env, pipes, j);

            if (j == tbl_len - 1 && !f_pid[i])
                last_cmd(table->cmd[0], table, env, pipes, tbl_len, j);
            
            if (j == tbl_len - 1)
            {
                close(pipes[1][0]);
                close(pipes[1][1]);
                close(pipes[0][1]);
                close(pipes[0][0]);
            }

//// cat << del | head -n 3 | cat | wc -l        
			i++;
		}
		j++;
		
		table = table->next;
	}
	wait_all(f_pid, i, pipes);
}

void wait_all(int *pid, int last, int **pipes)
{
    int i;
    int status;

    i = 0;
    close(pipes[1][0]);
    close(pipes[1][1]);
    close(pipes[0][1]);
    close(pipes[0][0]);
    while (i < last - 1)
    {
        waitpid(pid[i] , NULL, 0);
        i++;
    }
    waitpid(pid[i], &status, 0);
    if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}
