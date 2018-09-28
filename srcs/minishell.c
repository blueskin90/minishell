/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 01:24:55 by toliver           #+#    #+#             */
/*   Updated: 2018/09/09 16:43:58 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				main(int argc, char **argv, char **envp)
{
	t_envs		env;

	init(argc, argv, envp, &env);
	loop(&env);
	exitshell(&env);
	return (1);
}
