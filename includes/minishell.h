/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:07:10 by toliver           #+#    #+#             */
/*   Updated: 2020/02/04 18:18:59 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "libftprintf.h"

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <limits.h>

/*
** FONCTIONS AUTORISEES
** malloc, free, access, open, close, read, write, opendir, readdir, closedir,
** getcwd, chdir, stat, lstat, fstat, fork, execve, wait, waitpid, wait3, wait4
** signal, kill, exit
*/

enum					e_crash
{
	MALLOC_FAIL,
};

enum					e_error_origin
{
	ENV,
	SETENV,
	UNSETENV,
	EXPAND,
	CD,
	EXEC,
};

enum					e_warning_type
{
	MISSING_VARIABLE,
	MISSING_EQUAL,
	MISSING_HOME,
	MISSING_OLDPWD,
	MISSING_CWD,
	FORK_FAILED,
	DOESNT_EXIST,
	CANT_EXECUTE,
	NOT_FOUND,
};

typedef struct			s_envp
{
	char				**env;
	int					size;
	int					occupied;
}						t_envp;

typedef struct			s_env
{
	char				*prog_name;
	t_envp				envp;
	char				**commands;
	char				**command;
	pid_t				pid;
	char				*path;
}						t_env;

/*
** INIT FUNCTIONS
*/

int						ft_env_init(t_env *env, char **av, char **envp);
void					ft_free_env(t_env *env);
void					ft_free_command(char **command);
t_env					*ft_get_env(void);
void					ft_set_env(t_env *env);
/*
** ENV HANDLING FUNCTIONS
*/

int						ft_envp_set_(t_env *env);
int						ft_envp_realloc(t_env *env, int size);
int						ft_env(t_env *env);
int						ft_setenv(t_env *env, char *value);
int						ft_unsetenv(t_env *env, char *value);
int						ft_addenv(t_env *env, char *value);
char					*ft_env_get_value(t_env *env, char *value);
int						ft_env_get_index(t_env *env, char *value);
int						ft_addenv(t_env *env, char *value);
void					ft_remove_env(t_env *env, int index);
int						ft_unsetenv(t_env *env, char *value);
int						ft_setenv(t_env *env, char *param);
char					*ft_env_get_value(t_env *env, char *value);
int						ft_env_get_index(t_env *env, char *value);
void					ft_dump_env(char **env);
int						ft_env_getsize(char **env);
char					**ft_env_add_param(char **env, char *param);
int						ft_env_add_params(char ***envp, t_env *env, int *i);
int						ft_envp_atoi(t_env *env, char *value);
int						ft_envp_shlevel(t_env *env);
int						ft_envp_getpwd(t_env *env);
int						ft_envp_set_(t_env *env);
int						ft_envp_get_(t_env *env);
int						ft_envp_realloc(t_env *env, int size);
char					*ft_envp_concat(char *str1, char *str2);
int						ft_envp_mandatory(t_env *env);
int						ft_envp_copy(t_env *env, char **envp, int size);
int						ft_envp_alloc_and_copy(t_env *env, char **envp);
int						ft_env_init(t_env *env, char **av, char **envp);

/*
** EXEC FUNCTIONS
*/

int						ft_exec_command(t_env *env);
int						ft_echo_internal(t_env *env);
int						ft_env_internal(t_env *env);
int						ft_setenv_internal(t_env *env);
int						ft_unsetenv_internal(t_env *env);
int						ft_exec_internal(char *path, char **argv,
							char **env_cpy, t_env *env);
int						ft_is_path(char *str);
int						ft_exec_findpath(t_env *env);
int						ft_exec(t_env *env);
void					ft_env_cpy_free(char **tofree);
char					**ft_env_cpy(t_env *env);
int						ft_check_path_error(char *path, t_env *env, int error);
int						ft_check_path(char *path, t_env *env);
char					*ft_path_combine(char *start, char *end, char *link);
int						ft_update_env(t_env *env, char *path, char *value);
int						ft_update_pwd(t_env *env, char *oldpwd);
int						ft_cd(t_env *env, char *path);
int						ft_cd_minus_internal(t_env *env);
int						ft_cd_home(t_env *env);
int						ft_cd_internal(t_env *env);
int						ft_exec_command(t_env *env);
int						ft_split_command(t_env *env, char *str);
char					*ft_expand_tilde(t_env *env, char *str);
char					**ft_split_variables_free(char **tab);
char					**ft_split_variables_alloc(char *str, int count);
char					**ft_split_variables(char *str);
int						ft_expand_replace_variable(t_env *env, char **tab);
char					*ft_expand_fuse(char **splitted);
char					*ft_expand_variable(t_env *env, char *str);
char					*ft_expand_str(t_env *env, char *str);
int						ft_split_commands(t_env *env, char *str);
int						ft_exec_commands(t_env *env);

/*
** UTILS
*/

char					*ft_envp_get_value(char **envp, char *value);
int						ft_gnl2(int fd, char **line);
int						ft_is_charset(char c, char *charset);
int						ft_error(int irogin, int value, char *param,
		t_env *env);
int						ft_crash(int value, char *param, t_env *env);
int						ft_warning(int origin, int value, char *param,
		t_env *env);
int						ft_usage(void);
char					**ft_split_charset(char *str, char *charset);
char					**ft_env_cpy(t_env *env);
void					ft_env_cpy_free(char **tofree);
char					**ft_split_free(char **array);
#endif
