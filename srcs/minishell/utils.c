/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:54:46 by toliver           #+#    #+#             */
/*   Updated: 2020/01/12 19:10:38 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_crash(int value, char *param, t_env *env)
{
	(void)param;
	if (value == MALLOC_FAIL)
		ft_dprintf(2, "%s: Malloc failed\n", env->prog_name);
	else if (value == MISSING_CWD)
		ft_dprintf(2, "%s: Couldn't retrive CWD\n", env->prog_name);
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
	else if (origin == EXPAND)
		return ("expansion");
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
	else if (value == MISSING_HOME)
		ft_dprintf(2, "$HOME is not set !\n");
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




int		ft_is_charset(char c, char *charset)
{
	while (*charset)
	{
		if (c == *charset)
			return (1);
		charset++;
	}
	return (0);
}

char			**ft_split_charset_allocarray(char *str, char *charset)
{
	int			nbr;
	char		**array;

	nbr = 0;
	while (str && *str)
	{
		if (!ft_is_charset(*str, charset))
		{
			nbr++;
			while (*str && !ft_is_charset(*str, charset))
				str++;
		}
		while (*str && ft_is_charset(*str, charset))
			str++;
	}
	array = (char**)malloc(sizeof(char*) * (nbr + 1));
	ft_bzero(array, sizeof(char*) * (nbr + 1));
	return (array);
}

int				ft_split_charset_allocstr(char *str, char *charset, char **arr)
{
	int			wrd;
	int			i;

	wrd = 0;
	while (str && *str)
	{
		if (!ft_is_charset(*str, charset))
		{
			i = 0;
			while (str[i] && !ft_is_charset(str[i], charset))
				i++;
			if (!(arr[wrd] = (char*)malloc(sizeof(char) * (i + 1))))
				return (0);
			ft_strncpy(arr[wrd], str, i);
			arr[wrd][i] = '\0';
			str += i;
			wrd++;
		}
		while (*str && ft_is_charset(*str, charset))
			str++;
	}
	return (1);
}

char			**ft_split_free(char **array)
{
	int			i;

	i = 0;
	while (array && array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}

char			**ft_split_charset(char *str, char *charset)
{
	char		**array;

	if (!(array = ft_split_charset_allocarray(str, charset)))
		return (NULL);
	if (!ft_split_charset_allocstr(str, charset, array))
		return (ft_split_free(array));
	return (array);
}
