/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:02:22 by meharit           #+#    #+#             */
/*   Updated: 2023/05/27 23:40:21 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**find_path(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, "PATH") == 0)
			return (ft_split(env->value, ':'));
		env = env->next;
	}
	return (NULL);
}

char	*cmd_exist(t_cmd *table, t_env *env)
{
	char	**path;
	char	*cmd;
	char	*test;
	int		i;

	i = 0;
	if (!table->cmd)
		return (NULL);
	if (access(table->cmd[0], F_OK) == 0) //does exist
		return (table->cmd[0]);
	cmd = ft_my_strjoin("/", table->cmd[0]);
	path = find_path(env);
	if (path)
	{
		while (path[i])
		{
			test = ft_strjoin(path[i], cmd);
			if (access(test, X_OK) == 0)
				return (test);
			i++;
		}
	}
	return (NULL); //
}

void	exec_single(t_env *env, t_cmd *table)
{
	char	*cmd_path;
	int		f_pid;
	int		status;
	// int		**herdoc_p;

	if (table->cmd && is_builtin(table->cmd[0]))
		exec_builtin(table->cmd[0], table, &env);
	else
	{
		f_pid = fork();
		if (!f_pid)
		{
			cmd_path = cmd_exist(table, env);
			redir_in(table, 0);
			redir_out(table);
			if (!cmd_path && table->cmd)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(table->cmd[0], 2);
				ft_putstr_fd(": command not found\n", 2);
				exec.g_exit_status = 127;
				exit(exec.g_exit_status);
			}
			execve(cmd_path, table->cmd, exec.env);
			if (!table->cmd)
				exit(0);
			ft_putstr_fd("problem exec\n", 2);
		}
		else
		{
			// herdoc_p = exec.herdoc_pipe;
			// close(herdoc_p[0][1]);
			// close(herdoc_p[0][0]);
			waitpid(f_pid, &status, 0);
			exec.g_exit_status = WEXITSTATUS(status);
		}
	}
}

int	table_len(t_cmd *table)
{
	int	len;

	len = 0;
	while (table)
	{
		table = table->next;
		len++;
	}
	return (len);
}

t_exec	init_exec()
{
	t_exec	exec;
	
	exec.built_in = 0;
	exec.std_in = dup(STDIN_FILENO);
	exec.std_out = dup(STDOUT_FILENO);
	return (exec);
}

void	execute(t_cmd *table, t_env **dup_env)
{
	int		i;
	
	i = 0;
	if (!table)
		return;
	open_herdoc(table);
	if (table_len(table) == 1)
	{
		dprintf(2, "exec single\n");
		exec_single(*dup_env, table);
	}
	else
	{
		dprintf(2, "exec multi\n");
		multi_cmd(*dup_env, table);
	}
}
