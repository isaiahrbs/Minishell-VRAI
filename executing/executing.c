/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimatayi <dimatayi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:57:27 by dimatayi          #+#    #+#             */
/*   Updated: 2025/03/18 05:41:46 by dimatayi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_free(char **executable, char ***cmd)
{
	if (*executable)
	{
		free(*executable);
		*executable = NULL;
	}
	if (*cmd)
		free_double_ptr(*cmd);
}

int	ft_executable(char **executable, char ***cmd, t_data *data)
{
	t_token	*env_var;

	env_var = data->env_list;
	if (!strcmp("env", *executable))
	{
		while (env_var && env_var->name && env_var->content)
		{
			printf("%s=%s\n", env_var->name, env_var->content);
			env_var = env_var->next;
		}
		ft_free(executable, cmd);
		exit(0);
	}
	if (!ft_exec(*executable, *cmd, data))
		ft_free(executable, cmd);
	if (data->error == MALLOC_ERROR)
		exit(1);
	printf("command not found\n");
	exit(0);
}

int	child(t_command *tmp, int *prev_pipe_read, int *fd, t_data *data)
{
	char	**cmd;
	char	*executable;
	int		infile;
	int		outfile;

	infile = 0;
	outfile = 1;
	cmd = NULL;
	executable = NULL;
	while (tmp && tmp->value && tmp->type != PIPE)
	{
		is_cmd(tmp, &executable, &cmd);
		if (is_redirection(tmp, &executable, &cmd))
		{
			redirout_append(tmp, &outfile, &executable, &cmd);
			redirect_in(tmp, &infile, &executable, &cmd);
			tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	edit_pipe_fd(infile, outfile, prev_pipe_read, fd);
	if (executable)
		ft_executable(&executable, &cmd, data);
	exit(0);
}

t_command	*parent(int *prev_pipe_read, int *fd, t_command *tmp, int *child_failed	)
{
		int		status;
		pid_t	wait_result;

	*child_failed = 0;
	if (*prev_pipe_read != -1)
		close(*prev_pipe_read);
	if (fd[1] != -1)
		close(fd[1]);
	*prev_pipe_read = fd[0];
	while (tmp && tmp->value && tmp-> type != PIPE)
		tmp = tmp->next;
	if (tmp && tmp->type == PIPE)
		tmp = tmp->next;
	wait_result = wait(&status);
	while (wait_result > 0)
		wait_result = wait(&status);
	return (tmp);
}

int	executing(t_data *data)
{
	t_command	*tmp;
	int			prev_pipe_read;
	int			fd[2];
	pid_t		pid;
	int			child_failed;

	data->error = NO_TYPE;
	prev_pipe_read = -1;
	if (!data->commands)
		return (0);
	tmp = data->commands;
	while (tmp && tmp->value)
	{
		if (ft_pipe(tmp, fd))
			return (1);
		if (ft_fork(&pid, &prev_pipe_read, fd))
			return (1);
		if (pid == 0)
			child(tmp, &prev_pipe_read, fd, data);
		tmp = parent(&prev_pipe_read, fd, tmp, &child_failed);
		/* if (child_failed)
			return (1); */
	}
	return (0);
}

//executable argument redirection file pipe
/*[echo hello] [<] [text.txt] [ | ] [ls -l] [>] [output.txt]
[ls -l] [>] [djjfkdsjfjsk skskdjsk] [ | ] [cat text.txt ls cat echo] */