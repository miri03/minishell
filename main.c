/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 17:19:51 by yismaail          #+#    #+#             */
/*   Updated: 2023/06/19 22:36:01 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	check_args(int ac, char **av, t_env **dup_env, char **env)
{
	(void)av;
	g_exec.g_exit_status = 0;
	if (ac != 1)
	{
		ft_putendl_fd("noo we don't do that here", 2);
		exit(1);
	}
	parse_env(env, dup_env);
}

void	remove_spaces(t_token **token, t_token *tok)
{
	t_token	*tmp;

	tmp = NULL;
	while (tok)
	{
		if (tok->type == SPACEE)
		{
			ft_remove(tmp, &tok, token);
		}
		else
		{
			tmp = tok;
			tok = tok->next;
		}
	}
}

void	check_cmd(t_cmd **cmd)
{
	char	**substr;
	int		i;

	if ((*cmd)->type == WORD && ft_strchr_2((*cmd)->cmd[0], ' '))
	{
		substr = ft_split((*cmd)->cmd[0], ' ');
		ft_free((*cmd)->cmd);
		i = 0;
		while (substr[i])
			i++;
		(*cmd)->cmd = (char **)malloc(sizeof(char *) * (i + 1));
		i = 0;
		while (substr[i])
		{
			(*cmd)->cmd[i] = ft_strdup(substr[i]);
			i++;
		}
		(*cmd)->cmd[i] = NULL;
		i = 0;
		ft_free(substr);
	}
}

void	ft_minishell(t_env **env, t_token **token, t_cmd **cmd)
{
	handler_expand(token, *env, *token);
	remove_spaces(token, *token);
	if (check_syntax(*token))
		parse_cmd(token, cmd);
	else
		ft_lstclear_t(token);
}

void	main_loop(t_token *token, t_env *dup_env, t_cmd *cmd)
{
	char	*line;

	while (1)
	{
		token = NULL;
		cmd = NULL;
		line = readline(GREEN "minishell> " RESET);
		if (!line)
			to_exit();
		if (is_all_spaces(line))
		{
			free(line);
			continue ;
		}
		add_history(line);
		if (token_line(line, &token))
		{
			ft_minishell(&dup_env, &token, &cmd);
			check_cmd(&cmd);
			execute(cmd, &dup_env);
			clear_cmds(&cmd);
		}
		free(line);
	}
}

int	main(int ac, char **av, char **env)
{
	t_token	*token;
	t_env	*dup_env;
	t_cmd	*cmd;

	dup_env = NULL;
	minishell_mess();
	check_args(ac, av, &dup_env, env);
	set_signals();
	token = NULL;
	cmd = NULL;
	main_loop(token, dup_env, cmd);
}
