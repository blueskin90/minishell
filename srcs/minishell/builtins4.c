/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 09:38:46 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 13:19:13 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_cd_internal(t_env *env)
{
	if (env->command[1] == NULL)
		return (ft_cd_home(env));
	else if (env->command[1] && env->command[1][0] == '-'
			&& env->command[1][1] == '\0')
		return (ft_cd_minus_internal(env));
	else
		return (ft_cd(env, env->command[1]));
	return (1);
}

int				ft_exec_command(t_env *env)
{
	char		*str;

	str = env->command[0];
	if (str == NULL)
		return (1);
	if (!str[0])
		return (1);
	if (!(ft_envp_set_(env)))
		return (0);
	if (ft_strnequ("env", str, 3) && !str[3])
		return (ft_env_internal(env));
	if (ft_strnequ("unsetenv", str, 8) && !str[8])
		return (ft_unsetenv_internal(env));
	if (ft_strnequ("setenv", str, 6) && !str[6])
		return (ft_setenv_internal(env));
	if (ft_strnequ("echo", str, 4) && !str[4])
		return (ft_echo_internal(env));
	if (ft_strnequ("cd", str, 2) && !str[2])
		return (ft_cd_internal(env));
	if (ft_strnequ("exit", str, 4) && !str[4])
	{
		ft_printf("exit\n");
		return (0);
	}
	return (ft_exec(env));
}

int				ft_split_command(t_env *env, char *str)
{
	ft_free_command(env->command);
	if (!(env->command = ft_split_charset(str, "\t\n\r\v\f ")))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	return (1);
}

static void		ft_expand_tilde_fuck_the_norm(int *i, char *str,
		char *expanded, char *home)
{
	*i = 0;
	while (*str)
	{
		if (*str == '~')
		{
			ft_strcat(expanded, home);
			*i += ft_strlen(home);
		}
		else
		{
			expanded[*i] = *str;
			(*i)++;
		}
		str++;
	}
	expanded[*i] = '\0';
}

char			*ft_expand_tilde(t_env *env, char *str)
{
	char		*home;
	int			i;
	int			count;
	char		*expanded;

	home = ft_env_get_value(env, "HOME");
	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '~')
			count++;
		i++;
	}
	if (count && home == NULL)
		ft_warning(EXPAND, MISSING_HOME, NULL, env);
	if (home == NULL)
		home = "";
	if (!(expanded = (char*)malloc(sizeof(char) *
					(count * ft_strlen(home) + i + 1))))
		return (NULL);
	ft_bzero(expanded, count * ft_strlen(home) + i + 1);
	ft_expand_tilde_fuck_the_norm(&i, str, expanded, home);
	return (expanded);
}
