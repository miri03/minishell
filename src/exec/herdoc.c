// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   herdoc.c                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/06/13 14:13:03 by meharit           #+#    #+#             */
// /*   Updated: 2023/06/13 17:20:45 by meharit          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../includes/minishell.h"

// int	n_herdoc(t_redi *in)
// {
// 	int	n;

// 	n = 0;
// 	while (in)
// 	{
// 		if (in->type == heredoc)
// 			n++;
// 		in = in->next;
// 	}
// 	g_exec.n_herdoc = n;
// 	return (n);
// }

// // recheck no env

// int	make_herdoc_pipe(int h, t_redi *in)
// {
// 	int	herdo;

// 	herdo = n_herdoc(in);
// 	if (herdo)
// 	{
// 		g_exec.herdoc_pipe[h] = malloc(sizeof(t_exec) * 2);
// 		if (pipe(g_exec.herdoc_pipe[h]) == -1)
// 			perror("pipe\n");
// 	}
// 	return (herdo);
// }

// void	herdoc(int herdo, t_redi *tmp_in, t_env *env, int h)
// {
// 	char	*line;

// 	while (1)
// 	{
// 		line = readline(">");
// 		if (!line || !ft_strcmp(line, tmp_in->file))
// 		{
// 			free(line);
// 			break ;
// 		}
// 		if (herdo == 1)
// 		{
// 			if (tmp_in->must_exp)
// 				expand_var(env, &line);
// 			write(g_exec.herdoc_pipe[h][1], line, ft_strlen(line));
// 			write(g_exec.herdoc_pipe[h][1], "\n", 1);
// 		}
// 		free(line);
// 	}
// }

// int	parent_herdoc(int p_id, int h)
// {
// 	int	status;

// 	waitpid(p_id, &status, 0);
// 	if (WIFSIGNALED(status))
// 	{
// 		free(g_exec.herdoc_pipe[h]);
// 		g_exec.g_exit_status = WTERMSIG(status) + 128;
// 		return (1);
// 	}
// 	return (0);
// }

// int	is_herdoc(int herdo, int h, t_redi *tmp_in, t_env *env)
// {
// 	int	p_id;

// 	p_id = fork();
// 	if (!p_id)
// 	{
// 		set_default();
// 		herdoc(herdo, tmp_in, env, h);
// 		if (!herdo)
// 			close(g_exec.herdoc_pipe[h][1]);
// 		exit(g_exec.g_exit_status);
// 	}
// 	else
// 	{
// 		herdo--;
// 		if (parent_herdoc(p_id, h))
// 			return (1);
// 	}
// 	return (0);
// }

// // <--herdoc readme--> //

// // aloceted as much of nodes I have for herdoc pipes
// // check each node of the command table
// // put the number of herdocs in each redir_in in the variable herdo
// // if there is at least one herdoc open one pipe to fill it whith the content of
// // the last one loop around redir_in check if I have a herdoc fork to set
// // signals to default open herdocs but only put the content of the last of in
// // the pipe close the write end of the pipe exit from the child process
// // meanwhile in the parent wait for the child and stop executing if I got a
// // signal keep looping around the redir_in until the last in that node keep
// // looping around the nodes until the last add up to the index of herdoc pipes

// // << m cat <<g cat
