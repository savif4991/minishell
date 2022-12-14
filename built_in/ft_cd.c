/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 14:28:14 by daejlee           #+#    #+#             */
/*   Updated: 2023/01/11 14:35:34 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

static int	set_3(t_ft_cd *p, char **dir_adr)
{
	if (!*dir_adr)
		return (chdir(p->env_home));
	if (!ft_strncmp(*dir_adr, "-", ft_strlen(*dir_adr)))
	{
		if (!p->node)
		{
			write(2, "minishell: ft_cd: OLDPWD not set\n", 34);
			return (0);
		}
		else
		{
			printf("%s\n", p->node->value);
			return (chdir(p->node->value));
		}
	}
	else if (!ft_strncmp(*dir_adr, "~", ft_strlen(*dir_adr)))
	{
		if (!p->env_home)
		{
			write(2, "minishell: ft_cd: HOME not set\n", 32);
			return (0);
		}
		else
			return (chdir(p->env_home));
	}
	return (1);
}

static int	set_1(t_ft_cd *p, char **dir_adr, t_env *env)
{
	p->env_home = get_env_val("HOME", env);
	p->env_cdpath = get_env_val("CDPATH", env);
	p->env_pwd = get_env_val("PWD", env);
	p->node = find_env(env, "OLDPWD");
	if (!set_3(p, dir_adr))
		return (0);
	if (p->node)
	{
		free (p->node->value);
		p->node->value = p->env_pwd;
	}
	if (!*dir_adr && (!p->env_home || p->env_home[0] == '\0'))
		return (1);
	else if (!*dir_adr)
		*dir_adr = p->env_home;
	return (1);
}

static int	set_2(char *dir, t_ft_cd p, char **curpath)
{
	char	*temp_free;

	if (dir[0] == '/')
		*curpath = ft_strdup(dir);
	else if (!ft_strncmp(dir, "..", 2) || dir[0] == '.'
		|| check_cdpath(curpath, p.env_cdpath))
			*curpath = ft_strdup(dir);
	if (!*curpath)
		return (-1);
	if (*curpath[0] != '/')
	{
		temp_free = *curpath;
		if (p.env_pwd[ft_strlen(p.env_pwd) - 1] != '/')
			*curpath = ft_strjoin_modified(p.env_pwd, *curpath, '/');
		else
			*curpath = ft_strjoin(p.env_pwd, *curpath);
		free(temp_free);
		if (!*curpath)
			return (-1);
	}
	if (!*curpath)
		return (-1);
	return (0);
}

/**
 * @brief 
 * ????????? cd ???????????????. ?????? ?????? ?????? ?????? ???????????? ???????????????. ????????? ???????????? ????????????.
 * original : cd [-L|-P] [directory]
 * @return int ?????? ??? 0, ?????? ??? -1
 */
int	ft_cd(char *dir, t_env *env)
{
	t_ft_cd	p;
	char	*curpath;
	int		ret;

	if (!set_1(&p, &dir, env))
		return (0);
	if (set_2(dir, p, &curpath) == -1)
		return (-1);
	ret = chdir(curpath);
	p.node = find_env(env, "PWD");
	(*p.node).value = getcwd(NULL, 0);
	if (ret)
	{
		write(2, "ft_cd: no such file or directory: ", 35);
		write(2, dir, ft_strlen(dir));
		write(2, "\n", 1);
	}
	free(curpath);
	return (ret);
}
