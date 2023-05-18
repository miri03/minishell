/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yismaail <yismaail@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 17:19:51 by yismaail          #+#    #+#             */
/*   Updated: 2023/05/18 03:53:04 by yismaail         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	minishell_mess()
{
	printf(RED"\n ███▄ ▄███▓ ██▓ ███▄    █  ██▓  ██████  ██░ ██ ▓█████  ██▓     ██▓    \n");
	printf("▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▒▒██    ▒ ▓██░ ██▒▓█   ▀ ▓██▒    ▓██▒    \n");
	printf("▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒░ ▓██▄   ▒██▀▀██░▒███   ▒██░    ▒██░    \n");
	printf("▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░  ▒   ██▒░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░    \n");
	printf("▒██▒   ░██▒░██░▒██░   ▓██░░██░▒██████▒▒░▓█▒░██▓░▒████▒░██████▒░██████▒\n");
	printf("░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓  ▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░\n");
	printf("░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░░ ░▒  ░ ░ ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░\n");
	printf("░      ░    ▒ ░   ░   ░ ░  ▒ ░░  ░  ░   ░  ░░ ░   ░     ░ ░     ░ ░   \n");
	printf("       ░    ░           ░  ░        ░   ░  ░  ░   ░  ░    ░  ░    ░  ░\n"RESET);
	printf(BLUE"\nby: meharit && yismaail\n"RESET);
	printf("\n\n");
	
}

void	check_args(int ac, char **av, t_env **dup_env, char **env)
{
	(void)av;
	g_exit_status = 0;
	if (ac != 1)
	{
		ft_putendl_fd("noo we don't do that here", 2);
		exit(1);
	}
	parse_env(env, dup_env);
}

void	remove_spaces(t_token **token, t_token *tok)
{
	t_token *tmp;
	
	tmp = NULL;
	while (tok)
	{
		if (tok->type == SPACE)
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

/* void	show_in(t_cmd *cmd, t_env **env)
{
	(void)env;
	while (cmd)
	{
		printf("content : %s\n", cmd->cmd[0]);
	
		printf("content : %s\n", cmd->cmd[2]);

		cmd = cmd->next;
	}
} */

void	ft_minishell(t_env **env, t_token **token, t_cmd **cmd)
{
	handler_expand(token, *env, *token);
	remove_spaces(token, *token);
	if (check_syntax(*token))
	{
		parse_cmd(token, cmd);
		//show_in(*cmd, env);
			// while (*cmd)
			// {
			// 	printf("%s\n", (*cmd)->cmd[0]);
			// 	printf("%s\n", (*cmd)->cmd[1]);
			// 	(*cmd) = (*cmd)->next;
			// }
		
		// // printf("lhamdolilah\n");
	}
	else
		ft_lstclear_t(token);
}
int	is_all_spaces(char *str)
{
	int i = 0;
	while(str[i])
	{
		if((str[i] != ' ' || str[i] != '\n' || str[i] != '\t'))
			return (0);
		i++;
	}
	return (1);
}

void	ft_free(char **str)
{
	int	i;

	i = 0;
	if (str)
	{
		while (str[i])
			free(str[i++]);
		free(str);
	}
}

void	clear_cmds(t_cmd **cmd)
{
	t_cmd	*tmp1;
	t_cmd	*tmp2;

	if (!cmd || !*cmd)
		return ;
	tmp1 = *cmd;
	while (tmp1)
	{
		if (tmp1->in)
			ft_lstclear_2(&tmp1->in);
		if (tmp1->out)
			ft_lstclear_2(&tmp1->out);
		if (tmp1->cmd)
			ft_free(tmp1->cmd);
		tmp2 = tmp1->next;
		free(tmp1);
		tmp1 = tmp2;
	}
}
int	main(int ac, char **av, char **env)
{
	// int		i;
	char	*line;
	t_token	*token;
	t_env	*dup_env;
	t_cmd	*cmd;

	dup_env = NULL;
	check_args(ac, av, &dup_env, env);
	// minishell_mess();
	while (1)
	{
		token = NULL;
		cmd = NULL;
		line = readline(GREEN"minishell> "RESET);
		if (!line)
			exit(g_exit_status);
		if(is_all_spaces(line))
		{
			free(line);
			continue ;
		}
		add_history(line);
		if (token_line(line, &token))
		{
			ft_minishell(&dup_env, &token, &cmd);
			get_input(cmd);
			// execute(cmd, &dup_env);
			clear_cmds(&cmd);
		}
		free(line);

		// //for parsing  leaks
		// i = 0;
		// if (cmd)
		// {
		// while (cmd->cmd[i])
		// {
		// 	free(cmd->cmd[i]);
		// 	i++;
		// }
		// free (cmd->cmd);  //char **cmd
		// }
		// //
	}
}
