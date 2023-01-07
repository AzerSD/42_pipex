/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asioud <asioud@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/31 21:22:29 by asioud            #+#    #+#             */
/*   Updated: 2023/01/02 19:16:03 by asioud           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/includes/libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char	*get_cmd_path(char *cmd, char **envp);
void	execute(char *argv, char **envp);
void	child_process(char *argv, char **envp);
void	here_doc(int argc, char *delimiter);
int		main(int argc, char **argv, char **envp);

int	main(int argc, char **argv, char **envp)
{
	int	nb_cmd;
	int	fd[2];

	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		{
			nb_cmd = 3;
			fd[1] = open_file(argv[argc - 1], 0);
			here_doc(argc, argv[2]);
		}
		else
		{
			nb_cmd = 2;
			fd[1] = open_file(argv[argc - 1], 1);
			fd[0] = open_file(argv[1], 2);
			dup2(fd[0], STDIN_FILENO);
		}
		while (nb_cmd < argc - 2)
			child_process(argv[nb_cmd++], envp);
		dup2(fd[1], STDOUT_FILENO);
		execute(argv[argc - 2], envp);
	}
	display_error("\e[1;31mInput Error\n", 1);
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
		display_error("\e[1;31mPath Error", 1);
	}
	if (execve(path, cmd, envp) == -1)
		display_error("\e[1;31mExec Error", 1);
}

void	child_process(char *argv, char **envp)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		display_error("\e[1;31mChildp Pipe Error", 1);
	pid = fork();
	if (pid == -1)
		display_error("\e[1;31mChildp Fork Error", 1);
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execute(argv, envp);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

void	here_doc(int argc, char *delimiter)
{
	pid_t	pid;
	int		fd[2];
	char	*line;

	if (argc < 6)
		display_error("\e[1;31mHere_doc wrong arguments", 1);
	if (pipe(fd) == -1)
		display_error("\e[1;31mHere_doc Pipe Error", 1);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		while (ft_fgets(&line))
		{
			if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
				exit(EXIT_FAILURE);
			write(fd[1], line, ft_strlen(line));
		}
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		wait(NULL);
	}
}

// ./pipex_bonus here_doc eof cat "grep main" outfile
// << eof cat | "grep main" |  >> outfile 