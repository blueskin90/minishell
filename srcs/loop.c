/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 02:23:08 by toliver           #+#    #+#             */
/*   Updated: 2018/09/01 09:01:57 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				printenv(t_envs *env)
{
	int			i;

	i = 0;
	while (env->envp && env->envp[i])
	{
		ft_putendl(env->envp[i]);
		i++;
	}
	return (1);
}

int				echo(char **instructions)
{
	int			i;
	int			isfirst;

	i = 1;
	isfirst = 1;
	while (instructions && instructions[i])
	{
		if (isfirst == 0)
			ft_putchar(' ');
		else
			isfirst = 0;
		ft_putstr(instructions[i]);
		i++;
	}
	return (1);
}

int				isquote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

int				iswhitespace(char c)
{
	if (c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f'
			|| c == ' ')
		return (1);
	return (0); // attention, ptet pas return 1 si c'est /n
}

int				get_wordnumber(char *line)
{
	int			i;
	int			wrdnbr;
	char		lastquote;

	i = 0;
	wrdnbr = 0;
	while (line && line[i])
	{
		while (iswhitespace(line[i]))
			i++;
		if (line[i] && !iswhitespace(line[i]) && !isquote(line[i]) && ++wrdnbr)
		{
			while (line[i] && !iswhitespace(line[i]) && !isquote(line[i]))
				i++;
		}
		if (line[i] && isquote(line[i]) && ++wrdnbr)
		{
			lastquote = line[i];
			i++;
			while (line[i] && line[i] != lastquote)
				i++;
			if (line[i] == lastquote)
				i++;
		}
	}
	return (wrdnbr);
}

int				get_firstword(char *line)
{
	int			i;

	i = 0;
	while (line && line[i] && iswhitespace(line[i]))
		i++;
	return (i);
}

int				get_nextword(char *line)
{
	int			i;
	char		c;

	i = 0;
	if (isquote(*line))
	{
		c = *line;
		i++;
		while (line && line[i] && line[i] != c)
			i++;
		if (line && line[i] && line[i] == c)
			i++;
		while (line && line[i] && iswhitespace(line[i]))
			i++;
	}
	else
	{
		while (line && line[i] && !(iswhitespace(line[i])))
			i++;
		while (line && line[i] && iswhitespace(line[i]))
			i++;
	}
	return (i);
}

int				wordsize(char *line)
{
	int			i;
	char		c;

	i = 0;
	if (isquote(*line))
	{
		c = *line;
		i++;
		while (line && line[i] && line[i] != c)
			i++;
		if (line[i] == c)
			return (i - 1);
	}
	else
	{
		while (line && line[i] && !(iswhitespace(line[i]))
				&& !(isquote(line[i])))
			i++;
	}
	return (i);
}

int				addstring(char *str, char **dst, char c, char ***tab)
{
	(void)dst;
	(void)str;
	(void)tab;
	(void)c;
	int			i;

	i = 0;
	while ((*tab)[i] != *dst)
	{
		i++;
	}
	ft_printf("last = '%s' verif = '%s'\n", *tab[i], *dst);
	return (0);
}

int				waitforquotes(char c, char ***tab)
{
	int			isover;
//	char		*str;
//	int			returnval;

	isover = 0;
	/*
	while (isover != 1)
	{
		if (c == '\'')
			ft_printf("quote>");
		else
			ft_printf("dquote>");
		if (ft_get_next_line(1, &str) == -1)
			return (-1);
		if ((returnval = addstring(str, dst, c, tab)) == -1)
			return (-1);
		if (returnval == 0)
			isover = 1;
		free(str);
	}
	*/
	(void)c;
	(void)tab;
	return (1);
}
int				wordcopy(char *line, char **dst, char ***tab)
{
	char		c;
	int			i;

	i = 0;
	if (isquote(*line))
	{
		c = *line;
		line++;
		while (line[i] && line[i] != c)
		{
			(*dst)[i] = line[i];
			i++;
		}
		(*dst)[i] = '\0';
	if (line[i] != c)
			return (waitforquotes(c, tab));
		return (1);
	}
	while (line[i] && !iswhitespace(line[i]) && !isquote(line[i]))
	{
		(*dst)[i] = line[i];
		i++;
	}
	(*dst)[i] = '\0';
	(void)tab;
	return (1);
}

int				splitinstructions(char *line, char ***instructions)
{
	char		**split;
	int			wrdnumber;
	int			i;

	split = NULL;
	if ((wrdnumber = get_wordnumber(line)) != 0)
	{
		if (!(split = (char**)malloc(sizeof(char*) * (wrdnumber + 1))))
			return (-1);
		split[wrdnumber] = NULL;
		line += get_firstword(line);
		i = 0;
		while (*line)
		{
			if (!(split[i] = (char*)malloc(sizeof(char) * (wordsize(line) + 1))))
				return (-1);
			if (wordcopy(line, &split[i], &split) == -1)
				return (-1);
			line += get_nextword(line);
			i++;
		}
	}
	*instructions = split;
	return (1); // penser si le dernier est encore ouvert
}

// faire un nouveau splitinstructions en liste chainee, plus pratix

int				splitline(char *line, t_list *ptr)
{
	char		c;

	if (isquote(*line) && !(ptr->next = ft_lstnew(line + 1, wordsize(line) + 1)))
		return (-1);
	if (!isquote(*line) && !(ptr->next = ft_lstnew(line, wordsize(line) + 1)))
		return (-1);
	c = *line;
	if (isquote(*line))
		((char*)(ptr->next->content))[wordsize(line + 1)] = '\0';
	else
		((char*)(ptr->next->content))[wordsize(line)] = '\0';
	if (line[wordsize(line + 1)] != c)
		ft_printf("incomplete char = %c and end char = %c\n", c, line[wordsize(line + 1)]);
	return (1);
}

int				execline(t_envs *env, char *line)
{
	char		**splittedline;

	if (splitinstructions(line, &splittedline) == -1)
		env->running = -1;
	// a la place de splitinstructions (comme splitwhitespaces, faire un split en liste chainee)
	if (!splittedline || !splittedline[0])
		return (1);
	if (ft_strcmp(splittedline[0], "env") == 0)
		printenv(env);
	else if (ft_strcmp(splittedline[0], "echo") == 0)
		echo(splittedline);
	return (1);
}

int				loop(t_envs *env)
{
	char		*str;

	while (env->running > 0)
	{
		ft_putstr("$>");
		if (ft_get_next_line(1, &str) == -1)
			env->running = MALLOC_FAILED;
		execline(env, str);
		free(str);
	}
	return (1);
}
