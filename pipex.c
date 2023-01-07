/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asioud <asioud@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 05:55:07 by asioud            #+#    #+#             */
/*   Updated: 2023/01/02 19:07:49 by asioud           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/includes/libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	execute(char *argv, char **envp);
void	child_process(char **argv, int *fd, char **envp);
void	parent_task(char **argv, int *fd, char **envp);
char	*get_cmd_path(char *cmd, char **envp);

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid;

	if (argc == 5)
	{
		if (pipe(fd) == -1)
			display_error("\e[1;31mPipe failed", 1);
		pid = fork();
		if (pid == -1)
			display_error("\e[1;31mFork failed", 1);
		else if (pid == 0)
			child_process(argv, fd, envp);
		parent_task(argv, fd, envp);
	}
	else
	{
		printf("\e[1;31mINPUT ERROR\n");
	}
}

void	child_process(char **argv, int *fd, char **envp)
{
	int	filein;

	filein = open_file(argv[1], 2);
	dup2(fd[1], STDOUT_FILENO);
	dup2(filein, STDIN_FILENO);
	close(fd[0]);
	execute(argv[2], envp);
}

void	parent_task(char **argv, int *fd, char **envp)
{
	int	fileout;

	fileout = open_file(argv[4], 1);
	dup2(fd[0], STDIN_FILENO);
	dup2(fileout, STDOUT_FILENO);
	close(fd[1]);
	execute(argv[3], envp);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	int		i;
	char	*path;
	char	**paths;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i], ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin_free(paths[i], ft_strjoin("/", cmd));
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

void	execute(char *argv, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(argv, ' ');
	path = get_cmd_path(cmd[0], envp);
	if (path == NULL)
	{
		free_2d(cmd);
		display_error("\e[1;31m404 cmd Path Not Found", 1);
	}
	if (execve(path, cmd, envp) == -1)
		display_error("\e[1;31mExecution Failed", 1);
}

// ./pipex_bonus pipex.c cat "grep main" outfile