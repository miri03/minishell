/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 17:19:51 by yismaail          #+#    #+#             */
/*   Updated: 2023/06/16 23:25:57 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	check_tokens(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	printf("--------CHECK_TOKENS-----------\n");
	while (tmp)
	{
		printf("content = %s\n", tmp->content);
		printf("type    = %d\n", tmp->type);
		tmp = tmp->next;
	}
	printf("----------------------------\n");
}

void    get_input(t_cmd *command)
{
	
    t_cmd *cmd;
	t_redi *in;
	t_redi *out;

    int     i;
    int     x;
	
    cmd = command;
	printf("----------TABLE-------------\n");
	i = 0;
	while (cmd)
    {
        x = 0;
        while (cmd->cmd && cmd->cmd[x])
        {
            printf("[node:%d] cmd[%d] = %s\n", i, x, cmd->cmd[x]);
            x++;
        }
		printf("[node:%d] pipe   = %d\n", i,cmd->pipe);
		printf("[node:%d] er     = %d\n", i,cmd->err);
        if (cmd->in)
        {
			in = cmd->in;
			while (in)
			{
				printf("-------------in-------------\n");
				printf("type = %d\n", in->type);
				printf("file = %s\n", in->file);
				printf("m_expd = %d\n", in->must_exp);
				in = in->next;
			}
        }
        if (cmd->out)
        {
			out = cmd->out;
			while (out)
			{
			  printf("-------------out------------\n");
			  printf("type = %d\n", out->type);
			  printf("file = %s\n", out->file);
			  printf("m_expd = %d\n", out->must_exp);
			  out = out->next;
			}
        }
        printf("----------------------------\n");
		i++;
		cmd = cmd->next;
	}
	printf("\n");
}

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
			get_input(cmd);
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
	// minishell_mess();
	check_args(ac, av, &dup_env, env);
	g_exec.g_exit_status = 0;
	set_signals();
	token = NULL;
	cmd = NULL;
	main_loop(token, dup_env, cmd);
}
