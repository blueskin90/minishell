/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 21:23:31 by toliver           #+#    #+#             */
/*   Updated: 2018/09/04 21:38:50 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			while (line[i] && !iswhitespace(line[i]) && !isquote(line[i]))
				i++;
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

int				wordcopy(char *line, char **dst)
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
		return (1);
	}
	while (line[i] && !iswhitespace(line[i]) && !isquote(line[i]))
	{
		(*dst)[i] = line[i];
		i++;
	}
	(*dst)[i] = '\0';
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
			if (!(split[i] =
						(char*)malloc(sizeof(char) * (wordsize(line) + 1))))
				return (-1);
			if (wordcopy(line, &split[i]) == -1)
				return (-1);
			line += get_nextword(line);
			i++;
		}
	}
	*instructions = split;
	return (1);
}

int				splitline(char *line, t_list *ptr)
{
	char		c;

	if (isquote(*line)
			&& !(ptr->next = ft_lstnew(line + 1, wordsize(line) + 1)))
		return (-1);
	if (!isquote(*line) && !(ptr->next = ft_lstnew(line, wordsize(line) + 1)))
		return (-1);
	c = *line;
	if (isquote(*line))
		((char*)(ptr->next->content))[wordsize(line + 1)] = '\0';
	else
		((char*)(ptr->next->content))[wordsize(line)] = '\0';
	if (line[wordsize(line + 1)] != c)
		ft_printf("incomplete char = %c and end char = %c\n", c,
				line[wordsize(line + 1)]);
	return (1);
}
