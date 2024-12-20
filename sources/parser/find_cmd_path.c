/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:37:17 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/16 14:07:27 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			get_cmd_path(t_shell *mini, t_cmd *cmd);
static char	*search_command_in_paths(char **paths, t_cmd *cmd);
static int	check_abs_path(t_shell *mini, t_cmd *cmd);
int			check_special_cases(t_shell *mini, t_cmd *cmd);
void		cmd_error_and_exit_stat(t_shell *mini, t_cmd *cmd, int exit_status);
int			check_for_directory(t_shell *mini, t_cmd *cmd);

// Resolves absolute path for command

int	get_cmd_path(t_shell *mini, t_cmd *cmd)
{
	char	*paths_str;
	char	**paths;
	t_env	*temp;
	int		abs_path_status;

	if (cmd->command == NULL || ft_strlen(cmd->command) == 0)
	{
		mini->exit_stat = 0;
		return (1);
	}
	abs_path_status = check_abs_path(mini, cmd);
	if (abs_path_status == 0)
		return (0);
	if (abs_path_status != 1)
		return (mini->exit_stat != 0);
	temp = mini->env;
	while (temp)
	{
		if (ft_strncmp(temp->name, "PATH", 4) == 0)
			paths_str = temp->value;
		temp = temp->next;
	}
	paths = ft_split(paths_str, ':');
	if (!paths)
	{
		perror("Failed to split PATH");
		return (1);
	}
	cmd->cmd_path = search_command_in_paths(paths, cmd);
	if (!cmd->cmd_path)
	{
		cmd_error_and_exit_stat(mini, cmd, 127);
		return (1);
	}
	return (0);
}

// Tests each possible path until finds one that works,
// returns correct absolute path

static char	*search_command_in_paths(char **paths, t_cmd *cmd)
{
	char	*cmd_path;
	char	*temp_cmd;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp_cmd = ft_strjoin(paths[i], "/");
		if (!temp_cmd)
			break ;
		cmd_path = ft_strjoin(temp_cmd, cmd->command);
		free(temp_cmd);
		if (!cmd_path)
			break ;
		if (access(cmd_path, X_OK) == 0)
		{
			ft_free_array(paths);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	ft_free_array(paths);
	return (NULL);
}

// Checks whether command is already an absolute path

static int	check_abs_path(t_shell *mini, t_cmd *cmd)
{
	if (check_special_cases(mini, cmd))
		return (-1);
	if (cmd->command[0] == '/'
		|| (cmd->command[0] == '.' && cmd->command[1] == '/'))
	{
		if (check_for_directory(mini, cmd))
			return (-1);
		if (access(cmd->command, X_OK) == 0)
		{
			cmd->cmd_path = cmd->command;
			return (0);
		}
		if (access(cmd->command, F_OK) == 0 && access(cmd->command, X_OK) != 0)
		{
			ft_putstr_fd(cmd->command, 2);
			ft_putendl_fd(": Permission denied", 2);
			mini->exit_stat = 126;
			return (-1);
		}
		ft_putstr_fd(cmd->command, 2);
		ft_putendl_fd(": No such file or directory", 2);
		mini->exit_stat = 127;
		return (-1);
	}
	return (1);
}

int	check_special_cases(t_shell *mini, t_cmd *cmd)
{
	if (cmd->command[0]  == '~' && !cmd->command[1])
	{
		ft_putstr_fd(cmd->command, 2);
		ft_putendl_fd(": Is a directory", 2);
		mini->exit_stat = 126;
		return (1);
	}
	if (cmd->command[0]  == '.' && !cmd->command[1])
	{
		ft_putstr_fd(cmd->command, 2);
		ft_putendl_fd(": filename argument required", 2);
		mini->exit_stat = 127;
		return (1);
	}
	if (cmd->command[0] && cmd->command[1] && !cmd->command[2])
	{
		if (cmd->command[0] == '.' && cmd->command[1] == '.')
		{
			ft_putstr_fd(cmd->command, 2);
			ft_putendl_fd(": command not found", 2);
			mini->exit_stat = 127;
			return (1);
		}
	}
	return (0);
}

int	check_for_directory(t_shell *mini, t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->command, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		ft_putstr_fd(cmd->command, 2);
		ft_putendl_fd(": Is a directory", 2);
		mini->exit_stat = 126;
		return (1);
	}
	return (0);
}

void	cmd_error_and_exit_stat(t_shell *mini, t_cmd *cmd, int exit_status)
{
	ft_putstr_fd(cmd->command, 2);
	ft_putendl_fd(": command not found", 2);
	mini->exit_stat = exit_status;
}
