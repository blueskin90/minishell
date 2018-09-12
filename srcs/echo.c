/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 22:45:48 by toliver           #+#    #+#             */
/*   Updated: 2018/09/12 20:08:30 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				printvariable(char *str, int len, t_envs *env)
{
	char		c;
	t_var		*ptr;
	char		*name;

	c = str[len];
	str[len] = '\0';
	ptr = env->envp;
	name = ft_strdup(str);
	while (ptr && ft_strcmp(name, ptr->name))
		ptr = ptr->next;
	if (ptr)
		ft_putstr(ptr->value);
	str[len] = c;
	return (1);
}

int				echowithvariable(char *str, t_envs *env)
{
	int			i;
	int			ii;

	i = 0;
	ii = 0;
	i++;
	while (str[i + ii] && iswhitespace(str[i + ii]) == 0)
		ii++;
	printvariable(str + i, ii, env);
	if (str[i + ii])
		echostr(str + i + ii, env);
	return (1);
}

int				echostr(char *str, t_envs *env)
{
	int			i;

	i = 0;
	if (str[0] == '~' && str[1] == '\0' && envvarexist("HOME", env->envp))
	{
		printvarvalue("HOME", env->envp);
		i++;
	}
	while (str[i])
	{
		if (str[i] == '$' && ((i > 0 && str[i] - 1 != '\\') || i == 0))
		{
			echowithvariable(str + i, env);
			break ;
		}
		if (str[i] == '\\')
			i++;
		ft_putchar(str[i]);
		i++;
	}
	return (1);
}

int				echo(char **instructions, t_envs *env)
{
	int			i;
	int			isfirst;
	int			noption;

	i = 1;
	isfirst = 1;
	noption = 0;
	if (instructions && instructions[i][0] == '-')
	{
		if (instructions[i][1] == 'n')
			noption = 1;
		if (instructions[i][1] == '\0' || instructions[i][1] == 'n')
			i++;
	}
	while (instructions && instructions[i])
	{
		if (isfirst == 0)
			ft_putchar(' ');
		isfirst = 0;
		echostr(instructions[i], env);
		i++;
	}
	if (!noption)
		ft_putchar('\n');
	return (1);
}
