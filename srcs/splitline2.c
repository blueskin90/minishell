/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitline2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 17:50:52 by toliver           #+#    #+#             */
/*   Updated: 2018/10/24 13:46:26 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				expandhome(char **line, char *homepath, t_envs *env)
{
	char		*expandedpath;

	if (!(expandedpath = (char*)malloc(sizeof(char) *
					(ft_strlen(*line) + ft_strlen(homepath)))))
		returnval(-1, env);
	ft_strcpy(expandedpath, homepath);
	ft_strcat(expandedpath, *line + 1);
	free(*line);
	*line = expandedpath;
	return (1);
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
