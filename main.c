/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 17:19:51 by yismaail          #+#    #+#             */
/*   Updated: 2023/06/13 17:09:07 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	check_args(int ac, char **av, t_env **dup_env, char **env)
{
	(void)av;
	exec.g_exit_status = 0;
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

void	ft_minishell(t_env **env, t_token **token, t_cmd **cmd)
{
	handler_expand(token, *env, *token);
	remove_spaces(token, *token);
	if (check_syntax(*token))
		parse_cmd(token, cmd);
	else
		ft_lstclear_t(token);
}

void	sig_int_handler(int s)
{
	(void)s;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	set_signals(void)
{
	signal(SIGQUIT, SIG_IGN);        //ctr-\ //
	signal(SIGINT, sig_int_handler); //ctr-c //
}

void	set_default(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_token	*token;
	t_env	*dup_env;
	t_cmd	*cmd;

	dup_env = NULL;
	minishell_mess();
	check_args(ac, av, &dup_env, env);
	exec.g_exit_status = 0;
	set_signals();
	while (1)
	{
		token = NULL;
		cmd = NULL;
		line = readline(GREEN "minishell> " RESET);
		if (!line)
		{
			ft_putstr_fd("exit\n", 2);
			exit(exec.g_exit_status);
		}
		if (is_all_spaces(line))
		{
			free(line);
			continue ;
		}
		add_history(line);
		if (token_line(line, &token))
		{
			ft_minishell(&dup_env, &token, &cmd);
			exec.herdoc_pipe = malloc(sizeof(t_exec) * table_len(cmd)); //
			get_input(cmd);
			execute(cmd, &dup_env);
			free(exec.herdoc_pipe);
			clear_cmds(&cmd);
		}
		free(line);
	}
}

//<< m cat | ls | <<m
//ls | << m cat | cat