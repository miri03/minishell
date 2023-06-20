/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exist_uti.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meharit <meharit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 01:03:00 by meharit           #+#    #+#             */
/*   Updated: 2023/06/20 01:03:17 by meharit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_path(char **path)
{
	int	i;

	i = 0;
	while (path && path[i])
		free(path[i++]);
	free(path);
}

void	free_cmd_path(char **path, char *cmd)
{
	free_path(path);
	free(cmd);
}
