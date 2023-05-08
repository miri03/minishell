/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:02:22 by meharit           #+#    #+#             */
/*   Updated: 2023/05/08 19:13:58 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

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

char	*cmd_exist(char *cmd, t_env *env)
{
	char	**path;
	char	*test;
	int		i;

	i = 0;
	if (access(cmd, F_OK) == 0) //does exist
		return (cmd);
	cmd = ft_my_strjoin("/", cmd);
	path = find_path(env);
	if (path)
	{
		while (path[i])
		{
			test = ft_strjoin(path[i],cmd);
			if (access(test, X_OK) == 0)
				return (test);
			i++;
		}
	}
	return (NULL);//
}

void	exec(char *cmd, t_env *env, t_cmd *table)
{
	char	*cmd_path;

	printf("[%s] not builin\n", cmd);
	cmd_path = cmd_exist(cmd, env);
	if (!cmd_path)
	{
		printf("command not found\n");
		return ;
	}
	execve(cmd_path, &table->cmd[1], find_path(env));  //fix options
	printf("error\n");
}

void	check_builin(char *cmd, t_cmd *table, t_env **env)
{
	if (ft_strcmp(cmd , "exit") == 0)
	{
		ft_exit(table);
		return ;
	}
	if (ft_strcmp(cmd , "env") == 0)
	{
		ft_env(env);
		return ;
	}
	if (ft_strcmp(cmd , "pwd") == 0)
	{
		ft_pwd();
		return ;
	}
	if (ft_strcmp(cmd , "unset") == 0)
	{
		ft_unset(env, table);
		return ;
	}
	if (ft_strcmp(cmd , "cd") == 0)
	{
		ft_cd(table, env);
		return ;
	}
	if (ft_strcmp(cmd , "echo") == 0)
	{
		ft_echo(table);
		return ;
	}
	if (ft_strcmp(cmd , "export") == 0)
	{
		ft_export(*env, table);
		return ;
	}
	if (!fork())
		exec(cmd, *env, table);
	// else


}

void	execute(t_cmd *cmd, t_env **dup_env)
{
	int		i;

	i = 0;
	while (cmd)
	{
		check_builin(cmd->cmd[i], cmd, dup_env);
		cmd = cmd->next;
		// i++;
	}
}
