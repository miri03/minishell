
// #include "../../includes/minishell.h"

// void    first_cmd(char *cmd, t_cmd *table, t_env *env, int **pipes)
// {
// 	char    *cmd_path;

// 	cmd_path = cmd_exist(cmd, env);
// 	redir_in(table, cmd_path, cmd);
// 	redir_out(table, cmd_path, cmd); /// ?
// 	dup2(1, pipes[0][1]);
// 	if (!cmd_path)
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(cmd, 2);
// 		ft_putstr_fd(": command not found\n", 2);
// 		g_exit_status = 127;
// 		exit (g_exit_status);
// 	}     //if no infile or outfile
// 	execve(cmd_path, table->cmd, find_path(env));
// 	ft_putstr_fd("problem exec\n", 2);
// }

// void wait_all(pid_t waited, int *status)
// {
// 	int stat;
// 	int id;

// 	id = wait(&stat);
// 	if (id == waited)
// 		*status = stat;
// 	if (id != -1)
// 		wait_all(waited, status);
// 	return ; 
// }

// void    pipes(t_env *env, t_cmd *table)
// {
// 	int **pipes;
// 	int *f_pid;
// 	int i;
// 	int j;
// 	int	status;
// 	int	cmds;

// 	j = 0;
// 	i = 0;
// 	cmds = table_len(table);
// 	pipes = (int **)malloc(sizeof(int *) * 2);
// 	f_pid = (int *)malloc(sizeof(int) * table_len(table));  //if builtin ??
// 	while (i < 2)
// 	{
// 		pipes[i] = malloc(sizeof(int) * 2);
// 		if (pipe(pipes[i]) == -1)
// 			perror("pipe");
// 		i++;
// 	}
// 	i = 0;
// 	while (table)
// 	{
		
// 		if (!is_builtin(table->cmd[0]))
// 		{
// 			f_pid[i] = fork();
// 			if (!f_pid[i])
// 			{
// 				dprintf(2, "i give my second chance to cupid\n");
// 				if (j == 0)
// 				  first_cmd(table->cmd[0], table, env, pipes);
// 				// if (j == cmds - 1)
// 				// 	last_cmd();
// 			}

// 			i++;
// 		}
// 		j++;
		
// 		table = table->next;
// 	}
// 	wait_all(f_pid[i-1],&status);
// 	// waitpid(-1, &status, 0);
// 	// g_exit_status = WEXITSTATUS(status);

// }