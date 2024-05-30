/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malena-b <mario3d93@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:16:06 by malena-b          #+#    #+#             */
/*   Updated: 2024/05/30 11:55:55 by malena-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	report_and_exit(char *error_msg)
{
	ft_putstr_fd(error_msg, 2);
	exit (1);
}

void	ft_free_mat(char **mat)
{
	size_t	i;

	i = 0;
	while (mat[i])
		free(mat[i++]);
	free(mat);
}

char	*ft_getenv(char *path_name, char **env)
{
	char	*substr;
	int		i;
	int		j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		substr = ft_substr(env[i], 0, j);
		if (ft_strncmp(substr, path_name, ft_strlen(substr)) == 0)
		{
			free(substr);
			return (env[i] + j + 1);
		}
		free(substr);
		i++;
	}
	return (NULL);
}

char	*get_path(char *commands, char **env)
{
	int		i;
	char	*exec;
	char	**total_paths;
	char	*actual_path;
	char	**s_commands;

	i = -1;
	total_paths = ft_split(ft_getenv("PATH", env), ':', 0, 0);
	s_commands = ft_split(commands, ' ', 0, 0);
	while (total_paths[++i])
	{
		actual_path = ft_strjoin(total_paths[i], "/");
		exec = ft_strjoin(actual_path, s_commands[0]);
		free(actual_path);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_mat(s_commands);
			return (exec);
		}
		free(exec);
	}
	ft_free_mat(total_paths);
	ft_free_mat(s_commands);
	return (commands);
}
