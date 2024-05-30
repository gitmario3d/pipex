/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malena-b <mario3d93@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 11:42:59 by malena-b          #+#    #+#             */
/*   Updated: 2024/05/30 11:55:30 by malena-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	execution(char *commands, char **env)
{
	char	**commands_mat;
	char	*path;

	commands_mat = NULL;
	commands_mat = ft_split(commands, ' ', 0, 0);
	path = get_path(commands_mat[0], env);
	execve(path, commands_mat, env);
	ft_putstr_fd("pipex: couldn't find the next command: ", 2);
	ft_putendl_fd(commands_mat[0], 2);
	ft_free_mat(commands_mat);
	exit(1);
}

void	c_process(char **argv, int *pipe_fd, char **env)
{
	int		fd;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		report_and_exit("Error in the open function.\n");
	dup2(fd, 0);
	dup2(pipe_fd[1], 1);
	close(pipe_fd[0]);
	execution(argv[2], env);
}

void	p_process(char **argv, int *pipe_fd, char **env)
{
	int			fd;
	pid_t		pid;

	waitpid(0, 0, 0);
	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		report_and_exit("Error in the open function.\n");
	dup2(fd, 1);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[1]);
	pid = fork();
	if (pid == -1)
		report_and_exit("Error at child process creation (fork).");
	if (pid == 0)
	{
		close(fd);
		execution(argv[3], env);
	}
	waitpid(0, 0, 0);
	close(fd);
}

int	main(int argc, char **argv, char **env)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (argc != 5)
		report_and_exit("Wrong inputs (./pipex inf commands commands outf)");
	if (pipe(pipe_fd) == -1)
		report_and_exit("Error at pipe creation.");
	pid = fork();
	if (pid == -1)
		report_and_exit("Error at child process creation (fork).");
	if (pid == 0)
		c_process(argv, pipe_fd, env);
	else
		p_process(argv, pipe_fd, env);
	return (0);
}
