/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:54:46 by toliver           #+#    #+#             */
/*   Updated: 2020/01/12 04:25:07 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_crash(int value, char *param, t_env *env)
{
	(void)param;
	if (value == MALLOC_FAIL)
		ft_dprintf(2, "%s: Malloc failed\n", env->prog_name);
	else
		ft_dprintf(2, "%s: Unknown error !\n", env->prog_name);
	ft_free_env(env);
	return (0);	
}

char			*ft_error_origin(int origin)
{
	if (origin == ENV)
		return ("env");
	else if (origin == SETENV)
		return ("setenv");
	else if (origin == UNSETENV)
		return ("unsetenv");
	return ("UNKNOWN");
}

int				ft_warning(int origin, int value, char *param, t_env *env)
{
	(void)env;
	ft_dprintf(2, "%s: ", ft_error_origin(origin));
	if (value == MISSING_EQUAL)
		ft_dprintf(2, "%s does not contain an =\n", param);
	else if (value == MISSING_VARIABLE)
		ft_dprintf(2, "couldn't find variable : %s in env\n", param);
	else
		ft_dprintf(2, "Unknown warning !\n");
	return (1);	
}

int				ft_error(int origin, int value, char *param, t_env *env)
{
	(void)env;
	(void)value;
	(void)param;
	ft_dprintf(2, "%s: ", ft_error_origin(origin));
	ft_dprintf(2, "Unknown error !\n");
	return (1);	
}

int				ft_usage(void)
{
	ft_printf("Usage: ./minishell\n");
	return (-1);
}
