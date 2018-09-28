/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd3.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 17:52:54 by toliver           #+#    #+#             */
/*   Updated: 2018/09/28 17:53:13 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				isabsolute(char *str)
{
	if (str[0] == '/')
		return (1);
	return (0);
}

int				isvalidpath(char *path)
{
	if (access(path, F_OK) == -1)
		ft_printf("minishell: no such file or directory: %s\n", path);
	else if (access(path, X_OK) == -1)
		ft_printf("cd: permission denied: %s\n", path);
	else
		return (1);
	return (0);
}
