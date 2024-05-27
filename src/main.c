/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malena-b <mario3d93@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 11:42:59 by malena-b          #+#    #+#             */
/*   Updated: 2024/05/27 13:18:35 by malena-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	execute(char *cmd, char **env)
{
	char	**cmd_mat;
	char	*path;

	cmd_mat = ft_split(cmd, ' ');
	path = get_path(cmd_mat[0], env);
	if (execve(path, cmd_mat, env) == -1)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(cmd_mat[0], 2);
		ft_free_tab(cmd_mat);
		exit(1);
	}
}

void	c_process(char **argv, int *pipe_fd, char **env)
{
	int		fd;

	fd = open(argv[1], O_RDONLY);
	dup2(fd, 0);
	dup2(pipe_fd[1], 1);
	close(pipe_fd[0]);
	execute(argv[2], env);
}

void	p_process(char **argv, int *pipe_fd, char **env)
{
	int		fd;
//añadir comprobación de errores en las siguientes llamadas
	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC , 0644);
	dup2(fd, 1);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[1]);
	execute(argv[3], env);
}

int	main(int argc, char **argv, char **env)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (argc != 5)
		report_and_exit(1);
	if (pipe(pipe_fd) == -1)
		report_and_exit(2);
	pid = fork();
	if (pid == -1)
		report_and_exit(3);
	if (pid == 0)
		c_process(argv, pipe_fd, env);
	else
		p_process(argv, pipe_fd, env);
	char *c=malloc(500);
	(void)c;
	return (0);
}
