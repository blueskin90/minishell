/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 01:57:56 by toliver           #+#    #+#             */
/*   Updated: 2018/10/23 21:23:42 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				pwdshell(char **splittedline, t_envs *env)
{
	char		*currentpath;

	(void)env;
	if (splittedline[0])
		ft_printf("pwd: too many arguments\n");
	else
	{
		currentpath = getcwd(NULL, 0);
		if (currentpath)
		{
			ft_printf("%s\n", currentpath);
			free(currentpath);
		}
		else
			ft_printf("Couldn't find the current working directory\n");
	}
	return (1);
}
